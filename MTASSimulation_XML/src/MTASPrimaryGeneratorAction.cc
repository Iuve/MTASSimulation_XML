#include <fstream>
#include <stdio.h>
#include "globals.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

#include "MTASPrimaryGeneratorAction.hh"
#include "MTASPassiveShield.hh"
#include "MTASActiveShield.hh"
#include "Exception.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

void SetDecaySimulationStatus(bool status) {g_decaySimulationStatus = status;}

MTASPrimaryGeneratorAction::MTASPrimaryGeneratorAction( MTASDetectorConstruction* theDetectorConstruction)
{
	m_MTASDetectorConstruction = theDetectorConstruction;
	decay_ = 0L;
	if(g_decaySimulationStatus)
	{
	// kill a step/track in Geant4 - search for a solution to stop energy deposit
		try
		{
				decay_ = new Decay();
		}
		catch (Exception& except)
		{
			std::cout << "MTASPrimaryGeneratorAction exception: " << except.GetMessage() << std::endl;
			throw except;
		}
	}
	//LoadBackgroundFile();
	SetUp();
}

void MTASPrimaryGeneratorAction::SetUp( void )
{

  //Particle
  G4int n_particle = 1;
  m_ParticleGun = new G4ParticleGun( n_particle );

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* gammaPD = particleTable->FindParticle("gamma");
  //G4ParticleDefinition* neutronPD = particleTable->FindParticle("neutron");
  //G4ParticleDefinition* electronPD = particleTable->FindParticle("e-");


  //m_ParticleGun->SetParticleDefinition(neutronPD);
  m_ParticleGun->SetParticleDefinition(gammaPD);
  m_ParticleGun->SetParticleEnergy( 1000.0 * keV );
  m_ParticleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,0.*cm));
  m_ParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

}

MTASPrimaryGeneratorAction::~MTASPrimaryGeneratorAction()
{
	if(decay_)
		delete decay_;
  delete m_ParticleGun;
}



void MTASPrimaryGeneratorAction::GenerateSmoothCenterStartPosition(G4ThreeVector* startPos, G4double beamRadius,G4double beltWidth)
{
	//-BeamRadius < startX < BeamRadius
	G4double StartX=2*beamRadius*(G4UniformRand()-0.5);
	//startX^2+startY^2<=BeamRadius^2
	G4double StartY=2*sqrt(beamRadius*beamRadius- StartX * StartX)*(G4UniformRand()-0.5);
	G4double StartZ=beltWidth*G4UniformRand();
    *startPos = G4ThreeVector (StartX, StartY, StartZ);
}

void MTASPrimaryGeneratorAction::GenerateBackgroundStartPosition(G4ThreeVector* startPos)
{
	G4double ShieldX;
	G4double ShieldY;
	G4double ShieldZ;
	if(m_MTASDetectorConstruction->GetPassiveShield())
	{
		ShieldX = m_MTASDetectorConstruction->GetPassiveShield()->GetShieldx();
		ShieldY = m_MTASDetectorConstruction->GetPassiveShield()->GetShieldy();
		ShieldZ = m_MTASDetectorConstruction->GetPassiveShield()->GetShieldz();
    }
    else if(m_MTASDetectorConstruction->GetActiveShield())
    {
    	ShieldX = m_MTASDetectorConstruction->GetActiveShield()->GetShieldx();
		ShieldY = m_MTASDetectorConstruction->GetActiveShield()->GetShieldy();
		ShieldZ = m_MTASDetectorConstruction->GetActiveShield()->GetShieldz();
    }
    else
    {
        ShieldX = 0;
		ShieldX = 0;
		ShieldX = 0;
    }
	G4double HalfWorldLengthX=ShieldX/2.+ 2. * cm;
    G4double HalfWorldLengthY=ShieldY/2.+ 2. * cm;
    G4double HalfWorldLengthZ=ShieldZ/2.+ 2. * cm;
    
    G4double StartX=2*HalfWorldLengthX*(G4UniformRand()-0.5);
    G4double StartY=2*HalfWorldLengthY*(G4UniformRand()-0.5);
	G4double StartZ=2*HalfWorldLengthZ*(G4UniformRand()-0.5);
	while (StartX>-ShieldX/2. && StartX<ShieldX/2. && StartY>-ShieldY/2. && StartY<ShieldY/2. && StartZ>-ShieldZ/2. && StartZ<ShieldZ/2.)
	{
     	StartX=2*HalfWorldLengthX*(G4UniformRand()-0.5);
    	StartY=2*HalfWorldLengthY*(G4UniformRand()-0.5);
    	StartZ=2*HalfWorldLengthZ*(G4UniformRand()-0.5); 
	}  
	*startPos = G4ThreeVector (StartX, StartY, StartZ); 
}

void  MTASPrimaryGeneratorAction::GenerateIsotropicDirectionDistribution(G4ThreeVector* direction, G4double theta0)
{
	// -cos(Theta0) < cosTheta < cos(Theta0)
	G4double cosTheta = ( G4UniformRand() - 0.5 ) * 2.0*cos(theta0);
	G4double sinTheta = sqrt( 1.0 - cosTheta * cosTheta );
	// 0. < phi < 2*pi
	G4double phi = G4UniformRand() * 8.0 * atan(1.0);
	*direction = G4ThreeVector (cos(phi) *sinTheta, sin(phi) * sinTheta, cosTheta);
}

