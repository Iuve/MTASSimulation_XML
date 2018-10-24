/**
* Class constructing all hexagon modules
*/

#include <math.h> 
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4Polyhedra.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "MTASHexagonModules.hh"
  
MTASHexagonModules::MTASHexagonModules(G4LogicalVolume* logicWorld)
{
	m_logicWorld=logicWorld;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
	SetParameters();
	Construct();
	LocateModules();
	SetVisibleAtributes();
}

MTASHexagonModules::~MTASHexagonModules()
{
	//if(m_materialsAndColorsManager != 0)
		//delete m_materialsAndColorsManager;
}

void MTASHexagonModules::SetParameters()
{ 
	
	M_Pi=4*atan(1);
	m_inch=2.54*cm;
	m_NumberOfLayers=4;
	m_CristalSide= 4 *m_inch;
	m_CristalLength= 21 *m_inch;
	m_Layer0Thickness = 0.81 * mm;//0.81
	m_Layer1Thickness = 0.04 * mm;//0.04
	m_Layer2Thickness = 2.025 * mm;//0.6946 * mm;

//m_Layer1Thickness = 0.04 * mm;

	m_Layer3Thickness = 0.5 * mm;
	
	m_Layer0InCenterHoleThickness = m_Layer0Thickness;
	m_Layer1InCenterHoleThickness = m_Layer1Thickness;
	m_Layer2InCenterHoleThickness = 0.68 * mm;
	//m_Layer2InCenterHoleThickness = m_Layer2Thickness;
	m_Layer3InCenterHoleThickness = m_Layer3Thickness;
	
	//m_PipeRadius = 1 * m_inch;//Promień zerwnętrzny aluminiowej rury
	m_PipeLength = m_CristalLength;
	m_TubsRadius = 1.25 * m_inch;// + 0.5 * mm;
}

