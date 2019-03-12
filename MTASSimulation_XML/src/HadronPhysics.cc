
#include "HadronPhysics.hh"
#include "G4ProcessManager.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"

HadronPhysics::HadronPhysics(const G4String& name)
                     :  G4VPhysicsConstructor(name) {}



HadronPhysics::~HadronPhysics() {
  //fDecayProcess = NULL;
}


void HadronPhysics::ConstructParticle()
{

}

void HadronPhysics::ConstructProcess()
{
    G4HadronElasticPhysics* hadronElasticPhys = new G4HadronElasticPhysics();
    hadronElasticPhys -> ConstructProcess();
    G4HadronPhysicsQGSP_BIC* hadronPhysQGSP_BIC = new G4HadronPhysicsQGSP_BIC();
    hadronPhysQGSP_BIC-> ConstructProcess();

}


