/**
* Class constructing elements of central detector
*/

#include <math.h> 
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4Polyhedra.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4VisAttributes.hh"


#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSNofSecondary.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "MTASLadder.hh"
using namespace CLHEP;
  
MTASLadder::MTASLadder(G4LogicalVolume* logicWorld)
{
	m_logicWorld=logicWorld;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
	SetParameters();
	Construct();
	SetVisibleAtributes();
}

MTASLadder::~MTASLadder()
{

}

void MTASLadder::SetParameters()
{ 
	
	M_Pi = 4*atan(1);
	m_inch = 2.54*cm;
	a = 0.125 * m_inch;
	b = 0.202 * m_inch;
	c = 0.196 * m_inch;
	A = 0.5 * m_inch;
	B = 0.04 * m_inch;
	C = 0.075 * m_inch;
	length = 65.250 * m_inch;
	//ladderXPosition = 0.425 *m_inch;
	ladderXPosition = 0.496 *m_inch;
	//notchA = 0.06 *m_inch;
		
	notchA = 0.44 *m_inch;
	notchLength = 3.13 *m_inch;
	notchZPosition = 30 *m_inch;
	
	pipeOuterRadius = 2.5/2.*m_inch;
	//pipeThickness = 0.04 *m_inch;//was 0.03
	pipeThickness = 0.02 * m_inch;
	pipeLength = 60 *m_inch;
}

