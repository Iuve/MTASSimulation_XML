/**
* Class constructing Aluminium Plates
*/

#include <math.h> 

#include "MTASAlPlate.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4Polyhedra.hh"
using namespace CLHEP;

  
MTASAlPlate::MTASAlPlate(G4LogicalVolume* logicWorld) 
{
	m_logicWorld=logicWorld;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
	SetParameters();
	Construct();
	SetVisibleAtributes();
}
 
MTASAlPlate::~MTASAlPlate()
{
	//if(m_materialsAndColorsManager != 0)
		//delete m_materialsAndColorsManager;
}

void MTASAlPlate::SetParameters()
{ 
	m_inch=2.54*cm; 
	M_Pi=4*atan(1);
	
	m_CristalSide=4 *m_inch;//It should be getted form MTASHexagonModules
	m_AllLayersThickness = (0.81+0.04+0.6946+0.5) * mm;
	m_AlThickPlateRadius = 5*m_CristalSide; //closer NaI Radius- distance from the centre to the side
	m_AlThickPlateThickness = 19.1 * mm;
	m_AlThickPlateDistance=0.5*614 *mm;
	m_HoleInThickPlateRadius = 3 *m_inch;
  
	m_LiftingEyesLength = 60 *mm;

	m_RodsLength= 1249 *mm;
  
	m_AlThinPlateRadius = 5*m_CristalSide; //farther NaI
	m_AlThinPlateThickness = 12.7 * mm;
	m_AlThinPlateDistance = 0.5*m_RodsLength;
	m_HoleInThinPlateRadius = 1.5 *m_inch;
	m_CentralHoleInThinPlateRadius = 4 *m_inch;
	m_HoleToRodRadius = 1 *m_inch;
	m_RodLocateRadius = m_AlThinPlateRadius*2/sqrt(3)-5 * cm - m_HoleToRodRadius; 
}  
  
  
void MTASAlPlate::Construct()
{

	G4Material* AlPlate_Material = m_materialsAndColorsManager->GetAluminium();

	const G4int numberSides = 6;
	const G4int numberZPlanes = 2;
	const G4double  startPhi = 0.0 * degree;
	const G4double  deltaPhi = 360.0 * degree;
	const G4double rZero[numberZPlanes] = { 0.0, 0.0 };

//thick plate	
	const G4double AlRadius[2] = { m_AlThickPlateRadius, m_AlThickPlateRadius };
	const G4double AlZThickPlanes[2] = { -m_AlThickPlateThickness/2., m_AlThickPlateThickness/2. };
	G4Polyhedra *AlThick_Solid = new G4Polyhedra("AlThick_Solid", startPhi+30.0 * degree,
	 deltaPhi+30.0 * degree,  numberSides, numberZPlanes, AlZThickPlanes, rZero, AlRadius );
	G4Tubs * HoleInThickPlate_Solid = new G4Tubs ("HoleInThickPlate_Solid", 0, 
	m_HoleInThickPlateRadius,m_AlThickPlateThickness/2. , startPhi, deltaPhi);
	G4Tubs * ThickHoleToRod_Solid = new G4Tubs ("ThickHoleToRod_Solid", 0, 
	m_HoleToRodRadius,m_AlThickPlateThickness/2. , startPhi, deltaPhi);
  
//thin plate
	const G4double AlZThinPlanes[2] = { -m_AlThinPlateThickness/2., m_AlThinPlateThickness/2. };
	G4Polyhedra *AlThin_Solid = new G4Polyhedra("AlThin_Solid", startPhi+30.0 * degree,
	deltaPhi+30.0 * degree,  numberSides, numberZPlanes, AlZThinPlanes, rZero, AlRadius );
	G4Tubs * HoleInThinPlate_Solid = new G4Tubs ("HoleInThinPlate_Solid", 0,
	m_HoleInThinPlateRadius, m_AlThinPlateThickness/2., startPhi, deltaPhi);
	G4Tubs * ThinHoleToRod_Solid = new G4Tubs ("ThinHoleToRod_Solid", 0,
	m_HoleToRodRadius, m_AlThinPlateThickness/2., startPhi, deltaPhi);
	G4Tubs * CentralHoleInThinPlate_Solid = new G4Tubs ("CentralHoleInThinPlate_Solid", 0,
	m_CentralHoleInThinPlateRadius, m_AlThickPlateThickness/2., startPhi, deltaPhi);

	G4RotationMatrix *zRot = new G4RotationMatrix;
	zRot -> rotateZ(0*rad);
	G4ThreeVector zTrans(0.0,0.0,0.0);
  
//Central hole
	G4SubtractionSolid *ThickPlate[25];
	G4SubtractionSolid *ThinPlate[25];
	ThickPlate[0]=new G4SubtractionSolid("ThickPlate_Subtraction",
	AlThick_Solid, HoleInThickPlate_Solid, zRot, zTrans);
	ThinPlate[0]= new G4SubtractionSolid("ThickPlate_Subtraction",
	AlThin_Solid, CentralHoleInThinPlate_Solid, zRot, zTrans);
	G4double Fi0=M_Pi/6.0;
	
	const G4double radius0=m_CristalSide*sqrt(3)/2.0+m_AllLayersThickness;
	const G4double MainRadius=radius0*2.0/sqrt(3);
	const G4double FirstRingRadius=MainRadius*sqrt(3);
	const G4double SecondRingRadius=MainRadius*3;
	const G4double ThirdRingRadius=MainRadius*2*sqrt(3);
	const G4double DeltaFi=M_PI/3.0;
	
	for(int i=0; i<6; i++)
	{
		zTrans = G4ThreeVector( FirstRingRadius*cos(Fi0+i*DeltaFi), FirstRingRadius*sin(Fi0+i*DeltaFi), 0 );
		ThickPlate[i+1] = new G4SubtractionSolid("ThickPlate_Subtraction", ThickPlate[i],
		HoleInThickPlate_Solid, zRot, zTrans);
		ThinPlate[i+1] = new G4SubtractionSolid("ThickPlate_Subtraction", ThinPlate[i],
		HoleInThinPlate_Solid, zRot, zTrans);
	}
   
	Fi0=0;
	for(int i=6; i<12; i++)
	{
		zTrans = G4ThreeVector( SecondRingRadius*cos(Fi0+i*DeltaFi), SecondRingRadius*sin(Fi0+i*DeltaFi), 0 );
		ThickPlate[i+1] = new G4SubtractionSolid("ThickPlate_Subtraction",
		ThickPlate[i], HoleInThickPlate_Solid, zRot, zTrans);
		ThinPlate[i+1] = new G4SubtractionSolid("ThickPlate_Subtraction",
		ThinPlate[i], HoleInThinPlate_Solid, zRot, zTrans);
	}

	Fi0=M_PI/6.0;
	for(int i=12; i<18; i++)
	{
		zTrans = G4ThreeVector( ThirdRingRadius*cos(Fi0+i*DeltaFi), ThirdRingRadius*sin(Fi0+i*DeltaFi), 0 );
		ThickPlate[i+1] = new G4SubtractionSolid("ThickPlate_Subtraction",
		ThickPlate[i], HoleInThickPlate_Solid, zRot, zTrans);
		ThinPlate[i+1] = new G4SubtractionSolid("ThickPlate_Subtraction",
		ThinPlate[i], HoleInThinPlate_Solid, zRot, zTrans);
	}
    
	for(int i=0; i<6; i++)
	{
		zTrans = G4ThreeVector( m_RodLocateRadius*cos(Fi0+i*DeltaFi), m_RodLocateRadius*sin(Fi0+i*DeltaFi), 0 );
		ThickPlate[i+19] = new G4SubtractionSolid("ThickPlate_Subtraction",
		ThickPlate[i+18], ThickHoleToRod_Solid, zRot, zTrans);
		ThinPlate[i+19] = new G4SubtractionSolid("ThickPlate_Subtraction",
		ThinPlate[i+18], ThinHoleToRod_Solid, zRot, zTrans);
	}
    
	m_AlThickPlate_logVol = new G4LogicalVolume( ThickPlate[24], AlPlate_Material, "AlThick_Logical");  
	m_AlThinPlate_logVol = new G4LogicalVolume( ThinPlate[24], AlPlate_Material, "AlThin_Logical");
 
	G4ThreeVector position = G4ThreeVector( 0, 0, -m_AlThickPlateDistance );
	new G4PVPlacement( 0, position, m_AlThickPlate_logVol, "AlThickPlate_Physical", m_logicWorld, 0, 1 );
	position = G4ThreeVector( 0, 0, m_AlThickPlateDistance );
	new G4PVPlacement( 0, position, m_AlThickPlate_logVol, "AlThickPlate_Physical", m_logicWorld, 0, 2 );
  
	position = G4ThreeVector( 0, 0, -m_AlThinPlateDistance );
	new G4PVPlacement( 0, position, m_AlThinPlate_logVol, "AlThinPlate_Physical", m_logicWorld, 0, 1 );
	position = G4ThreeVector( 0, 0, m_AlThinPlateDistance );
	new G4PVPlacement( 0, position, m_AlThinPlate_logVol, "AlThinPlate_Physical", m_logicWorld, 0, 2 );

//rods  
	G4double AllRodsLength = m_LiftingEyesLength + m_RodsLength;
  
	G4Tubs * Rod_Solid = new G4Tubs ("Rod_Solid", 0, m_HoleToRodRadius, AllRodsLength/2., startPhi, deltaPhi);
	m_Rod_logVol = new G4LogicalVolume(Rod_Solid, AlPlate_Material, "Rod_Logical");

	Fi0=M_Pi/6.0;
	for(int i=0; i<6; i++)
	{
		position = G4ThreeVector(m_RodLocateRadius*cos(Fi0+i*DeltaFi), m_RodLocateRadius*sin(Fi0+i*DeltaFi), 0);
		new G4PVPlacement(0, position, m_Rod_logVol, "Rod_Physical", m_logicWorld, 0, i);
	}
}


void MTASAlPlate::SetVisibleAtributes()
{
	m_Rod_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());//(m_Att0);
	m_AlThickPlate_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());//(m_Att1);
	m_AlThinPlate_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());//(m_Att1);
  
}

