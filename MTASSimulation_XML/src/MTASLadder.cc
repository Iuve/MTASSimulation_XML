/**
* Class constructing elements of central detector
*/

#include <math.h> 
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4Torus.hh"
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
	
	
	//Test cables (MS October 2022)
	G4Material* cableMaterial= m_materialsAndColorsManager->GetCableTest();
	G4Tubs *cableSolid = new G4Tubs("cableSolid", 0.*cm,0.5*cm, pipeLength/2., 0., 360.);
	cableLogVol = new G4LogicalVolume(cableSolid,cableMaterial,"cableLogVol");
	position = G4ThreeVector(ladderXPosition+A+C/2. - 0.75*cm, -1.0*cm, -length/2. + notchZPosition);
	//new G4PVPlacement( 0, position, cableLogVol, "cablePhysical", m_logicWorld, 0, 0 );
	
	position = G4ThreeVector(-ladderXPosition-A-C/2. + 0.75*cm, -1.0*cm, -length/2. + notchZPosition);
	//new G4PVPlacement( 0, position, cableLogVol, "cablePhysical", m_logicWorld, 0, 0 );
	/*
        G4Box *squareCableSolid = new G4Box("squareCableSolid", 2.0*cm, 1.0*mm, 2.5*cm);
        squareCableLogVol = new G4LogicalVolume(squareCableSolid,cableMaterial,"squareCableLogVol");
        position = G4ThreeVector(0., 5.0*mm, 0.);
        G4RotationMatrix*	rotationMatrixArray = new G4RotationMatrix();
        rotationMatrixArray->rotateY(CLHEP::pi*rad);
        new G4PVPlacement( rotationMatrixArray, position, squareCableLogVol, "squareCablePhysical", m_logicWorld, 0, 0 );
    */
    
    //Aluminium blob/vacuum from Charlie, November 2022
    
// Top trapazoid, or G4Trd() if you will. Unrotated coordiantes are here. will rotate by 90 degrees around Y axis.
        const G4double wireOffsetY = 9.0 * mm;
        const G4double dx0_1 = 38.6 * mm;
        const G4double dx0_2 = 10.0 * mm;
        const G4double dy0_1 = 1.0 * mm;
        const G4double dy0_2 = 10.0 * mm;
        const G4double z0 = 22.0 * mm;
    // Cut tub to connect cylinder and flat part
        const G4double rMin = 0.0*mm;
        const G4double rMax = 0.4 * 10.0 * mm;
        const G4double halfz1 = 0.5 * 4.0 * mm;
        const G4double Rtorus = 15.0 * mm;
        const G4double zHalfLength = 50.0 * mm;
//        const G4ThreeVector normTop(-0.707,0.0,-0.707);
//        const G4ThreeVector normBot(0.0,0.0,1.0);
        const G4ThreeVector deltaw_0_1(0.0,-0.4*mm,0.5*z0+halfz1 - 1.8 * mm);
        const G4double  startPhi = 0.0 * degree;
		const G4double  deltaPhi = 360.0 * degree;
		
		G4Material* wire_Shield_Material= m_materialsAndColorsManager->GetLowDensityAluminium();

        G4Trd* wire0_solid = new G4Trd( "wire_0" , 0.5*dx0_1, 0.5*dx0_2, 0.5*dy0_1, 0.5*dy0_2, 0.5*z0 );
//        G4CutTubs* wire1_solid = new G4CutTubs( "wire_1" , rMin, rMax, halfz1, startPhi, deltaPhi, normTop, normBot );
        G4Tubs* wire1_solid = new G4Tubs( "wire_1" , rMin, rMax, halfz1, startPhi, deltaPhi );

        G4RotationMatrix* rot_w01 = new G4RotationMatrix();//(matrixColumn1, matrixColumn2, matrixColumn3);
        rot_w01->rotateX( -15.0 * degree );
        //m_Rotations.push_back(rot_w01);

//        G4UnionSolid* ladderUnion1_solid = new G4UnionSolid ("ladderUnion1_solid", laddreInAir1_Solid, ladderFlat1_Solid, rotY0, deltaPos);

        G4UnionSolid *wire_0_1_solid = new G4UnionSolid("wire_0_1", wire0_solid, wire1_solid, rot_w01, deltaw_0_1 );

        G4LogicalVolume* wireTop_logVol = new G4LogicalVolume( wire_0_1_solid, wire_Shield_Material, "WireTop_Logical");
        
        //m_Wire_LogicalVolumes.push_back( wireTop_logVol );
        
        G4RotationMatrix* rot_w0 = new G4RotationMatrix();//(matrixColumn1, matrixColumn2, matrixColumn3);
        rot_w0->rotateY( 270.0 * degree );
        //m_Rotations.push_back(rot_w0);

        G4RotationMatrix* rot_w0_1 = new G4RotationMatrix();//(matrixColumn1, matrixColumn2, matrixColumn3);
        rot_w0_1->rotateX( 15.0 * degree );
        //m_Rotations.push_back(rot_w0_1);

        G4RotationMatrix* rot_w02 = new G4RotationMatrix();//(matrixColumn1, matrixColumn2, matrixColumn3);
        (*rot_w02) = (*rot_w0_1) * (*rot_w0);
        //m_Rotations.push_back(rot_w02);
        
        position = G4ThreeVector( - 0.5 * z0 - 1.2 * mm, wireOffsetY, 0.0*mm );
	new G4PVPlacement( rot_w02, position,  wireTop_logVol, "Wire0_Physical", m_logicWorld, 0, 0 );
       
        G4Torus* wire2_solid = new G4Torus( "wire_2" , rMin, rMax, Rtorus, 270.0 * degree, 90.0 * degree );
        G4LogicalVolume* wireTop1_logVol = new G4LogicalVolume( wire2_solid, wire_Shield_Material, "WireTop1_Logical");
        
        //m_Wire_LogicalVolumes.push_back( wireTop1_logVol );
        
        G4RotationMatrix* rot_w2 = new G4RotationMatrix();//(matrixColumn1, matrixColumn2, matrixColumn3);
        rot_w2->rotateX( 90.0 * degree );
        //m_Rotations.push_back(rot_w2);
        position = G4ThreeVector( 0.8*mm, ( 7.5*mm  + rMax ) , -15.0*mm );
	new G4PVPlacement( rot_w2, position,  wireTop1_logVol, "Wire2_Physical", m_logicWorld, 0, 0 );
        
        G4Tubs* wire3_solid = new G4Tubs( "wire_3" , rMin, rMax, zHalfLength, startPhi, deltaPhi );
        G4LogicalVolume* wireTop3_logVol = new G4LogicalVolume( wire3_solid, wire_Shield_Material, "WireTop3_Logical");
        
        //m_Wire_LogicalVolumes.push_back( wireTop3_logVol );

        position += G4ThreeVector( Rtorus, 0.0 , -zHalfLength );
	new G4PVPlacement( 0, position,  wireTop3_logVol, "Wire3_Physical", m_logicWorld, 0, 0 );

        position[1] = -position[1];
        position[2] += 2.0 * Rtorus;
	new G4PVPlacement( 0, position,  wireTop3_logVol, "Wire3_Physical", m_logicWorld, 0, 0 );
	
	

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