void MTASLadder::Construct()
{
	G4ThreeVector position( 0., 0., 0. );
	
	//alu pipe
	G4Material* pipeMaterial= m_materialsAndColorsManager->GetAluminium();
	G4Tubs *pipeSolid = new G4Tubs("pipeSolid", pipeOuterRadius-pipeThickness,pipeOuterRadius, pipeLength/2., 0., 360.);
	pipeLogVol = new G4LogicalVolume(pipeSolid,pipeMaterial,"pipeLogVol");
	new G4PVPlacement( 0, position, pipeLogVol, "pipePhysical", m_logicWorld, 0, 0 );
	
	//Aluminium backbone
	G4Material* backboneMaterial= m_materialsAndColorsManager->GetAluminium();
	
	G4Box* backboneSolid1 = new G4Box("backboneSolid1", C/2., c/2., length/2.);
	G4Box* backboneSolid2 = new G4Box("backboneSolid2", A/2., a/2., length/2.);
	G4Trap* backboneSolid3 = new G4Trap ("backboneSolid3",length, b, C,B);
	
	G4RotationMatrix* rot = new G4RotationMatrix; 
	rot->rotateY(0.*rad); //no rotation
	G4ThreeVector trans(-C/2.-A/2., 0, 0);
	G4UnionSolid * backboneSolid12 = new G4UnionSolid ("backboneSolid12", backboneSolid1, backboneSolid2, rot, trans);
	
	trans = G4ThreeVector((B-C)/4., c/2.+b/2., 0.);
	G4UnionSolid * backboneSolid123 = new G4UnionSolid ("backboneSolid123", backboneSolid12, backboneSolid3, rot, trans);
	
	trans = G4ThreeVector((B-C)/4., -c/2.-b/2., 0.);
	G4RotationMatrix* xRot = new G4RotationMatrix; 
	xRot->rotateX(pi*rad);
	G4UnionSolid * backboneSolid1234 = new G4UnionSolid ("backboneSolid1234", backboneSolid123, backboneSolid3, xRot, trans);
	
	//cat part from centre
	G4Box* notchSolid = new G4Box("notchSolid", notchA/2., a/2., notchLength/2.);
	trans = G4ThreeVector(-C/2.-A+notchA/2., 0., length/2. - notchZPosition );
	G4SubtractionSolid *backboneSolid = new G4SubtractionSolid("backboneSolid", backboneSolid1234, notchSolid, rot, trans);
	
	backboneLogVol = new G4LogicalVolume( backboneSolid, backboneMaterial, "backboneLogVol");
	position = G4ThreeVector(ladderXPosition+A+C/2., 0., -length/2. + notchZPosition);
	new G4PVPlacement( 0, position, backboneLogVol, "backbonePhysical", m_logicWorld, 0, 0 );
	
	position = G4ThreeVector(-ladderXPosition-A-C/2., 0., -length/2. + notchZPosition);
	G4RotationMatrix* zRot = new G4RotationMatrix; 
	zRot->rotateZ(pi*rad); //no rotation
	new G4PVPlacement( zRot, position, backboneLogVol, "backbonePhysical", m_logicWorld, 0, 1 );
	

/**cable connector */
	
	//Al layer:
	G4double aluX = 2.15 *cm;
	G4double aluXX = 1.275 *cm;
	G4double aluY = 2.7 *cm;
	G4double aluZ = 2 *mm;//1.0 mm tests 26 XI 2012
	
	const G4ThreeVector zero(0., 0., 0.);
	//G4Material *aluminium = m_materialsAndColorsManager->GetSteel();
	G4Material *aluminium = m_materialsAndColorsManager->GetPolyethylene();
	G4Trap *aluCableLayerSolid = new G4Trap("aluCableLayerSolid", aluZ, aluY, aluX, aluXX);
	G4LogicalVolume *aluCableLayerLogic = new G4LogicalVolume(aluCableLayerSolid, aluminium, "aluCableLayerLogic");
	
	G4double polyX = 0.9*aluX;//1.75 *cm;
	G4double polyXX = 0.9*aluXX;//0.875 *cm;
	G4double polyY = 0.9*aluY;//2.6 *cm;
	G4double polyZ = 0.9*aluZ;//*aluZ;
	G4Material *polyethylen = m_materialsAndColorsManager->GetPolyethylene();	
	G4Trap *polyCableLayerSolid = new G4Trap("polyCableLayerSolid", polyZ, polyY, polyX, polyXX);
	G4LogicalVolume *polyCableLayerLogic = new G4LogicalVolume(polyCableLayerSolid, polyethylen, "polyCableLayerLogic");
	new G4PVPlacement( 0, zero, polyCableLayerLogic, "polyCableLayerPhysical", aluCableLayerLogic, 0, 0 );

	G4double cuX = 0.9*polyX;//1.05 *cm;
	G4double cuXX = 0.9*polyXX;//0.475 *cm;
	G4double cuY = 0.9*polyY;//2.5 *cm;
	G4double cuZ = 0.3*polyZ;//0.15 *cm;
	G4Material *copper = m_materialsAndColorsManager->GetCopper();	
	G4Trap *cuCableLayerSolid = new G4Trap("cuCableLayerSolid", cuZ, cuY, cuX, cuXX);
	G4LogicalVolume *cuCableLayerLogic = new G4LogicalVolume(cuCableLayerSolid, copper, "cuCableLayerLogic");
	new G4PVPlacement( 0, zero, cuCableLayerLogic, "cuCableLayerPhysical", polyCableLayerLogic, 0, 0 );
	
				
	position = G4ThreeVector(-1*cm, 0.5*cm , 0.85*cm);
	G4RotationMatrix*	rotationMatrix = new G4RotationMatrix();
	rotationMatrix->rotateZ(pi/2.*rad);
	rotationMatrix->rotateY(pi/2.*rad);
	
	position = G4ThreeVector(-1*cm, 0.5*cm , -0.85*cm);
	G4RotationMatrix*	rotationMatrix2 = new G4RotationMatrix();
	rotationMatrix2->rotateX(pi*rad);
	rotationMatrix2->rotateY(pi/2.*rad);
	rotationMatrix2->rotateX(pi/2.*rad);
	
	position = G4ThreeVector(1*cm, -0.5*cm , 0.85*cm);
	G4RotationMatrix*	rotationMatrix3 = new G4RotationMatrix();
	rotationMatrix3->rotateZ(pi*3/2.*rad);
	rotationMatrix3->rotateY(pi/2.*rad);

	position = G4ThreeVector(1*cm, -0.5*cm , -0.85*cm);
	G4RotationMatrix*	rotationMatrix4 = new G4RotationMatrix();
	rotationMatrix4->rotateZ(pi*rad);
	rotationMatrix4->rotateX(pi*rad);
	rotationMatrix4->rotateY(pi/2.*rad);
	rotationMatrix4->rotateX(pi/2.*rad);

	
}

void MTASLadder::SetVisibleAtributes()
{
	backboneLogVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());
	//backboneLogVol2->SetVisAttributes(m_materialsAndColorsManager->GetGrey());
	/*backboneLogVol3->SetVisAttributes(m_materialsAndColorsManager->GetGrey());*/
	pipeLogVol->SetVisAttributes(m_materialsAndColorsManager->GetYellow());
}


