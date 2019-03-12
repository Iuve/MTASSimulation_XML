

#include "LoadDecayData.hh"
#include "pugixml.hh"
#include "FermiDistribution.hh"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

static std::string g_xmlInputFileName;
void SetXmlInputFileName(std::string xmlFileName) {g_xmlInputFileName = xmlFileName;}

const double energyLevelUncertanity(3.); // used in FindPointerToLevel

double CalculateHalfLifeTimeInSeconds(double time, std::string unit)
{
    if (time == 0.0)
		return time;
    else
    {
			if(unit == "F")
				return time * pow(10., -9.);
			else if(unit == "MS")
				return time * 0.001;
			else if(unit == "US")
				return time * pow(10., -6.);
			else if(unit == "S")
			    return time;
			else if(unit == "M")
				return time * 60.;
			else if(unit == "H")
				return time * 3600.;
			else if(unit == "Y")
				return time * 31556926.;
			else
				return 0.;
    }
}

using namespace std;

LoadDecayData::LoadDecayData()
{
	
	LoadDataFromXml();
//	cout << "SUCCESSFUL DATA LOADING" << endl;

}

LoadDecayData::~LoadDecayData()
{

}

void LoadDecayData::LoadDataFromXml()
{
	pugi::xml_document dir;
    if (!dir.load_file(g_xmlInputFileName.c_str())) cout << "Error connected to " << g_xmlInputFileName << " file." << endl; // Exception

    for (pugi::xml_node nuclideFile = dir.child("NuclideFile"); nuclideFile; nuclideFile = nuclideFile.next_sibling("NuclideFile"))
    {
		allNuclides_.push_back(LoadNuclideData(nuclideFile.attribute("FileName").value()));
	}
	
	FindPointersToFinalLevels();
	
	int tempAtomicNumber = dir.child("StartLevel").attribute("AtomicNumber").as_int();
	int tempAtomicMass = dir.child("StartLevel").attribute("AtomicMass").as_int();
	double tempEnergy = dir.child("StartLevel").attribute("Energy").as_double();
	startLevel_ = FindPointerToLevel(tempAtomicNumber, tempAtomicMass, tempEnergy, energyLevelUncertanity);
	
	tempAtomicNumber = dir.child("StopLevel").attribute("AtomicNumber").as_int();
	tempAtomicMass = dir.child("StopLevel").attribute("AtomicMass").as_int();
	tempEnergy = dir.child("StopLevel").attribute("Energy").as_double();
	stopLevel_ = FindPointerToLevel(tempAtomicNumber, tempAtomicMass, tempEnergy, energyLevelUncertanity);
}


