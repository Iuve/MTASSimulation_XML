#include "MTASAnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"


using namespace CLHEP;


MTASAnalysisManager::MTASAnalysisManager(std::string outputFile)
: m_RootOutputFile(0), m_NormalizeOutput(true)
{
	m_energyNTuple = 0;
	m_energy2NTuple = 0;
	m_lightNTuple1  = 0;
  m_lightNTuple2  = 0;
  m_siliNTuple1  = 0;
  m_siliNTuple2  = 0;
	SetUp();
	m_outputFile = outputFile;
}

// Destructor
MTASAnalysisManager::~MTASAnalysisManager()
{	
	MakeAndOpenOutputFile();

	if( m_RootOutputFile != 0 )
	{
		TObjArray histogramList;
		histogramList.Add( m_energyNTuple );
		histogramList.Add( m_energy2NTuple );	
		histogramList.Add( m_lightNTuple1 );
		histogramList.Add( m_lightNTuple2 );	
		histogramList.Add( m_siliNTuple1 );	
		histogramList.Add( m_siliNTuple2 );
		histogramList.Write();
	}

	CloseOutputFile();
  
	//CleanUp();
}

void MTASAnalysisManager::SetUp()
{
	CleanUp(); 
	m_temp = 0;
}


void MTASAnalysisManager::CleanUp()
{
	CloseOutputFile();

	if( m_RootOutputFile != 0 )
	{
	  delete m_RootOutputFile;
	}

	if( m_energyNTuple != 0 )
	{
		delete m_energyNTuple;
	}
	
	if( m_energy2NTuple != 0 )
	{
		delete m_energy2NTuple;
	}
	if( m_lightNTuple1 != 0 )
	{
		delete m_lightNTuple1;
	}
	
		if( m_lightNTuple2 != 0 )
	{
		delete m_lightNTuple2;
	}
	if( m_siliNTuple1 != 0 )
	{
		delete m_siliNTuple1;
	}
	if( m_siliNTuple2 != 0 )
	{
		delete m_siliNTuple2;
	}

}



void MTASAnalysisManager::MakeAndOpenOutputFile()
{
  m_RootOutputFile = new TFile( m_outputFile.c_str(),"recreate" );
}


void MTASAnalysisManager::CloseOutputFile()
{
	if( m_RootOutputFile != 0 )
	{
		m_RootOutputFile->Close();
	}
}

//Background
/*void MTASAnalysisManager::AddEnergyDepositedInCell( G4double startEnergy)
{
	if( m_pCellsHitNTuple == 0 )
	{
		m_pCellsHitNTuple = new TNtuple("BkgInfo","Cells Hit by Tin* Decay","startEnergy", 32000 );
	}

	m_pCellsHitNTuple->Fill(startEnergy/MeV );
	
}*/

//efficiency for all module
void MTASAnalysisManager::AddEnergyDepositedInCell(const int eventID ,
const G4double totEnDep, const G4double *enDepInAllModules)
{
	if( m_energyNTuple == 0 )
	{
		m_energyNTuple = new TNtuple("EnDepInModulesPart1","En dep in det",
		"eventID:TotalEnDep:enDepInMod0:enDepInMod1:enDepInMod2:enDepInMod3:enDepInMod4:enDepInMod5:enDepInMod6", 32000 );
	}

	m_energyNTuple->Fill(eventID, totEnDep/keV, *(enDepInAllModules)/keV, *(enDepInAllModules+1)/keV, 
	*(enDepInAllModules+2)/keV, *(enDepInAllModules+3)/keV,*(enDepInAllModules+4)/keV,
	*(enDepInAllModules+5)/keV,*(enDepInAllModules+6)/keV);
}
	