void MTASHexagonModules::Construct()
{
	//Materials settings
	G4Material* Cristal_Material= m_materialsAndColorsManager->GetNaI();
	G4Material* layer0_Material = m_materialsAndColorsManager->GetCarbonFiber();
	G4Material* layer1_Material = m_materialsAndColorsManager->GetSteel();
	G4Material* layer2_Material = m_materialsAndColorsManager->GetSiliconPutty();
	G4Material* layer3_Material = m_materialsAndColorsManager->GetTeflon();



	//Construct hexagon
	const G4int numberSides = 6;
	const G4int numberZPlanes = 2;
	const G4double  startPhi = 0.0 * degree;
	const G4double  deltaPhi = 360.0 * degree;
	const G4double rZero[numberZPlanes] = { 0.0, 0.0 };
	const G4double halfZ0=0.5* m_CristalLength+m_Layer0Thickness +
	m_Layer1Thickness + m_Layer2Thickness + m_Layer3Thickness;
	const G4ThreeVector zero( 0.0, 0.0, 0.0 );

	//Layer 0 - external
	const G4double radius0=m_CristalSide*sqrt(3)/2.0+m_Layer0Thickness +
	m_Layer1Thickness + m_Layer2Thickness + m_Layer3Thickness;
	const G4double r0_Outer[numberZPlanes] = { radius0, radius0 };
	const G4double layer0HalfLength = halfZ0;
	const G4double zPlanes_0[numberZPlanes] = { -layer0HalfLength, layer0HalfLength };
	

	G4Polyhedra *layer0_Solid = new G4Polyhedra("Layer0_Solid", startPhi, deltaPhi,
	numberSides, numberZPlanes, zPlanes_0, rZero, r0_Outer );
	m_layer0_logVol = new G4LogicalVolume( layer0_Solid, layer0_Material, "Layer0_Logical");


	//Layer 1
	const G4double radius1 = radius0 - m_Layer0Thickness;
	const G4double r1_Outer[numberZPlanes] = { radius1, radius1 };
	const G4double layer1HalfLength = layer0HalfLength-m_Layer0Thickness;
	const G4double zPlanes_1[numberZPlanes] = { -layer1HalfLength, layer1HalfLength };
			
	G4Polyhedra *layer1_Solid = new G4Polyhedra("Layer1_Solid", startPhi, deltaPhi,
	numberSides, numberZPlanes, zPlanes_1, rZero, r1_Outer );
	m_layer1_logVol = new G4LogicalVolume( layer1_Solid, layer1_Material, "Layer1_Logical");

	new G4PVPlacement( 0, zero, m_layer1_logVol, "Layer1_Physical", m_layer0_logVol, 0, 0 );		

	//Layer 2
	const G4double radius2 = radius1 - m_Layer1Thickness;
	const G4double r2_Outer[numberZPlanes] = { radius2, radius2 };
	const G4double layer2HalfLength = layer1HalfLength-m_Layer1Thickness;
	const G4double zPlanes_2[numberZPlanes] = { -layer2HalfLength, layer2HalfLength };
			
	G4Polyhedra *layer2_Solid = new G4Polyhedra("Layer2_Solid", startPhi, deltaPhi,
	numberSides, numberZPlanes, zPlanes_2, rZero, r2_Outer );
	m_layer2_logVol = new G4LogicalVolume( layer2_Solid, layer2_Material, "Layer2_Logical");

	new G4PVPlacement( 0, zero, m_layer2_logVol, "Layer2_Physical", m_layer1_logVol, 0, 0 );

	//Layer 3
	const G4double radius3 = radius2 - m_Layer2Thickness; 
	const G4double r3_Outer[numberZPlanes] = { radius3, radius3 };
	const G4double layer3HalfLength = layer2HalfLength-m_Layer2Thickness;
	const G4double zPlanes_3[numberZPlanes] = { -layer3HalfLength, layer3HalfLength};
			
	G4Polyhedra *layer3_Solid = new G4Polyhedra("Layer3_Solid", startPhi, deltaPhi,
	numberSides, numberZPlanes, zPlanes_3, rZero, r3_Outer );
	m_layer3_logVol = new G4LogicalVolume( layer3_Solid, layer3_Material, "Layer3_Logical");

	new G4PVPlacement( 0, zero, m_layer3_logVol, "Layer2_Physical", m_layer2_logVol, 0, 0 );

	//NaI Crystal
	const G4double radius_NaI = m_CristalSide*sqrt(3)/2.0;
	const G4double rNaI_Outer[numberZPlanes] = { radius_NaI, radius_NaI };
	const G4double zNaIPlanes[numberZPlanes] = { -0.5* m_CristalLength, 0.5* m_CristalLength };
			
	G4Polyhedra *NaI_Solid = new G4Polyhedra("NaI_Solid", startPhi, deltaPhi,  numberSides,
	numberZPlanes, zNaIPlanes, rZero, rNaI_Outer );
	m_NaI_logVol = new G4LogicalVolume( NaI_Solid, Cristal_Material, "NaI_Logical");

	new G4PVPlacement( 0, zero, m_NaI_logVol, "NaI_Physical", m_layer3_logVol, 0, 0 );

//*********************************************************************************************************************************
	//Central module - with the hole

	//Layer 0
	G4double TubsRadius0 = m_TubsRadius;
	G4Polyhedra *layer0_Centre_Solid = new G4Polyhedra("Layer0_Centre_Solid", startPhi,
	deltaPhi,numberSides, numberZPlanes, zPlanes_0, rZero, r0_Outer ); 
	G4Tubs * tubs0_Solid = new G4Tubs ("Tubs0_Solid", 0, TubsRadius0, layer0HalfLength, startPhi, deltaPhi);
	G4SubtractionSolid * hexagon_tubs_subtraction0 = new G4SubtractionSolid("Hexagon_tubs_subtraction0",
	layer0_Centre_Solid,tubs0_Solid);

	m_layer0_Centre_logVol = new G4LogicalVolume( hexagon_tubs_subtraction0,
	layer0_Material, "Layer0_Centre_Logical");

	//Layer 1
	G4double TubsRadius1 = TubsRadius0 + m_Layer0InCenterHoleThickness;
	G4Polyhedra *layer1_Centre_Solid = new G4Polyhedra("Layer1_Centre_Solid", startPhi,
	deltaPhi, numberSides, numberZPlanes, zPlanes_1, rZero, r1_Outer );
	G4Tubs * tubs1_Solid = new G4Tubs ("Tubs1_Solid", 0, TubsRadius1, 
	layer1HalfLength, startPhi, deltaPhi);
	G4SubtractionSolid * hexagon_tubs_subtraction1 =
	new G4SubtractionSolid("Hexagon_tubs_subtraction1",layer1_Centre_Solid,tubs1_Solid);

	m_layer1_Centre_logVol = new G4LogicalVolume( hexagon_tubs_subtraction1, layer1_Material, "Layer1_Centre_Logical");
	new G4PVPlacement( 0, zero, m_layer1_Centre_logVol, "Layer1_Centre_Physical", m_layer0_Centre_logVol, 0, 0 );

	//Layer 2
	G4double TubsRadius2 = TubsRadius1 + m_Layer1InCenterHoleThickness;
	G4Polyhedra *layer2_Centre_Solid = new G4Polyhedra("Layer2_Centre_Solid", startPhi,
	deltaPhi, numberSides, numberZPlanes, zPlanes_2, rZero, r2_Outer );
	G4Tubs * tubs2_Solid = new G4Tubs ("Tubs2_Solid", 0, TubsRadius2,
	layer2HalfLength, startPhi, deltaPhi);
	G4SubtractionSolid * hexagon_tubs_subtraction2 =
	new G4SubtractionSolid("Hexagon_tubs_subtraction2",layer2_Centre_Solid,tubs2_Solid);

	m_layer2_Centre_logVol = new G4LogicalVolume( hexagon_tubs_subtraction2, layer2_Material, "Layer2_Centre_Logical");
	new G4PVPlacement( 0, zero, m_layer2_Centre_logVol, "Layer2_Centre_Physical", m_layer1_Centre_logVol, 0, 0 );

	//Layer 3
	G4double TubsRadius3 = TubsRadius2 + m_Layer2InCenterHoleThickness;
	G4Polyhedra *layer3_Centre_Solid = new G4Polyhedra("Layer3_Centre_Solid", startPhi,
	 deltaPhi, numberSides, numberZPlanes, zPlanes_3, rZero, r3_Outer );
	G4Tubs * tubs3_Solid = new G4Tubs ("Tubs3_Solid", 0, TubsRadius3,
	layer3HalfLength, startPhi, deltaPhi);
	G4SubtractionSolid * hexagon_tubs_subtraction3 =
	new G4SubtractionSolid("Hexagon_tubs_subtraction3",layer3_Centre_Solid,tubs3_Solid);

	m_layer3_Centre_logVol = new G4LogicalVolume( hexagon_tubs_subtraction3, layer3_Material, "Layer3_Centre_Logical");
	new G4PVPlacement( 0, zero, m_layer3_Centre_logVol, "Layer3_Centre_Physical", m_layer2_Centre_logVol, 0, 0 );

	//NaI Crystal
	G4double TubsRadiusNaI = TubsRadius3 + m_Layer3InCenterHoleThickness;
	G4Polyhedra *NaI_Centre_Solid = new G4Polyhedra("NaI_Centre_Solid", startPhi,
	deltaPhi, numberSides, numberZPlanes, zNaIPlanes, rZero, rNaI_Outer );
	G4Tubs * tubsNaI_Solid = new G4Tubs ("TubsNaI_Solid", 0, TubsRadiusNaI,
	0.5* m_CristalLength, startPhi, deltaPhi);
	G4SubtractionSolid * hexagon_tubs_subtractionNaI =
	new G4SubtractionSolid("Hexagon_tubs_subtractionNaI",NaI_Centre_Solid,tubsNaI_Solid);

	m_NaI_Centre_logVol = new G4LogicalVolume( hexagon_tubs_subtractionNaI, Cristal_Material, "NaI_Centre_Logical");
	new G4PVPlacement( 0, zero, m_NaI_Centre_logVol, "NaI_Centre_Physical", m_layer3_Centre_logVol, 0, 0 );
	
}


