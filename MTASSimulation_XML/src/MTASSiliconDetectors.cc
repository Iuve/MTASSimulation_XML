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
#include "G4VisAttributes.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "MTASSiliconDetectors.hh"
  
MTASSiliconDetectors::MTASSiliconDetectors(G4LogicalVolume* logicWorld)
{
	m_logicWorld=logicWorld;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
	SetParameters();
	Construct();
	SetVisibleAtributes();
}

MTASSiliconDetectors::~MTASSiliconDetectors()
{
	//if(m_materialsAndColorsManager != 0)
		//delete m_materialsAndColorsManager;
}

void MTASSiliconDetectors::SetParameters()
{ 
	
	M_Pi=4*atan(1);
	m_inch=2.54*cm;

	//silicon panel sizes
	m_panelLength = 43.6 *mm; //x
	m_panelWidth = 63 *mm; //z
	m_panelThickness = 1 *mm; //y
	m_activeAreaLendth = 40 *mm;
	m_activeAreaWidth = 60 *mm;
	m_stripSeparation = 0.01 *mm;
	m_nrOfStrips = 7;	
	m_deadLayerThickness = 0.000*mm;

	//connector panel sizes:
	m_connPanA = 0.8 *mm;
	m_connPanB = 4 *mm;
	m_connPanC = 7.33 *mm;
	m_connPanD = 1.6 *mm;
	m_connPanZ = 54.88 *mm;
	
	m_framePositionY = 2.667*mm;
	//m_siliconPositionY = 1.024*mm;
	//m_siliconPositionY = 1.135*mm;
	m_siliconPositionY = 1.5*mm;
}

void MTASSiliconDetectors::Construct()
{
	ConstructFrame();
	ConstructSilicon();
}
void MTASSiliconDetectors::ConstructFrame()
{
	G4ThreeVector position( 0.0, 0.0, 0.0 );
	G4RotationMatrix* yRot = new G4RotationMatrix; 
	yRot->rotateY(0. *rad); 
	G4ThreeVector zTrans(0., 0., 0);
	
	//first element of plastic frame ***************************************
	G4Material *frameMaterial = m_materialsAndColorsManager->GetDerlin();
	
	G4double bigFrameX = 1.85 *m_inch;
	G4double bigFrameY = 0.09 *m_inch;
	G4double bigFrameZ = 3.63 *m_inch;
	G4Box *frame1_1Solid = new G4Box("frame1_1Solid", bigFrameX/2., bigFrameY/2., bigFrameZ/2.);
	
	//big hole dimentions:
	G4double bigHoleX = 1.56 * m_inch;
	G4double bigHoleY = bigFrameY;
	G4double bigHoleZ = 2.37 * m_inch;
	G4Box *frame1_2Solid = new G4Box("frame1_2Solid", bigHoleX/2., bigHoleY/2., bigHoleZ/2.);
	zTrans = G4ThreeVector(-0.075 * m_inch, 0., 0.);
	G4SubtractionSolid *frame1_3Solid = new G4SubtractionSolid("frame1_3Solid",
	frame1_1Solid, frame1_2Solid, yRot, zTrans);
	
	//left end right:
	G4double leftHoleX = 1.0 * m_inch;
	G4double leftHoleY = bigFrameY;
	G4double leftHoleZ = 0.48 * m_inch;
	G4Box *frame1_4Solid = new G4Box("frame1_4Solid", leftHoleX/2., leftHoleY/2., leftHoleZ/2.);
	zTrans = G4ThreeVector(0., 0., -1.575 * m_inch);
	G4SubtractionSolid *frame1_5Solid = new G4SubtractionSolid("frame1_5Solid",
	frame1_3Solid, frame1_4Solid, yRot, zTrans);//L
	zTrans = G4ThreeVector(0., 0., 1.575 * m_inch);
	G4SubtractionSolid *frame1_6Solid = new G4SubtractionSolid("frame1_6Solid",
	frame1_5Solid, frame1_4Solid, yRot, zTrans);//R
	
	//bottom hole:
	G4double bottomHoleX = 0.16 * m_inch;
	G4double bottomHoleY = bigFrameY;
	G4double bottomHoleZ = 2.23 * m_inch;
	G4Box *frame1_7Solid = new G4Box("frame1_7Solid", bottomHoleX/2., bottomHoleY/2., bottomHoleZ/2.);
	zTrans = G4ThreeVector(0.845 * m_inch, 0., 0.);
	G4SubtractionSolid *frame1_8Solid = new G4SubtractionSolid("frame1_8Solid",
	frame1_6Solid, frame1_7Solid, yRot, zTrans);
			
	G4LogicalVolume *frame1Logic = new G4LogicalVolume(frame1_8Solid, frameMaterial, "frame1Logic");
	//frame1Logic->SetVisAttributes(m_materialsAndColorsManager->GetRed());
	frame1Logic->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	position = G4ThreeVector(0., m_framePositionY, 0.);
	new G4PVPlacement(0, position, frame1Logic, "frame1Physical", m_logicWorld, 0, 0 );	
	
	//second frame - smaller *********************************************
	G4double smallFrameZ = 2.98 *m_inch;
	G4Box *frame2_1Solid = new G4Box("frame2_1Solid", bigFrameX/2., bigFrameY/2., smallFrameZ/2.);
	
	//big hole dimentions:
	zTrans = G4ThreeVector(-0.075 * m_inch, 0., 0.);
	G4SubtractionSolid *frame2_3Solid = new G4SubtractionSolid("frame2_3Solid",
	frame2_1Solid, frame1_2Solid, yRot, zTrans);
	
	//left end right:
	zTrans = G4ThreeVector(0., 0., -1.575 * m_inch);
	G4SubtractionSolid *frame2_5Solid = new G4SubtractionSolid("frame2_5Solid",
	frame2_3Solid, frame1_4Solid, yRot, zTrans);//L
	zTrans = G4ThreeVector(0., 0., 1.575 * m_inch);
	G4SubtractionSolid *frame2_6Solid = new G4SubtractionSolid("frame2_6Solid",
	frame2_5Solid, frame1_4Solid, yRot, zTrans);//R
	
	//bottom hole:
	zTrans = G4ThreeVector(0.845 * m_inch, 0., 0.);
	G4SubtractionSolid *frame2_8Solid = new G4SubtractionSolid("frame2_8Solid",
	frame2_6Solid, frame1_7Solid, yRot, zTrans);
			
	G4LogicalVolume *frame2Logic = new G4LogicalVolume(frame2_8Solid, frameMaterial, "frame2Logic");
	frame2Logic->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	//frame2Logic->SetVisAttributes(m_materialsAndColorsManager->GetRed());
	position = G4ThreeVector(0., -m_framePositionY, 0.);
	G4RotationMatrix*	rotationMatrixArray = new G4RotationMatrix();
	rotationMatrixArray->rotateY(CLHEP::pi*rad);
	new G4PVPlacement(rotationMatrixArray, position, frame2Logic, "frame2Physical", m_logicWorld, 0, 0 );	
	delete yRot;
}

