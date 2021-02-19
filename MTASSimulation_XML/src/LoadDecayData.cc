
#include "DeclareHeaders.hh"
#include "pugixml.hh"

#include <iostream>
#include <sstream>
#include <vector>

std::string g_xmlInputFileName;

void SetXmlInputFileName(std::string xmlFileName) {g_xmlInputFileName = xmlFileName;}

const double energyLevelUncertainty(0.2); // used in FindPointerToLevel

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

LoadDecayData::LoadDecayData(string xmlFileName)
{
	g_xmlInputFileName = xmlFileName;
	LoadDataFromXml();
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
	
	SetPointersToTransitions();
	
	FindPointersToFinalLevels();
	
	int tempAtomicNumber = dir.child("StartLevel").attribute("AtomicNumber").as_int();
	int tempAtomicMass = dir.child("StartLevel").attribute("AtomicMass").as_int();
	double tempEnergy = dir.child("StartLevel").attribute("Energy").as_double();
	startLevel_ = FindPointerToLevel(tempAtomicNumber, tempAtomicMass, tempEnergy, energyLevelUncertainty);
	
	tempAtomicNumber = dir.child("StopLevel").attribute("AtomicNumber").as_int();
	tempAtomicMass = dir.child("StopLevel").attribute("AtomicMass").as_int();
	tempEnergy = dir.child("StopLevel").attribute("Energy").as_double();
	stopLevel_ = FindPointerToLevel(tempAtomicNumber, tempAtomicMass, tempEnergy, energyLevelUncertainty);
	
	double tempTime = dir.child("EventLength").attribute("Value").as_double();
	string tempTimeUnit = dir.child("EventLength").attribute("TimeUnit").value();
	double tempTimeInSeconds = CalculateHalfLifeTimeInSeconds(tempTime, tempTimeUnit);
	eventDurationInSeconds_ = tempTimeInSeconds;
	
	tempTime = dir.child("CycleLength").attribute("Value").as_double();
	tempTimeUnit = dir.child("CycleLength").attribute("TimeUnit").value();
	tempTimeInSeconds = CalculateHalfLifeTimeInSeconds(tempTime, tempTimeUnit);
	cycleDurationInSeconds_ = tempTimeInSeconds;
	
	cout << "Event length set to " << eventDurationInSeconds_ << " seconds." << endl;
	cout << "Cycle length set to " << cycleDurationInSeconds_ << " seconds." << endl;
	
	if( !dir.child("SpecifyFirstTransition").empty() )
	{
		int nuclideAtomicNumber = dir.child("SpecifyFirstTransition").attribute("NuclideAtomicNumber").as_int();
		int nuclideAtomicMass = dir.child("SpecifyFirstTransition").attribute("NuclideAtomicMass").as_int();
		pugi::xml_node initialLevel = dir.child("SpecifyFirstTransition").child("InitialLevel");
		double initialLevelEnergy = initialLevel.attribute("Energy").as_double();
		pugi::xml_node specifyTransition = dir.child("SpecifyFirstTransition").child("SpecifyTransition");
		string specifiedType = specifyTransition.attribute("Type").value();
		double specifiedEnergy = specifyTransition.attribute("TransitionQValue").as_double();
		
		RecalculateIntensities(nuclideAtomicNumber, nuclideAtomicMass, initialLevelEnergy,
		specifiedType, specifiedEnergy);
		
        if( !dir.child("SpecifySecondTransition").empty() )
        {
            nuclideAtomicNumber = dir.child("SpecifySecondTransition").attribute("NuclideAtomicNumber").as_int();
            nuclideAtomicMass = dir.child("SpecifySecondTransition").attribute("NuclideAtomicMass").as_int();
            initialLevel = dir.child("SpecifySecondTransition").child("InitialLevel");
            initialLevelEnergy = initialLevel.attribute("Energy").as_double();
            specifyTransition = dir.child("SpecifySecondTransition").child("SpecifyTransition");
            specifiedType = specifyTransition.attribute("Type").value();
            specifiedEnergy = specifyTransition.attribute("TransitionQValue").as_double();

            RecalculateIntensities(nuclideAtomicNumber, nuclideAtomicMass, initialLevelEnergy,
            specifiedType, specifiedEnergy);
        }
	}
	
}


