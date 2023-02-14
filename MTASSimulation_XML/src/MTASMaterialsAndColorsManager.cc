//Class included all needed materials and vis attributes


#include "G4Material.hh"

#include "G4MaterialTable.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "MTASMaterialsAndColorsManager.hh"
using namespace CLHEP;

MTASMaterialsAndColorsManager::MTASMaterialsAndColorsManager()
{
	G4double atomicMass;
	G4double z;
	G4double density;
	G4double fractionmass;
	G4int numberElements;
  
	// Definicje pierwiastków tworzących materiały
	G4Element* H  = new G4Element("Hydrogen", "H",  z=1.,  atomicMass =1.008 *g/mole );
	G4Element* B  = new G4Element("Boron",    "B",  z=5.,  atomicMass =10.81 *g/mole );
	G4Element* C  = new G4Element("Carbon",   "C",  z=6.,  atomicMass = 12.01*g/mole );
	G4Element* N  = new G4Element("Nitrogen", "N",  z=7.,  atomicMass = 14.01*g/mole );
	G4Element* O  = new G4Element("Oxygen"  , "O",  z=8.,  atomicMass = 16.00*g/mole );
	G4Element* F  = new G4Element("Fluorine", "F",  z=9.,  atomicMass = 18.998*g/mole );
	G4Element* Na = new G4Element("Sodium",   "Na", z=11., atomicMass = 22.99*g/mole );
	G4Element* Al = new G4Element("Aluminium","Al", z=13., atomicMass = 26.98*g/mole );
	G4Element* Ni = new G4Element("Nickel"  , "Ni", z=28., atomicMass = 58.69*g/mole );
	G4Element* Mn = new G4Element("Manganese","Mn", z=25., atomicMass = 54.93*g/mole );
	G4Element* Mo = new G4Element("Molybdenum","Mo", z=42., atomicMass = 95.95*g/mole );
	G4Element* Si = new G4Element("Silicon"  ,"Si", z=14., atomicMass = 28.09*g/mole );
	G4Element* P  = new G4Element("Phosphorus","P",  z=15.,atomicMass = 30.97*g/mole );
	G4Element* Cr = new G4Element("Chrome"  , "Cr", z=24., atomicMass = 51.00*g/mole );
	G4Element* Fe = new G4Element("Iron"	  , "Fe", z=26., atomicMass = 55.85*g/mole );
	G4Element* I  = new G4Element("Iodine",   "I",  z=53., atomicMass = 126.9*g/mole );
	G4Element* Pb = new G4Element("Lead",    "Pb",  z=82., atomicMass = 207.2*g/mole );
	G4Element* Cd = new G4Element("Cadmium", "Cd",  z=48., atomicMass = 112.4*g/mole );
	G4Element* Mg = new G4Element("Magnesium","Mg",  z=12.,atomicMass = 24.305*g/mole );
	G4Element* Ca = new G4Element("Calcium", "Ca",  z=20., atomicMass = 40.078*g/mole );
	G4Element* Cu = new G4Element("Copper",  "Cu",  z=29., atomicMass = 63.546*g/mole );
	G4Element* Ag = new G4Element("Silver", "Ag", z = 47., atomicMass = 107.868*g/mole);
	G4Element* S = new G4Element("Sulfur", "S", z = 16., atomicMass = 32.06*g/mole);
	  
	m_Vacuum = new G4Material("GalVacuum", z=1., atomicMass= 1.01*g/mole,
	density = universe_mean_density, kStateGas, 3.e-18*pascal, 2.73*kelvin);
/*	m_Air = new G4Material("Air", density= 1.29*mg/cm3, numberElements=2);
	m_Air->AddElement(N, 70*perCent);
	m_Air->AddElement(O, 30*perCent);*/
	
	m_Air = new G4Material("GalacticVacuum", z=1., atomicMass= 1.01*g/mole,
	density = universe_mean_density, kStateGas, 3.e-18*pascal, 2.73*kelvin);
	
	m_Aluminium = new G4Material( "Aluminium", density= 2.7*g/cm3, numberElements=1 );
	m_Aluminium->AddElement( Al, 1 );
	
	m_NaI = new G4Material( "SodiumIodide", density= 3.67*g/cm3, numberElements=2 );
	m_NaI->AddElement( Na, 1 );
	m_NaI->AddElement( I, 1 );
	
	m_AluminiumOxide = new G4Material( "AluminiumOxide", density= 3.9*g/cm3, numberElements=2 );
	m_AluminiumOxide->AddElement( Al, 2 );
	m_AluminiumOxide->AddElement( O, 3 );

	m_CarbonFiber = new G4Material( "CarbonFiber", z=6., atomicMass=12.01*g/mole, density=1.8*g/cm3 );
	m_SiliconPutty = new G4Material( "SiliconPutty", density= 1.0*g/cm3, numberElements=2 );
	m_SiliconPutty->AddElement( Si, 1 );
	m_SiliconPutty->AddElement( O, 2 );
  
	m_Teflon = new G4Material( "Teflon", density= 2.2*g/cm3, numberElements=2 );
	m_Teflon->AddElement( C, 2 );
	m_Teflon->AddElement( F, 4 );
  
	m_Steel =  new G4Material( "Steel", density= 8.0 *g/cm3, numberElements=10 );
	m_Steel->AddElement( C,  0.05 *perCent );
	m_Steel->AddElement( Si,  1.5 *perCent );
	m_Steel->AddElement( Mn,  1.5 *perCent );
	m_Steel->AddElement( P,  0.04 *perCent );
	m_Steel->AddElement( N,  0.11 *perCent );
	m_Steel->AddElement( Cr,  17 * perCent );
	m_Steel->AddElement( Ni,  6 *perCent );
	m_Steel->AddElement( Mo,  0.8 *perCent );
	m_Steel->AddElement( S,  0.015 *perCent );
	double FeFrac = 100 - 0.05 - 1.5 - 1.5 - 0.04 - 0.11 - 17 - 6 - 0.8 - 0.015;
	m_Steel->AddElement( Fe,  FeFrac *perCent );
  
	m_Lead = new G4Material( "Lead", density= 11.34 *g/cm3, numberElements=1 );
	m_Lead ->AddElement( Pb, 1 );
  
  
	m_Cadmium = new G4Material( "Cadmium", density= 8.65 *g/cm3, numberElements=1 );
	m_Cadmium ->AddElement( Cd, 1 );
  
	m_Paraffin = new G4Material( "Paraffin", density= 0.89 *g/cm3, numberElements=2 );
	m_Paraffin ->AddElement( C, 27 );
	m_Paraffin ->AddElement( H, 56 );
  
	m_Borax = new G4Material( "Borax", density= 1.73 *g/cm3, numberElements=4 );
	m_Borax ->AddElement( Na, 2 );
	m_Borax ->AddElement( B, 4 );
	m_Borax ->AddElement( O, 17 );
	m_Borax ->AddElement( H, 20 );
  
	m_BC408 = new G4Material( "BC408", density= 1.032 *g/cm3, numberElements=2 );
	m_BC408->AddElement( C,  1./(1.+1.104)*100 *perCent );
	m_BC408->AddElement( H,  1.104/(1.+1.104)*100 *perCent );
	
	m_MDPE = new G4Material( "MultiDensityPE", density= 0.926*g/cm3, numberElements=2 );
	m_MDPE->AddElement( C, 2 );
	m_MDPE->AddElement( H, 4 );

	//m_Silicon  = new G4Material( "Silicon", z=14., atomicMass=28.09*g/mole, density=23.3*g/cm3 );
	m_Silicon  = new G4Material( "Silicon", z=14., atomicMass=28.09*g/mole, density=2.33*g/cm3 );
	
	m_Germanium  = new G4Material( "Germanium", z=32., atomicMass=72.63*g/mole, density=5.323*g/cm3 );
	
	m_Copper = new G4Material ("Copper", density =8.94 *g/cm3 , numberElements=1);
	m_Copper->AddElement(Cu, 1);
	
	m_MagnesiumOxide = new G4Material ("MagnesiumOxide", density = 3.58 *g/cm3, numberElements = 2);
	m_MagnesiumOxide->AddElement( Mg, 1);
	m_MagnesiumOxide->AddElement( O, 1);
	
	m_Polyethylene = new G4Material ("Polyethylene", density = 0.94 *g/cm3, numberElements = 2);
	m_Polyethylene->AddElement(C, 2);
	m_Polyethylene->AddElement(H, 4);
	
	m_SiliconDioxide = new G4Material ("SiliconDioxide", density = 2.648 *g/cm3, numberElements = 2);//SiO2
	m_SiliconDioxide->AddElement(Si, 1);
	m_SiliconDioxide->AddElement(O, 2);
	
	m_AluminiumOxide = new G4Material ("AluminiumOxide", density = 4. *g/cm3, numberElements = 2);
	m_AluminiumOxide->AddElement(Al, 2);
	m_AluminiumOxide->AddElement(O, 3);

	m_CalciumPeroxide = new G4Material ("CalciumPeroxide", density = 2.91 *g/cm3, numberElements = 2);
	m_CalciumPeroxide->AddElement(Ca, 1);
	m_CalciumPeroxide->AddElement(O, 2);
	
	m_BoronTrioxide = new G4Material ("BoronTrioxide", density = 2.46 *g/cm3, numberElements = 2);
	m_BoronTrioxide->AddElement(B, 2);
	m_BoronTrioxide->AddElement(O, 3);
	
	m_SodiumOxide = new G4Material ("SodiumOxide", density = 2.27 *g/cm3, numberElements = 2);
	m_SodiumOxide->AddElement(Na, 2);
	m_SodiumOxide->AddElement(O, 1);

		
	m_GlassFiber = new G4Material("GlassFiber", density = 2.55 *g/cm3, numberElements = 5);
	m_GlassFiber->AddMaterial(m_SiliconDioxide, 54*perCent);
	m_GlassFiber->AddMaterial(m_AluminiumOxide, 14*perCent);
	m_GlassFiber->AddMaterial(m_CalciumPeroxide, 20*perCent);
	m_GlassFiber->AddMaterial(m_BoronTrioxide, 10*perCent);	
	m_GlassFiber->AddMaterial(m_SodiumOxide, 2*perCent);	
	
	m_Derlin = new G4Material("Derlin", density = 1.41 *g/cm3, numberElements = 3);
	m_Derlin->AddElement(C, 1);
	m_Derlin->AddElement(H, 2);
	m_Derlin->AddElement(O, 1);
	
	m_IronOxide = new G4Material(" ", density = 5.242 *g/cm3, numberElements = 2);
	m_IronOxide->AddElement(Fe, 2);
	m_IronOxide->AddElement(O, 3);
	
	m_Silver = new G4Material("Silver", density = 10.49 *g/cm3, numberElements = 1);
	m_Silver->AddElement(Ag, 1);
	
	// Test cable (MS October 2022)
	// Trying to improove Charlie vacuum/blob/cable
	// valume fractions: 0.35 poly, 0.2 copper, 0.45 aluminum
	// converts into mass fractions: 0.10 poly, 0.54 copper, 0.36 aluminium
	m_CableTest = new G4Material("CableTest", density = 0.95 *g/cm3, numberElements = 3);
	//m_CableTest->AddMaterial(m_Aluminium, fractionmass = 1.0);
	//m_CableTest->AddElement(Cu, 1);
	m_CableTest->AddMaterial(m_Polyethylene, fractionmass = 0.1);
	m_CableTest->AddMaterial(m_Copper, fractionmass = 0.54); 
	m_CableTest->AddMaterial(m_Aluminium, fractionmass = 0.36);
	
	m_LowDensityAluminium = new G4Material( "LowDensityAluminium", density= 0.95*g/cm3, numberElements=1 );
	m_LowDensityAluminium->AddElement( Al, 1 );
			
	m_Green= new G4VisAttributes(G4Colour(0.0,1.0,0.0));  //Green color for NaI volumes
	m_Green->SetVisibility(true);
	m_Grey= new G4VisAttributes(G4Colour(0.5,0.5,0.5));  //Grey color for Carbon and Alu volumes - layer0 and 1
	m_Grey->SetVisibility(true);
	m_Yellow= new G4VisAttributes(G4Colour(0.5,0.5,0.0));   //Yellow color for Si volume - layer 2
	m_Yellow->SetVisibility(true);
	m_White= new G4VisAttributes(G4Colour(1.0,1.0,1.0));   //White color for MgO or teflon volumes - layer 3
	m_White->SetVisibility(true);
	m_Black= new G4VisAttributes(G4Colour(0.0,0.0,0.0));   
	m_Black->SetVisibility(true);
	m_Red= new G4VisAttributes(G4Colour(1.0,0.0,0.0));   
	m_Red->SetVisibility(true);
	m_Blue= new G4VisAttributes(G4Colour(0.0,0.0,1.0));   
	m_Blue->SetVisibility(true);
	/*delete H;
	delete B;
	delete C;
	delete N;
	delete O;
	delete F;
	delete Na;
	delete Al;
	delete Ni;
	delete Mn;
	delete Si;
	delete P;
	delete Cr;
	delete Fe;
	delete I;
	delete Pb;
	delete Cd;*/
}