void  MTASPrimaryGeneratorAction::GenerateBackgroundDirectionDistribution(G4ThreeVector* direction, G4ThreeVector startPos)
{
	G4double theta0=0.0;
	G4double cosTheta = ( G4UniformRand() - 0.5 ) * 2.0*cos(theta0);// -cos(Theta0) < cosTheta < cos(Theta0)
	G4double sinTheta = sqrt( 1.0 - cosTheta * cosTheta );
	G4double phi = G4UniformRand() * 8.0 * atan(1.0);// 0. < phi < 2*pi

	G4double randomXaim = cos(phi) * sinTheta;
	G4double randomYaim = sin(phi) * sinTheta;
	G4double randomZaim = cosTheta;


    if(randomXaim*startPos[0]>0)
		randomXaim=-randomXaim;
    if(randomYaim*startPos[1]>0)
		randomYaim=-randomYaim;
    if(randomZaim*startPos[2]>0)
		randomZaim=-randomZaim;
		
	*direction = G4ThreeVector (randomXaim, randomYaim, randomZaim);
}

/*void MTASPrimaryGeneratorAction::LoadBackgroundFile()
{
	ifstream BackgroundFile;
	BackgroundFile.open("tlo.txt");


	const G4int NoOfEleInFile=7543;
	m_NoOfEleInFile=7543;
	G4double BcgEnergy;
	G4double BcgCount;
	G4int MaxEnergy = 0;
	G4int MaxCount = 0;

	for (int i=0; i<NoOfEleInFile; i++)
	{
		BackgroundFile >> BcgEnergy;
		m_BcgEnergy.push_back(BcgEnergy);
		BackgroundFile >> BcgCount;
		m_BcgCount.push_back(BcgCount);
		if (BcgCount>MaxCount) //I'm looking for the maximum count
			{
			MaxCount=BcgCount;
			MaxEnergy=BcgEnergy;
			}
		
	}
	
	BackgroundFile.close();
	m_BcgEnergy.push_back(MaxEnergy);	
}*/

/*G4double MTASPrimaryGeneratorAction::GenerateBackgroundEnergyDistribution()
{


	int EnIndex;
	EnIndex=(int)(G4UniformRand()*m_NoOfEleInFile);
	while (m_BcgCount [EnIndex]<G4UniformRand()*m_BcgEnergy[m_NoOfEleInFile+1])
		EnIndex=(int)(G4UniformRand()*m_NoOfEleInFile);

	return m_BcgEnergy [EnIndex] * keV;
  
} */



void MTASPrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent )
{
	if(g_decaySimulationStatus)
		GenerateDecay(anEvent);
	else
		GenerateSingleParticle(anEvent);
		
}

void MTASPrimaryGeneratorAction::GenerateDecay( G4Event* anEvent )
{
		//Position
	//G4ThreeVector startPos( 17.6*cm, 8.8*cm, 0.0*cm );
	G4ThreeVector startPos( 0.0*cm, 0.0*cm, 0.0*cm );
	
	//double randX = 80*G4UniformRand() - 40;
	//double randY = 80*G4UniformRand() - 40;
	//double randZ = 50*G4UniformRand() - 25;
	
	//G4ThreeVector startPos( randX*cm, randY*mm, randZ*cm );
	
	//Direction
	G4ThreeVector direction( 1.0, 0.0, 0.0 );
	GenerateIsotropicDirectionDistribution(&direction,0.0);
	std::vector<Event> allDecay;
	try{
		allDecay = decay_ -> GenerateEventList();
	}
	catch (Exception& except)
	{
		std::cout << "MTASPrimaryGeneratorAction exception: " << except.GetMessage() << std::endl;
		throw except;
	}
	
	
	for(unsigned int i=0; i<allDecay.size();i++)
	{
			m_ParticleGun->SetParticlePosition( startPos );
			GenerateIsotropicDirectionDistribution(&direction,0.0);
			m_ParticleGun->SetParticleMomentumDirection( direction );
			m_ParticleGun->SetParticleEnergy(allDecay.at(i).energy *keV);
			m_ParticleGun->SetParticleDefinition(allDecay.at(i).type);
			m_ParticleGun->GeneratePrimaryVertex(anEvent);
	}
}

void MTASPrimaryGeneratorAction::GenerateSingleParticle( G4Event* anEvent )
{

	//Position
	//G4ThreeVector startPos( 17.6*cm, 8.8*cm, 0.0*cm );
	G4ThreeVector startPos( 0.0*cm, 0.0*cm, 0.0*cm );
	
	//Direction
	G4ThreeVector direction( 1.0, 0.0, 0.0 );
	GenerateIsotropicDirectionDistribution(&direction,0.0);
	
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	//G4ParticleDefinition* electronPD = particleTable->FindParticle("e-");
	//m_ParticleGun->SetParticleDefinition(electronPD);
	//m_ParticleGun->SetParticleEnergy(500*keV);
	
	m_ParticleGun->SetParticlePosition( startPos );
	m_ParticleGun->SetParticleMomentumDirection( direction );
	m_ParticleGun->GeneratePrimaryVertex(anEvent);

	
		/*for(unsigned int i=0; i<5;i++)
	{
			m_ParticleGun->SetParticlePosition( startPos );
			GenerateIsotropicDirectionDistribution(&direction,0.0);
			m_ParticleGun->SetParticleMomentumDirection( direction );
			m_ParticleGun->SetParticleEnergy(200*keV);
			m_ParticleGun->SetParticleDefinition(electronPD);
			m_ParticleGun->GeneratePrimaryVertex(anEvent);
			//std::cout<<m_ParticleGun->GetParticleEnergy()<<" "<<m_ParticleGun->GetParticleDefinition()<<std::endl;
	}*/


}