Nuclide LoadDecayData::LoadNuclideData(const string filename)
{	
	std::vector<Level> nuclideLevels;
	int atNumber;
	int atMass;
    double qBeta;
	
	pugi::xml_document doc;
    if (!doc.load_file(filename.c_str()))
    {
		cout << "Error connected to " << filename << " file. Returning empty nuclide." << endl; // Exception
		return Nuclide(atNumber, atMass, qBeta, nuclideLevels);
	}

    pugi::xml_node nuclide = doc.child("Nuclide");
    atNumber = nuclide.attribute("AtomicNumber").as_int();
	atMass = nuclide.attribute("AtomicMass").as_int();
    qBeta = nuclide.attribute("QBeta").as_double();
	
    for (pugi::xml_node level = nuclide.first_child(); level; level = level.next_sibling())
    {       
        std::vector<Gamma> gammasFromLvL;
        std::vector<Beta> betasFromLvL;
        std::vector<Neutron> neutronsFromLvL;
        std::vector<Alpha> alphasFromLvL;
        std::vector<Proton> protonsFromLvL;
        
        for (pugi::xml_node transition = level.first_child(); transition; transition = transition.next_sibling())
		{									
			string type = transition.attribute("Type").value();
			double transitionQval = transition.attribute("TransitionQValue").as_double();
            double intensity = transition.attribute("Intensity").as_double();
			double finalLvlEnergy = transition.child("TargetLevel").attribute("Energy").as_double();
			int finalLvlAtMass = transition.child("TargetLevel").attribute("AtomicMass").as_int();
			int finalLvlAtNumber = transition.child("TargetLevel").attribute("AtomicNumber").as_int();

			if(type == "EC")
			{
				Beta betaTransition(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, finalLvlAtNumber);
					
				for(pugi::xml_attribute attr: transition.child("ElectronCapture").attributes())
				{
					double value;
					istringstream( attr.value() ) >> value;
					//cout << attr.name() << " " << value << endl;
					betaTransition.SetECCoef(attr.name(), value);
				}
								
				betasFromLvL.push_back( betaTransition );
			}
			else if(type == "B-" || type == "B+")
			{
				betasFromLvL.push_back(Beta(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, finalLvlAtNumber));
			}
			else if(type == "G")
			{
				//additional check (if there is actual data, not only Total eCC, needed for example in 87Kr decay
				if( !transition.child("ElectronConversionCoefficient").empty() )
				// && ( (string)transition.child("ElectronConversionCoefficient").last_attribute().name() != "Total" ) )
				{			
					// eCC == ElectronConversionCoefficient
					double eCC = transition.child("ElectronConversionCoefficient").attribute("Total").as_double(); 
					Gamma gammaTransition(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, 
					finalLvlAtNumber, eCC, atNumber);
					
					for(pugi::xml_attribute attr: transition.child("ElectronConversionCoefficient").attributes())
					{
						double value;
						istringstream( attr.value() ) >> value;
						string typeName = attr.name();
						if( typeName == "Total" )
							continue;

						//cout << typeName << " " << value << endl;
						gammaTransition.SetShellElectronConvCoef(typeName, value);
					}
					
					if( (string)transition.child("ElectronConversionCoefficient").last_attribute().name() == "Total" )
					{
						gammaTransition.SetShellElectronConvCoef("KC", eCC);
						//cout << "KC" << " " << eCC << endl;
					}

					gammasFromLvL.push_back(gammaTransition);
				}
				else
				{
					gammasFromLvL.push_back(Gamma(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, finalLvlAtNumber));
				}
			}
			else if(type == "N")
			{
				neutronsFromLvL.push_back(Neutron(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, finalLvlAtNumber));
			}
			else if(type == "A")
			{
				alphasFromLvL.push_back(Alpha(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, finalLvlAtNumber));
			}	
			else if(type == "P")
			{
				protonsFromLvL.push_back(Proton(type, transitionQval, intensity, finalLvlEnergy, finalLvlAtMass, finalLvlAtNumber));
			}
			else
			{
				cout << "Unknown particle type: " << type << "! Data not registered." << endl;
			}		
		}
		double lvlEnergy = level.attribute("Energy").as_double();
		double lvlSpin = level.attribute("Spin").as_double();
		string lvlParity = level.attribute("Parity").value();
		double lvlHalfLifeTime = level.attribute("HalfLifeTime").as_double();
		string timeUnit = level.attribute("TimeUnit").value();
		double lvlHalfLifeTimeInSeconds = CalculateHalfLifeTimeInSeconds(lvlHalfLifeTime, timeUnit);
		
		nuclideLevels.push_back(Level(lvlEnergy, lvlSpin, lvlParity, lvlHalfLifeTimeInSeconds,
		 gammasFromLvL, betasFromLvL, neutronsFromLvL, alphasFromLvL, protonsFromLvL));

    }
    
    cout << "Nuclide data uploaded successfully." << endl;
	
    return Nuclide(atNumber, atMass, qBeta, nuclideLevels);
}