void MTASSiliconDetectors::ConstructSilicon()
{	
	G4ThreeVector position( 0.0, 0.0, 0.0 );
	G4RotationMatrix* yRot = new G4RotationMatrix; 
	yRot->rotateY(0. *rad); 
	G4ThreeVector zTrans(0., 0., 0);
	G4Material* deadLayerMaterial= m_materialsAndColorsManager->GetSilver();
	G4Material* siliconMaterial= m_materialsAndColorsManager->GetSilicon();
	//panel
	G4Box *siliPanelSolid = new G4Box ("siliPanelSolid", m_panelLength/2., m_panelThickness/2., m_panelWidth/2.);
	siliPanelLogUp = new G4LogicalVolume(siliPanelSolid, deadLayerMaterial, "siliPanelLogUp");
	siliPanelLogDown = new G4LogicalVolume(siliPanelSolid, deadLayerMaterial, "siliPanelLogDown");
		
	//strips
	G4double stripWidth = (m_activeAreaWidth - (m_nrOfStrips-1.)*m_stripSeparation)/m_nrOfStrips;
	G4Box *siliStripSolid = new G4Box("siliStripSolid", m_activeAreaLendth/2., m_panelThickness/2.- m_deadLayerThickness, stripWidth/2.);
	siliStripLog = new G4LogicalVolume(siliStripSolid, siliconMaterial, "siliStripLog");
	for(int i=0; i<7; i++)
	{
		//double xPos = (m_panelLength - m_activeAreaLendth)/2.;
		position = G4ThreeVector(  0., 0., (double)(i-3)*(stripWidth+m_stripSeparation));
		new G4PVPlacement( 0, position, siliStripLog, "siliStripPhysical", siliPanelLogUp, 0, i );
	}
	
	for(int i=0; i<7; i++)
	{
		//double xPos = (m_panelLength - m_activeAreaLendth)/2.;
		position = G4ThreeVector(  0., 0., (double)(i-3)*(stripWidth+m_stripSeparation));
		new G4PVPlacement( 0, position, siliStripLog, "siliStripPhysical", siliPanelLogDown, 0, i+7 );
	}
	//position = G4ThreeVector( -(m_connPanC-m_connPanB)/2., 0., 0.);
	//new G4PVPlacement(0, position, siliPanelLog, "siliPanelPhysical", m_logicWorld, 0, 0 );
	
	
//connector ************************************************************
	G4Material* connectorMaterial= m_materialsAndColorsManager->GetGlassFiber();

	G4Box *connPart1Solid = new G4Box ("connPart1Solid", m_connPanC/2., (m_connPanD-m_connPanA)/2., m_connPanZ/2.);
	G4Box *connPart2Solid = new G4Box ("connPart2Solid", (m_connPanC-m_connPanB) /2., m_connPanA/2., m_connPanZ/2.);
	
	zTrans = G4ThreeVector(-m_connPanB/2., -m_connPanD/2., 0);
	G4UnionSolid *connectorSolid = new G4UnionSolid("connectorSolid", connPart2Solid, connPart1Solid, yRot, zTrans);
	connectorLog = new G4LogicalVolume (connectorSolid, connectorMaterial, "connectorLog");
	
	position = G4ThreeVector( m_panelLength/2., -(m_panelThickness-m_connPanA)/2., 0.0 );
	//new G4PVPlacement(0, position, connectorLog, "connectorPhysical", m_logicWorld, 0, 0 );

	
//mother ******************************************************************
	G4Material* motherMaterial= m_materialsAndColorsManager->GetAir();
	zTrans = G4ThreeVector(-m_panelLength/2.-(m_connPanC-m_connPanB)/2., +(m_panelThickness-m_connPanA)/2., 0.);
	G4UnionSolid* motherSolid = new G4UnionSolid("motherSolid", connectorSolid,siliPanelSolid,  yRot, zTrans);
	G4LogicalVolume *motherLogicUp = new G4LogicalVolume(motherSolid, motherMaterial, "motherLogUp");
	G4LogicalVolume *motherLogicDown = new G4LogicalVolume(motherSolid, motherMaterial, "motherLogDown");

	//motherLogicUp->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	//motherLogicDown->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	motherLogicUp->SetVisAttributes(m_materialsAndColorsManager->GetWhite());
	motherLogicDown->SetVisAttributes(m_materialsAndColorsManager->GetWhite());
	
	position = G4ThreeVector(-m_panelLength/2.-(m_connPanC-m_connPanB)/2., +(m_panelThickness-m_connPanA)/2., 0.);
	new G4PVPlacement(0, position, siliPanelLogUp, "siliPanelPhysical", motherLogicUp, 0, 0 );
	new G4PVPlacement(0, position, siliPanelLogDown, "siliPanelPhysical", motherLogicDown, 0, 0 );
	
	position = G4ThreeVector( 0., 0., 0.0 );
	new G4PVPlacement(0, position, connectorLog, "connectorPhysical", motherLogicUp, 0, 0 );
	new G4PVPlacement(0, position, connectorLog, "connectorPhysical", motherLogicDown, 0, 0 );

//upper silicon	
	position = G4ThreeVector( m_panelLength/2., m_siliconPositionY-(m_panelThickness-m_connPanA)/2., 0.0 );
	new G4PVPlacement(0, position, motherLogicUp, "motherPhysicalUp", m_logicWorld, 0, 0 );
	
//lower silicon
	position = G4ThreeVector(-m_panelLength/2., -m_siliconPositionY-(m_panelThickness-m_connPanA)/2., 0.);
	G4RotationMatrix*	rotationMatrixArray = new G4RotationMatrix();
	rotationMatrixArray->rotateY(CLHEP::pi*rad);
	rotationMatrixArray->rotateZ(CLHEP::pi*rad);
	rotationMatrixArray->rotateY(CLHEP::pi*rad);
	new G4PVPlacement(rotationMatrixArray, position, motherLogicDown, "motherPhysicalDown", m_logicWorld, 0, 0 );
	
	delete yRot;
}

void MTASSiliconDetectors::SetVisibleAtributes()
{
	siliPanelLogUp->SetVisAttributes(m_materialsAndColorsManager->GetWhite());
	siliPanelLogDown->SetVisAttributes(m_materialsAndColorsManager->GetWhite());
	siliStripLog->SetVisAttributes(m_materialsAndColorsManager->GetYellow());
	connectorLog->SetVisAttributes(m_materialsAndColorsManager->GetGreen());
	//absorberLogVol->SetVisAttributes(m_materialsAndColorsManager->GetRed());
	
	/*siliPanelLogUp->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	siliPanelLogDown->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	siliStripLog->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	connectorLog->SetVisAttributes(m_materialsAndColorsManager->GetBlack());*/
}


