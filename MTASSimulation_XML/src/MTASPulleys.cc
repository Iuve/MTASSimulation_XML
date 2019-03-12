/**
* Class constructing elements of central detector
*/

#include <math.h> 
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4Polyhedra.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"

#include "G4VisAttributes.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "MTASPulleys.hh"
using namespace CLHEP;
  
MTASPulleys::MTASPulleys(G4LogicalVolume* logicWorld)
{
	m_logicWorld=logicWorld;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
	SetParameters();
	Construct();
	SetVisibleAtributes();
}

MTASPulleys::~MTASPulleys()
{

}

void MTASPulleys::SetParameters()
{ 
	
	M_Pi = 4*atan(1);
	m_inch = 2.54*cm;

}

void MTASPulleys::Construct()
{
	G4ThreeVector position(0., 0., 0.);
	G4RotationMatrix* yRot = new G4RotationMatrix; 
	yRot->rotateY(0. *rad); 
	G4ThreeVector zTrans(0., 0., 0);
	
	//mother volume for A pulleys
	G4double pulleyAX = 0.725*m_inch;
	G4double pulleyAY = 0.450*m_inch;
	G4double pulleyAZ = 1.985*m_inch;
	G4double pulleyALeftPartX = 0.15*m_inch;
	G4double pulleyAxleZ = 1.3*m_inch;
	G4double pulleyAxleFi = 0.125*m_inch;
	G4double pulleyAxlePositionX = 0.385*m_inch;
	G4Material* pulleyAMotherMat = m_materialsAndColorsManager->GetVaccum();
	G4Box* pulleyAMotherSolid1_1 =new G4Box("pulleyAMotherSolid1_1", pulleyAX/2., pulleyAY/2., pulleyAZ/2.);
	G4Box* pulleyAMotherSolid1_2 =new G4Box("pulleyAMotherSolid1_2", (pulleyAX-pulleyALeftPartX)/2., pulleyAY/2.,(pulleyAZ-pulleyAxleZ)/4.);
	
	zTrans = G4ThreeVector(pulleyALeftPartX/2. , 0.,(pulleyAZ +pulleyAxleZ)/4.);
	G4SubtractionSolid *pulleyAMotherSolid2 = new G4SubtractionSolid("pulleyAMotherSolid2", pulleyAMotherSolid1_1, pulleyAMotherSolid1_2, yRot, zTrans);
	zTrans = G4ThreeVector(pulleyALeftPartX/2. , 0.,-(pulleyAZ +pulleyAxleZ)/4.);
	G4SubtractionSolid *pulleyAMotherSolid3 = new G4SubtractionSolid("pulleyAMotherSolid3", pulleyAMotherSolid2, pulleyAMotherSolid1_2, yRot, zTrans);
	pulleyAMotherLogVol = new G4LogicalVolume(pulleyAMotherSolid3,pulleyAMotherMat, "pulleyAMotherLogVol");
 //new G4PVPlacement( 0, position, pulleyAMotherLogVol, "pulleyAMotherPhysical", m_logicWorld, 0, 0 );
	
	//Axle
	G4double rollerFi1 = 0.450*m_inch;
	G4double rollerZ1 = 0.75 * m_inch;
	G4double rollerFi2 = 0.375*m_inch;
	G4double rollerZ2 = 0.6*m_inch;
	G4double rollerHoleFi = 0.188*m_inch;
	//G4double rollerHoleZ = rollerZ1;
	
	G4Material* axleMaterial= m_materialsAndColorsManager->GetAluminium();
	G4Tubs *axleSolid1 = new G4Tubs("axleSolid1", 0., pulleyAxleFi/2., pulleyAxleZ/2.,  0., 360.);
	G4LogicalVolume* axleLogVol = new G4LogicalVolume(axleSolid1,axleMaterial,"axleLogVol");
	position = G4ThreeVector(pulleyAxlePositionX-0.5*pulleyAX, 0., 0.);
	new G4PVPlacement( 0, position, axleLogVol, "axlePhysical", pulleyAMotherLogVol, 0, 0 );
	
	//Roller 
	G4Material* rollerMaterial= m_materialsAndColorsManager->GetAluminium();
	G4Tubs *rollerSolid1 = new G4Tubs("rollerSolid1", rollerHoleFi/2., rollerFi1/2., rollerZ1/2.,  0., 360.);
	G4Tubs *rollerSolid2 = new G4Tubs("rollerSolid2", rollerFi2/2., rollerFi1/2., rollerZ2/2.,  0., 360.);
	zTrans = G4ThreeVector(0., 0.,0.);
	G4SubtractionSolid *rollerSolid= new G4SubtractionSolid("rollerSolid1", rollerSolid1, rollerSolid2, yRot, zTrans);
	G4LogicalVolume* rollerLogVol = new G4LogicalVolume(rollerSolid,rollerMaterial,"rollerLogVol");
	position = G4ThreeVector(pulleyAxlePositionX-0.5*pulleyAX, 0., 0.);
	new G4PVPlacement( 0, position, rollerLogVol, "rollerPhysical", pulleyAMotherLogVol, 0, 0 );
	


//Type A	
	//element1
	G4double elem1X = 0.15*m_inch;
	G4double elem1Y = pulleyAY;
	G4double elem1Z = 0.593 *m_inch; 
	G4Box *elem1Solid = new G4Box("elem1Solid", elem1X/2., elem1Y/2., elem1Z/2.);
	G4LogicalVolume* elem1LogVol = new G4LogicalVolume (elem1Solid, rollerMaterial, "elem1LogVol");
	position = G4ThreeVector(elem1X/2. - pulleyAX/2., 0., pulleyAZ/2. - elem1Z/2.);
	new G4PVPlacement( 0, position, elem1LogVol, "elem1Physical", pulleyAMotherLogVol, 0, 0 );
	position = G4ThreeVector(elem1X/2. - pulleyAX/2., 0., -pulleyAZ/2. + elem1Z/2.);
	new G4PVPlacement( 0, position, elem1LogVol, "elem1Physical", pulleyAMotherLogVol, 0, 1 );
	
	//element2
	G4double elem2X = 0.1*m_inch;
	G4double elem2Y = pulleyAY;
	G4double elem2Z = 0.8 *m_inch; 
	G4Box *elem2Solid = new G4Box("elem2Solid", elem2X/2., elem2Y/2., elem2Z/2.);
	G4LogicalVolume* elem2LogVol = new G4LogicalVolume (elem2Solid, rollerMaterial, "elem2LogVol");
	position = G4ThreeVector(elem2X/2. - pulleyAX/2., 0., 0);
	new G4PVPlacement( 0, position, elem2LogVol, "elem2Physical", pulleyAMotherLogVol, 0, 0 );
	
	//elem2a
	G4double elem2Xa = elem2X;
	G4double elem2Ya = elem2Y;
	G4double elem2Za = 1.05 *m_inch; 
	G4Box *elem2aSolid = new G4Box("elem2aSolid", elem2Xa/2., elem2Ya/2., elem2Za/2.);
	G4LogicalVolume* elem2aLogVol = new G4LogicalVolume (elem2aSolid, rollerMaterial, "elem2aLogVol");
	position = G4ThreeVector(-elem2Xa/2. + pulleyAX/2., 0., 0.);
	new G4PVPlacement( 0, position, elem2aLogVol, "elem2aPhysical", pulleyAMotherLogVol, 0, 0);
	
	//element3
	G4double elem3X = 0.475*m_inch;
	G4double elem3Y = pulleyAY;
	G4double elem3Z = 0.125 *m_inch; 
	G4Box *elem3Solid1 = new G4Box("elem3Solid1", elem3X/2., elem3Y/2., elem3Z/2.);
	G4Tubs *elem3Solid2 = new G4Tubs("elem3Solid2", 0., pulleyAxleFi, elem3Z/2., 0., 360.);
	zTrans = G4ThreeVector(0., 0.,0.);
	G4SubtractionSolid *elem3Solid= new G4SubtractionSolid("elem3Solid", elem3Solid1, elem3Solid2, yRot, zTrans);	
	
	
	G4LogicalVolume* elem3LogVol = new G4LogicalVolume (elem3Solid, rollerMaterial, "elem3LogVol");
	position = G4ThreeVector(pulleyAxlePositionX-0.5*pulleyAX, 0., elem2Z/2. + elem3Z/2.);
	new G4PVPlacement( 0, position, elem3LogVol, "elem3Physical", pulleyAMotherLogVol, 0, 0 );
	position = G4ThreeVector(pulleyAxlePositionX-0.5*pulleyAX, 0., -elem2Z/2. - elem3Z/2.);
	new G4PVPlacement( 0, position, elem3LogVol, "elem3Physical", pulleyAMotherLogVol, 0, 1 );

//Type B
	G4double shortPulleyAxleZ = rollerZ1;
	G4double pulleyBX1 = 0.55*m_inch;
	G4double pulleyBY1 = rollerFi1;
	G4double pulleyBZ1 = shortPulleyAxleZ;
	G4double pulleyBX2 = 0.375*m_inch;
	G4double pulleyBY2 = 0.250*m_inch;//0.250??
	G4double pulleyBZ2 = 1.985*m_inch;
	G4double pulleyBX3 = 0.55*m_inch;
	G4double pulleyBY3 = 0.250*m_inch;//0.250??
	G4double pulleyBZ3 = pulleyAxleZ;
	G4Material* pulleyBMotherMat = m_materialsAndColorsManager->GetVaccum();
	G4Box* pulleyBMotherSolid1 =new G4Box("pulleyBMotherSolid1", pulleyBX1/2., pulleyBY1/2., pulleyBZ1/2.);
	G4Box* pulleyBMotherSolid2 =new G4Box("pulleyBMotherSolid2", pulleyBX2/2., pulleyBY2/2.,pulleyBZ2/2.);
	G4Box* pulleyBMotherSolid3 =new G4Box("pulleyBMotherSolid3", pulleyBX3/2., pulleyBY3/2.,pulleyBZ3/2.);
	zTrans = G4ThreeVector(-(pulleyBX1+pulleyBX2)/2., 0.,0.);
	G4UnionSolid *pulleyBMotherSolid12 = new G4UnionSolid("pulleyBMotherSolid", pulleyBMotherSolid1, pulleyBMotherSolid2, yRot, zTrans);
	zTrans = G4ThreeVector(0., 0.,0.);
	G4UnionSolid *pulleyBMotherSolid = new G4UnionSolid("pulleyBMotherSolid", pulleyBMotherSolid12, pulleyBMotherSolid3, yRot, zTrans);
	pulleyBMotherLogVol = new G4LogicalVolume(pulleyBMotherSolid, pulleyBMotherMat, "pulleyBMotherLogVol");
	//new G4PVPlacement(0, position, pulleyBMotherLogVol, "pulleyBMotherPhysical", m_logicWorld, 0, 0 );
	
	//Axle
	position = G4ThreeVector(0., 0., 0.);
	//G4Tubs *shortAxleSolid = new G4Tubs("shortAxleSolid", 0., pulleyAxleFi/2., shortPulleyAxleZ/2.,  0., 360.);
	//G4LogicalVolume* shortAxleLogVol = new G4LogicalVolume(shortAxleSolid,axleMaterial,"shortAxleLogVol");
	new G4PVPlacement( 0, position, axleLogVol, "axleBPhysical", pulleyBMotherLogVol, 0, 0 );
	//Rollers
	new G4PVPlacement( 0, position, rollerLogVol, "rollerBPhysical", pulleyBMotherLogVol, 0, 0 );

		
	//element1B
	G4double element1BX = pulleyBX1 + pulleyBX2;
	G4double element1BY = pulleyBY2;
	G4double element1BZ = 0.125*m_inch;
	G4double element1BPosZ = 0.462*m_inch;
	G4double element1BHolePosX = 0.65*m_inch - element1BX /2.;
	G4Box* elemBSolid1_1 = new G4Box("elemBSolid1_1", element1BX/2., element1BY/2., element1BZ/2.);
	G4Tubs *elemBSolid1_2 = new G4Tubs("elemBSolid1_2", 0., pulleyAxleFi/2.,element1BZ/2.,  0., 360.);
	zTrans = G4ThreeVector(element1BHolePosX, 0.,0.);
	G4SubtractionSolid *elem1BSolid= new G4SubtractionSolid("elem1BSolid", elemBSolid1_1, elemBSolid1_2, yRot, zTrans);	
G4LogicalVolume*	elem1BLogVol = new G4LogicalVolume(elem1BSolid, rollerMaterial, "elem1BLogVol");
	position = G4ThreeVector(-pulleyBX2/2., 0., element1BPosZ);
	new G4PVPlacement( 0, position, elem1BLogVol, "elemB1Physical", pulleyBMotherLogVol, 0, 0 );
	position = G4ThreeVector(-pulleyBX2/2., 0., -element1BPosZ);
	new G4PVPlacement( 0, position, elem1BLogVol, "elemB1Physical", pulleyBMotherLogVol, 0, 1 );
	
	//element2B
	G4double element2BX = pulleyBX2;
	G4double element2BY = 0.150 *m_inch;
	G4double element2BZ = 0.468 *m_inch;
	G4Box* elem2BSolid = new G4Box ("elem2BSolid",element2BX/2., element2BY/2., element2BZ/2.);
	G4LogicalVolume*	elem2BLogVol = new G4LogicalVolume(elem2BSolid, rollerMaterial, "elem2BLogVol");
	position = G4ThreeVector(-(pulleyBX1/2. + pulleyBX2/2.), -(pulleyBY2/2. - element2BY/2.), pulleyBZ2/2.-element2BZ/2.);
	new G4PVPlacement( 0, position, elem2BLogVol, "elem2BPhysical", pulleyBMotherLogVol, 0, 0 );
	position = G4ThreeVector(-(pulleyBX1/2. + pulleyBX2/2.), -(pulleyBY2/2. - element2BY/2.), -(pulleyBZ2/2.-element2BZ/2.));
	new G4PVPlacement( 0, position, elem2BLogVol, "elem2BPhysical", pulleyBMotherLogVol, 0, 1 );	
	
	//element3B
	G4double element3BX = pulleyBX2;
	G4double element3BY = 0.142 *m_inch;
	G4double element3BZ = 0.8 *m_inch;
	G4Box* elem3BSolid = new G4Box ("elem3BSolid",element3BX/2., element3BY/2., element3BZ/2.);
	G4LogicalVolume*	elem3BLogVol = new G4LogicalVolume(elem3BSolid, rollerMaterial, "elem3BLogVol");
	position = G4ThreeVector(-(pulleyBX1/2. + pulleyBX2/2.), pulleyBY2/2. - element2BY/2., 0);
	new G4PVPlacement( 0, position, elem3BLogVol, "elem3BPhysical", pulleyBMotherLogVol, 0, 0 );

//position
	
	//type A	
	G4RotationMatrix*	rotationMatrixArrayA = new G4RotationMatrix();
	rotationMatrixArrayA->rotateY(pi/2.*rad);
	rotationMatrixArrayA->rotateZ(pi/2.*rad);
	//rotationMatrixArrayA->rotateZ(pi/2.*rad);
	G4double pulleyAYPosition = -0.5*(pulleyAX+0.15 * m_inch);
	position = G4ThreeVector(0., pulleyAYPosition, 12*m_inch);
	new G4PVPlacement( rotationMatrixArrayA, position, pulleyAMotherLogVol, "pulleyAMotherPhysical", m_logicWorld, 0, 0 );
	position = G4ThreeVector(0., pulleyAYPosition, -12*m_inch);
	new G4PVPlacement( rotationMatrixArrayA, position, pulleyAMotherLogVol, "pulleyAMotherPhysical", m_logicWorld, 0, 1 );	
	position = G4ThreeVector(0., pulleyAYPosition, 23.5*m_inch);
	new G4PVPlacement( rotationMatrixArrayA, position, pulleyAMotherLogVol, "pulleyAMotherPhysical", m_logicWorld, 0, 2 );
	position = G4ThreeVector(0., pulleyAYPosition, -23.5*m_inch);
	new G4PVPlacement( rotationMatrixArrayA, position, pulleyAMotherLogVol, "pulleyAMotherPhysical", m_logicWorld, 0, 3 );		
	position = G4ThreeVector(0., pulleyAYPosition, -35.21*m_inch);
	new G4PVPlacement( rotationMatrixArrayA, position, pulleyAMotherLogVol, "pulleyAMotherPhysical", m_logicWorld, 0, 4 );	
		
	G4RotationMatrix*	rotationMatrixArray = new G4RotationMatrix();
	rotationMatrixArray->rotateZ(pi/2.*rad);
	rotationMatrixArray->rotateX(pi/2.*rad);
	rotationMatrixArray->rotateZ(pi/2.*rad);
	G4double pulleyBYPosition = 0.1875*m_inch;
	//position = G4ThreeVector(0., pulleyBYPosition, 6.385*m_inch);
	position = G4ThreeVector(0., pulleyBYPosition, 4*m_inch);//4.215?
	//position = G4ThreeVector(0., 0.1875*m_inch, 0.);
	new G4PVPlacement( rotationMatrixArray, position, pulleyBMotherLogVol, "pulleyBMotherPhysical", m_logicWorld, 0, 0 );
	//position = G4ThreeVector(0., pulleyBYPosition, -6.385*m_inch);
	position = G4ThreeVector(0., pulleyBYPosition, -4*m_inch);
	new G4PVPlacement( rotationMatrixArray, position, pulleyBMotherLogVol, "pulleyBMotherPhysical", m_logicWorld, 0, 1 );	
	position = G4ThreeVector(0., pulleyBYPosition, 12.385*m_inch);
	new G4PVPlacement( rotationMatrixArray, position, pulleyBMotherLogVol, "pulleyBMotherPhysical", m_logicWorld, 0, 2 );
	position = G4ThreeVector(0., pulleyBYPosition, -12.385*m_inch);
	new G4PVPlacement( rotationMatrixArray, position, pulleyBMotherLogVol, "pulleyBMotherPhysical", m_logicWorld, 0, 3 );
	position = G4ThreeVector(0., pulleyBYPosition, 18.385*m_inch);
	new G4PVPlacement( rotationMatrixArray, position, pulleyBMotherLogVol, "pulleyBMotherPhysical", m_logicWorld, 0, 4 );
	position = G4ThreeVector(0., pulleyBYPosition, -18.385*m_inch);
	new G4PVPlacement( rotationMatrixArray, position, pulleyBMotherLogVol, "pulleyBMotherPhysical", m_logicWorld, 0, 5 );
	position = G4ThreeVector(0., pulleyBYPosition, 26.385*m_inch);
	new G4PVPlacement( rotationMatrixArray, position, pulleyBMotherLogVol, "pulleyBMotherPhysical", m_logicWorld, 0, 6 );

}

void MTASPulleys::SetVisibleAtributes()
{
	pulleyAMotherLogVol->SetVisAttributes(m_materialsAndColorsManager->GetRed());
	pulleyBMotherLogVol->SetVisAttributes(m_materialsAndColorsManager->GetRed());
}