void MTASAnalysisManager::AddEnergyDepositedInCell2(const int eventID ,
const G4double totEnDep, const G4double *enDepInAllModules)
{
	if( m_energy2NTuple == 0 )
	{
		m_energy2NTuple = new TNtuple("EnDepInModulesPart2","En dep in det",
		"eventID:TotalEnDep:enDepInMod7:enDepInMod8:enDepInMod9:enDepInMod10:enDepInMod11:enDepInMod12:enDepInMod13:enDepInMod14:enDepInMod15:enDepInMod16:enDepInMod17:enDepInMod18", 32000 );
	}

	m_energy2NTuple->Fill(eventID, totEnDep/keV, *(enDepInAllModules+7)/keV,
	*(enDepInAllModules+8)/keV,*(enDepInAllModules+9)/keV,*(enDepInAllModules+10)/keV,
	*(enDepInAllModules+11)/keV,*(enDepInAllModules+12)/keV,*(enDepInAllModules+13)/keV,
	*(enDepInAllModules+14)/keV,*(enDepInAllModules+15)/keV,*(enDepInAllModules+16)/keV,
	*(enDepInAllModules+17)/keV,*(enDepInAllModules+18)/keV);	
}

void MTASAnalysisManager::AddLight1(const int eventID , const G4double totLight,
std::vector <G4double> lightProdInNaIMod)
{
	if( m_lightNTuple1 == 0 )
	{
		m_lightNTuple1 = new TNtuple("LightPart1","Light dep in det",
		"eventID:TotalLight:LightInMod0:LightInMod1:LightInMod2:LightInMod3:LightInMod4:LightInMod5:LightInMod6", 32000 );
	}

	m_lightNTuple1->Fill(eventID, totLight, lightProdInNaIMod.at(0), lightProdInNaIMod.at(1),
	lightProdInNaIMod.at(2), lightProdInNaIMod.at(3), lightProdInNaIMod.at(4),
	lightProdInNaIMod.at(5), lightProdInNaIMod.at(6));
}

void MTASAnalysisManager::AddLight2(const int eventID , const G4double totLight,
std::vector <G4double> lightProdInNaIMod)
{
	if( m_lightNTuple2 == 0 )
	{
		m_lightNTuple2 = new TNtuple("LightPart2","Light dep in det",
		"eventID:TotalLight:LightInMod7:LightInMod8:LightInMod9:LightInMod10:LightInMod11:LightInMod12:LightInMod13:LightInMod14:LightInMod15:LightInMod16:LightInMod17:LightInMod18",
		32000 );
	}

	m_lightNTuple2->Fill(eventID, totLight, lightProdInNaIMod.at(7), lightProdInNaIMod.at(8),
	lightProdInNaIMod.at(9), lightProdInNaIMod.at(10), lightProdInNaIMod.at(11),
	lightProdInNaIMod.at(12), lightProdInNaIMod.at(13), lightProdInNaIMod.at(14),
	lightProdInNaIMod.at(15), lightProdInNaIMod.at(16), lightProdInNaIMod.at(17), lightProdInNaIMod.at(18));
}

void MTASAnalysisManager::AddSiliconEnergy1(const int eventID , const G4double totSiliEn, const G4double *siliEn)
{
	if( m_siliNTuple1 == 0 )
	{
		m_siliNTuple1 = new TNtuple("EnDepInSili1","En dep in sili",
		"eventID:TotalSiliEn:enSili0:enSili1:enSili2:enSili3:enSili4:enSili5:enSili6", 32000 );
	}

	m_siliNTuple1->Fill(eventID, totSiliEn/keV, *(siliEn)/keV,*(siliEn+1)/keV,
	*(siliEn+2)/keV,*(siliEn+3)/keV,*(siliEn+4)/keV,*(siliEn+5)/keV,*(siliEn+6)/keV);	
}

void MTASAnalysisManager::AddSiliconEnergy2(const int eventID , const G4double totSiliEn, const G4double *siliEn)
{
	if( m_siliNTuple2 == 0 )
	{
		m_siliNTuple2 = new TNtuple("EnDepInSili2","En dep in sili",
		"eventID:TotalSiliEn:enSili7:enSili8:enSili9:enSili10:enSili11:enSili12:enSili13", 32000 );
	}

	m_siliNTuple2->Fill(eventID, totSiliEn/keV,*(siliEn+7)/keV,*(siliEn+8)/keV,
	*(siliEn+9)/keV,*(siliEn+10)/keV,*(siliEn+11)/keV,*(siliEn+12)/keV,*(siliEn+13)/keV);	
}