MTASMaterialsAndColorsManager::~MTASMaterialsAndColorsManager()
{
//materials
	if( m_Air != 0 )
		delete m_Air;
		
	if( m_Vacuum != 0 )
		delete m_Vacuum;
		
	if( m_Aluminium != 0 )
		delete m_Aluminium;
		
	if( m_NaI != 0 )
		delete m_NaI;
		
	if( m_AluminiumOxide != 0 )
		delete m_AluminiumOxide;

	if( m_CarbonFiber != 0 )
		delete m_CarbonFiber;
		
	if( m_SiliconPutty != 0 )
		delete m_SiliconPutty;
		
	if( m_Teflon != 0 )
		delete m_Teflon;
		
	if( m_Steel != 0 )
		delete m_Steel;	

 	if( m_Lead != 0 )
		delete m_Lead;
		
	if( m_Cadmium != 0 )
		delete m_Cadmium;
		
	if( m_Paraffin != 0 )
		delete m_Paraffin;
		
	if( m_Borax != 0 )
		delete m_Borax; 

	if( m_BC408 != 0 )
		delete m_BC408;

	if( m_Silicon != 0 )
		delete m_Silicon;

	if( m_Germanium != 0 )
		delete m_Germanium;
		
	if( m_MagnesiumOxide != 0 )
		delete m_MagnesiumOxide;
		
	if( m_Polyethylene != 0 )
		delete m_Polyethylene;
		
	if( m_CableTest != 0 )
		delete m_CableTest;
		
	if( m_LowDensityAluminium != 0 )
		delete m_LowDensityAluminium;
		
//colors
	 if( m_Green != 0 )
		delete m_Green;
		
	if( m_Grey != 0 )
		delete m_Grey;
		
	if( m_Yellow != 0 )
		delete m_Yellow;
		
	if( m_White != 0 )
		delete m_White; 

	if( m_Black != 0 )
		delete m_Black;
		
	if( m_Red != 0 )
		delete m_Red;
		
	if( m_Blue != 0 )
		delete m_Blue;
	
}

MTASMaterialsAndColorsManager *MTASMaterialsAndColorsManager::s_instance = 0;