void LoadDecayData::SetPointersToTransitions()
{
	for ( auto it = allNuclides_.begin(); it != allNuclides_.end(); ++it )
	{
		for ( auto jt = it->GetNuclideLevels()->begin(); jt != it->GetNuclideLevels()->end(); ++jt )
		{
			std::vector<Transition*> pointersToTransitionsFromLvL;
			for ( auto kt = jt->GetBetaTransitions()->begin(); kt != jt->GetBetaTransitions()->end(); ++kt )
			{
				Transition* pointerToBeta = &(*kt);
				pointersToTransitionsFromLvL.push_back(pointerToBeta);
			}
			for ( auto kt = jt->GetGammaTransitions()->begin(); kt != jt->GetGammaTransitions()->end(); ++kt )
			{
				Transition* pointerToGamma = &(*kt);
				pointersToTransitionsFromLvL.push_back(pointerToGamma);
			}
			for ( auto kt = jt->GetNeutronTransitions()->begin(); kt != jt->GetNeutronTransitions()->end(); ++kt )
			{
				Transition* pointerToNeutron = &(*kt);
				pointersToTransitionsFromLvL.push_back(pointerToNeutron);
			}
			for ( auto kt = jt->GetAlphaTransitions()->begin(); kt != jt->GetAlphaTransitions()->end(); ++kt )
			{
				Transition* pointerToAlpha = &(*kt);
				pointersToTransitionsFromLvL.push_back(pointerToAlpha);
			}
			for ( auto kt = jt->GetProtonTransitions()->begin(); kt != jt->GetProtonTransitions()->end(); ++kt )
			{
				Transition* pointerToProton = &(*kt);
				pointersToTransitionsFromLvL.push_back(pointerToProton);
			}

			jt->SetTransitions(pointersToTransitionsFromLvL);
			jt->NormalizeTransitionIntensities();
			jt->CalculateTotalProbability();
		}
	}
}

void LoadDecayData::FindPointersToFinalLevels()
{
	for ( auto it = allNuclides_.begin(); it != allNuclides_.end(); ++it )
	{
		for ( auto jt = it->GetNuclideLevels()->begin(); jt != it->GetNuclideLevels()->end(); ++jt )
		{
			for ( auto kt = jt->GetTransitions()->begin(); kt != jt->GetTransitions()->end(); ++kt )
			{
                (*kt)->SetPointerToFinalLevel( FindPointerToLevel( (*kt)->GetFinalLevelAtomicNumber(), (*kt)->GetFinalLevelAtomicMass(), (*kt)->GetFinalLevelEnergy(), energyLevelUncertainty ) );
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
	cout << "Pointer to level " << energy << " in " << atomicMass << " " << atomicNumber <<
	" not found with default accuracy of " << energyLevelUncertainty << "keV." << endl;
	
		// additional security
	for ( auto it = allNuclides_.begin(); it != allNuclides_.end(); ++it )
	{
		int atNumber = it->GetAtomicNumber();
		int atMass = it->GetAtomicMass();
		if(atomicNumber == atNumber && atomicMass == atMass)								
			for ( int i = 1; i < 51; i++)
			{
				for ( auto jt = it->GetNuclideLevels()->begin(); jt != it->GetNuclideLevels()->end(); ++jt )
				{
					double temp = jt->GetLevelEnergy();
					if( ((temp - i) <= energy) && ((temp + i) >= energy) )
					{
                        cout << "Pointer to level " << energy << " found with accuracy of " << i << "keV." << endl;
						return &(*jt);
                    }
				}
			}
	}
	
	// throw Exception
	cout << "Level " << energy << " STILL not found after 50keV threshold!" << endl;
	//cout << atomicMass << " " << atomicNumber << " " << energy << endl;
}

void LoadDecayData::RecalculateIntensities( int atomicNumber, int atomicMass, double lvlEnergy,
string transitionType, double transitionEnergy)
{
	 
	for ( auto it = allNuclides_.begin(); it != allNuclides_.end(); ++it )
	{
		int atNumber = it->GetAtomicNumber();
		int atMass = it->GetAtomicMass();
		if(atomicNumber == atNumber && atomicMass == atMass)
			for ( auto jt = it->GetNuclideLevels()->begin(); jt != it->GetNuclideLevels()->end(); ++jt )
			{
				double tempLvlE = jt->GetLevelEnergy();
				if( ((tempLvlE - 0.1) <= lvlEnergy) && ((tempLvlE + 0.1) >= lvlEnergy) )
				{
					for ( auto kt = jt->GetTransitions()->begin(); kt != jt->GetTransitions()->end(); ++kt )
					{
						string tempType = (*kt)->GetParticleType();
						double tempE = (*kt)->GetTransitionQValue();
						
						if(tempType != transitionType)
							(*kt)->ChangeIntensity(0.);
							
						else if( !( ((tempE - 0.1) <= transitionEnergy) && ((tempE + 0.1) >= transitionEnergy) ) )
							(*kt)->ChangeIntensity(0.);
						else
						{
							(*kt)->ChangeIntensity(1.);
							cout << "Changing intensity to 1." << endl;
						}
					}
					jt->CalculateTotalProbability();
				}
			}
	}
}

void LoadDecayData::RecalculatePointersToLevels()
{
    FindPointersToFinalLevels();
}


