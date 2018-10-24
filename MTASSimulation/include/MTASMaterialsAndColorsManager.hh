/**
 * Class is a singleton
 * @author 
 */

#ifndef MTASMaterialsAndColorsManager_h
#define MTASMaterialsAndColorsManager_h 1

class G4Material;
class G4MaterialPropertiesTable;
class G4VisAttributes;

class MTASMaterialsAndColorsManager
{
private:
	MTASMaterialsAndColorsManager();
	~MTASMaterialsAndColorsManager();
	static MTASMaterialsAndColorsManager *s_instance;
	
	G4Material* m_Air;
	G4Material* m_Vacuum;
	G4Material* m_Aluminium;
	G4Material* m_NaI;
	G4Material* m_AluminiumOxide;
	G4Material* m_CarbonFiber;
	G4Material* m_SiliconPutty;
	G4Material* m_Teflon;
	G4Material* m_Steel;
	G4Material* m_Lead;
	G4Material* m_Cadmium;
	G4Material* m_Paraffin;
	G4Material* m_Borax;
	G4Material* m_BC408;
	G4Material* m_MDPE;
	G4Material* m_Silicon;
	G4Material* m_Germanium;
	G4Material* m_MagnesiumOxide;
	G4Material* m_Polyethylene;
	G4Material* m_CalciumPeroxide;
	G4Material* m_BoronTrioxide;
	G4Material* m_SodiumOxide;
	G4Material* m_GlassFiber;
	G4Material* m_SiliconDioxide;
	G4Material* m_Derlin;
	G4Material* m_Copper;
	G4Material* m_IronOxide;
	G4Material* m_Silver;
		
	G4VisAttributes* m_Green;
	G4VisAttributes* m_Grey;
	G4VisAttributes* m_Yellow;
	G4VisAttributes* m_White;
	G4VisAttributes* m_Black;
	G4VisAttributes* m_Red;
	G4VisAttributes* m_Blue;
		
public:

	G4Material* GetAir() { return m_Air;}
	G4Material* GetVaccum() { return m_Vacuum;}
	G4Material* GetAluminium() { return m_Aluminium;}
	G4Material* GetNaI() { return m_NaI;}
	G4Material* GetAluminiumOxide() { return m_AluminiumOxide;}
	G4Material* GetCarbonFiber() { return m_CarbonFiber;}
	G4Material* GetSiliconPutty() { return m_SiliconPutty;}
	G4Material* GetTeflon() { return m_Teflon;}
	G4Material* GetSteel() { return m_Steel;}
	G4Material* GetLead() { return m_Lead;}
	G4Material* GetCadmium() { return m_Cadmium;}
	G4Material* GetParaffin() { return m_Paraffin;}
	G4Material* GetBorax() { return m_Borax;}
	G4Material* GetBC408() { return m_BC408;}
	G4Material* GetMDPE() { return m_MDPE;}
	G4Material* GetSilicon() { return m_Silicon;}
	G4Material* GetGermanium() { return m_Germanium;}
	G4Material* GetMagnesiumOxide() { return m_MagnesiumOxide;}
	G4Material* GetPolyethylene() { return m_Polyethylene;}
	G4Material* GetCalciumPeroxide() { return m_CalciumPeroxide;}
	G4Material* GetBoronTrioxide() { return m_BoronTrioxide;}
	G4Material* GetSodiumOxide() { return m_SodiumOxide;}
	G4Material* GetGlassFiber() { return m_GlassFiber;}
	G4Material* GetSiliconDioxide() { return m_SiliconDioxide;}
	G4Material* GetDerlin() {return m_Derlin;}
	G4Material* GetCopper() {return m_Copper;}
	G4Material* GetIronOxide() {return m_IronOxide;}
	G4Material* GetSilver() {return m_Silver;}
	    
	G4VisAttributes* GetGreen() { return m_Green;}
	G4VisAttributes* GetGrey() { return m_Grey;}
	G4VisAttributes* GetYellow() { return m_Yellow;}
	G4VisAttributes* GetWhite() { return m_White;}
	G4VisAttributes* GetBlack() { return m_Black;}
	G4VisAttributes* GetRed() { return m_Red;}
	G4VisAttributes* GetBlue() { return m_Blue;}

	static MTASMaterialsAndColorsManager *GetInstance()
	{
		if (!s_instance)
			s_instance = new MTASMaterialsAndColorsManager;
		return s_instance;
	}

};


	
#endif