void MTASHexagonModules::LocateModules()
{
	G4ThreeVector position( 0.0, 0.0, 0.0 );

	//Central module
	new G4PVPlacement( 0, position, m_layer0_Centre_logVol, "Hexagon_Centre_Module_Physical", m_logicWorld, 0, 0 );

	//Other modules
	const G4double radius0=m_CristalSide*sqrt(3)/2.0+m_Layer0Thickness + m_Layer1Thickness + m_Layer2Thickness + m_Layer3Thickness;
	G4double mainRadius=radius0*2.0/sqrt(3);
	G4double Fi0;
	const G4double DeltaFi=M_PI/3.0;

	//Inner ring
	const G4double FirstRingRadius=mainRadius*sqrt(3);
	Fi0=0.0;

	for(int i=0; i<6; i++)
	{
		position = G4ThreeVector( FirstRingRadius*sin(Fi0+i*DeltaFi), FirstRingRadius*cos(Fi0+i*DeltaFi), 0 );
		new G4PVPlacement( 0, position, m_layer0_logVol, "HexagonModule_Physical", m_logicWorld, 0, i );
	}

	//Middle ring
	const G4double SecondRingRadius=mainRadius*3;
	Fi0=M_Pi/6.0;
	for(int i=6; i<12; i++)
	{
		position = G4ThreeVector( SecondRingRadius*sin(Fi0+i*DeltaFi), SecondRingRadius*cos(Fi0+i*DeltaFi), 0 );
		new G4PVPlacement( 0, position, m_layer0_logVol, "HexagonModule_Physical", m_logicWorld, 0, i );
	}

	//External ring
	const G4double ThirdRingRadius=mainRadius*2*sqrt(3);
	Fi0=0.0;
	for(int i=12; i<18; i++)
	{
		position = G4ThreeVector( ThirdRingRadius*sin(Fi0+i*DeltaFi), ThirdRingRadius*cos(Fi0+i*DeltaFi), 0 );
		new G4PVPlacement( 0, position, m_layer0_logVol, "HexagonModule_Physical", m_logicWorld, 0, i );
	}

}

void MTASHexagonModules::SetVisibleAtributes()
{
	//Green color for NaI volumes
	m_NaI_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGreen());
	m_NaI_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGreen());

	//Grey color for Carbon and Alu volumes - layer0 and 1
	m_layer0_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());
	m_layer1_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());
	m_layer0_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());
	m_layer1_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());

	//Yellow color for Si volume - layer 2 and BC408
	m_layer2_logVol->SetVisAttributes(m_materialsAndColorsManager->GetYellow());
	m_layer2_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetYellow());
  
	//White color for MgO or teflon volumes - layer 3
	m_layer3_logVol->SetVisAttributes(m_materialsAndColorsManager->GetWhite());
	m_layer3_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetWhite());
	
	/*m_NaI_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	m_NaI_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());

	//Grey color for Carbon and Alu volumes - layer0 and 1
	m_layer0_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	m_layer1_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	m_layer0_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	m_layer1_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());

	//Yellow color for Si volume - layer 2 and BC408
	m_layer2_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	m_layer2_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
  
	//White color for MgO or teflon volumes - layer 3
	m_layer3_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());
	m_layer3_Centre_logVol->SetVisAttributes(m_materialsAndColorsManager->GetBlack());*/
}