Nuclide LoadDecayData::LoadNuclideData(const string filename)
{	
	std::vector<Level> nuclideLevels;
	pugi::xml_document doc;
    if (!doc.load_file(filename.c_str())) cout << "Error connected to other xml file." << endl; // Exception

    pugi::xml_node nuclide = doc.child("Nuclide");
    int atNumber = nuclide.attribute("AtomicNumber").as_int();
	int atMass = nuclide.attribute("AtomicMass").as_int();
	double QBeta = nuclide.attribute("QBeta").as_double(); // not used atm
	
    for (pugi::xml_node level = nuclide.first_child(); level; level = level.next_sibling())
    {       
        std::vector<Transition> transitionsFromLvL;
        for (pugi::xml_node transition = level.first_child(); transition; transition = transition.next_sibling())
		{									
			string type = transition.attribute("Type").value();
			double transitionQval = transition.attribute("TransitionQValue").as_double();
			double intensity = transition.attribute("Intensity").as_double();
			double finalLvlEnergy = transition.child("TargetLevel").attribute("Energy").as_double();
			int finalLvlAtMass = transition.child("TargetLevel").attribute("AtomicMass").as_int();
			int finalLvlAtNumber = transition.child("TargetLevel").attribute("AtomicNumber").as_int();
						
			if(type == "B+")
			{
				FermiDistribution* temp = new FermiDistribution(atNumber, transitionQval, 1);
				transitionsFromLvL.push_back(Transition(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, finalLvlAtNumber, temp));
//				delete temp;
			}
			else if(type == "B-")
			{
				FermiDistribution* temp = new FermiDistribution(atNumber, transitionQval, -1);
				transitionsFromLvL.push_back(Transition(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, finalLvlAtNumber, temp));
//				delete temp;
			}
			else
			{
				if(type == "G")
				{
					pugi::xml_attribute attr = transition.attribute("ElectronConversionCoefficient");
					if(attr = attr.next_attribute())
					{
						//double shellElectonConvCoeff[numberOfShellIndexes_];
						double eCC = transition.attribute("ElectronConversionCoefficient").as_double(); // eCC == ElectronConversionCoefficient
						Transition tempTransition(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, 
						finalLvlAtNumber, atNumber, eCC);
						
						for (attr; attr; attr = attr.next_attribute())
						{
							double value;
							istringstream( attr.value() ) >> value;
							tempTransition.SetShellElectronConvCoef(attr.name(), value);
							//SetShellElectronConvCoef(attr.name(), value, shellElectonConvCoeff);
						}
						//cout << "ElectronConversionCoefficient: " << eCC << endl;
						transitionsFromLvL.push_back(tempTransition);
						
					}
					else transitionsFromLvL.push_back(Transition(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, 
					finalLvlAtNumber, atNumber));
				}
				else
					transitionsFromLvL.push_back(Transition(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, finalLvlAtNumber, atNumber));
			}		
		}
		double lvlEnergy = level.attribute("Energy").as_double();
		double lvlSpin = level.attribute("Spin").as_double();
		string lvlParity = level.attribute("Parity").value();
		double lvlHalfLifeTime = level.attribute("HalfLifeTime").as_double();
		string timeUnit = level.attribute("TimeUnit").value();
		double lvlHalfLifeTimeInSeconds = CalculateHalfLifeTimeInSeconds(lvlHalfLifeTime, timeUnit);
//		cout << "lvlHalfLifeTimeInSeconds = " << lvlHalfLifeTimeInSeconds << endl;
		
		nuclideLevels.push_back(Level(lvlEnergy, lvlSpin, lvlParity, lvlHalfLifeTimeInSeconds, transitionsFromLvL));
    }   
	
	return Nuclide(atNumber, atMass, nuclideLevels);
}

void LoadDecayData::FindPointersToFinalLevels()
{
	for ( auto it = allNuclides_.begin(); it != allNuclides_.end(); ++it )
	{
		int atNumber = it->GetAtomicNumber();
		int atMass = it->GetAtomicMass();
		for ( auto jt = it->GetNuclideLevels()->begin(); jt != it->GetNuclideLevels()->end(); ++jt )
		{
			for ( auto kt = jt->GetTransitions()->begin(); kt != jt->GetTransitions()->end(); ++kt )
			{
				string type = kt->GetParticleType();
				if(type == "G") // have to improve it
					kt->SetPointerToFinalLevel( FindPointerToLevel( kt->GetFinalLevelAtomicNumber(), kt->GetFinalLevelAtomicMass(), kt->GetFinalLevelEnergy(), energyLevelUncertanity ) );
				else
					kt->SetPointerToFinalLevel( FindPointerToLevel( kt->GetFinalLevelAtomicNumber(), kt->GetFinalLevelAtomicMass(), kt->GetFinalLevelEnergy(), 0.1 ) );
			}
		}
	}
}


Level* LoadDecayData::FindPointerToLevel(int atomicNumber, int atomicMass, double energy, double energyLvlUnc)
{
	for ( auto it = allNuclides_.begin(); it != allNuclides_.end(); ++it )
	{
		int atNumber = it->GetAtomicNumber();
		int atMass = it->GetAtomicMass();
		if(atomicNumber == atNumber && atomicMass == atMass)
			for ( auto jt = it->GetNuclideLevels()->begin(); jt != it->GetNuclideLevels()->end(); ++jt )
			{
				double temp = jt->GetLevelEnergy();
				if( ((temp - energyLvlUnc) <= energy) && ((temp + energyLvlUnc) >= energy) )
					return &(*jt);
			}
	}
	// throw Exception
	cout << "Level not found!" << endl;
	cout << atomicMass << " " << atomicNumber << " " << energy << endl;
}




