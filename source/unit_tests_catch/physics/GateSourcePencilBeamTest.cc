#include <catch.hpp>
#include <GateSourcePencilBeam.hh>
#include <G4SystemOfUnits.hh>
#include <G4Event.hh>
#include <GateRunManager.hh>
#include <GatePhysicsList.hh>

TEST_CASE("pencil beam test","[example][physics]"){

  INFO("create geant4 environment for particle sources");
  auto runManager = new GateRunManager;
  runManager->SetUserInitialization( GatePhysicsList::GetInstance() );

  INFO("create pencil beam source");
  auto gpb = new GateSourcePencilBeam("test",false);

  INFO("set kinematics");
  gpb->SetIonParameter("proton");
  gpb->SetPosition(G4ThreeVector(0.,0.,0.));
  gpb->SetSigmaX(1. * mm);
  gpb->SetSigmaY(1. * mm);
  gpb->SetSigmaTheta(1. * mrad);
  gpb->SetSigmaPhi(1. * mrad);
  gpb->SetEllipseXThetaArea(1.*mm*mrad);
  gpb->SetEllipseYPhiArea(1.*mm*mrad);
  gpb->SetEnergy(100 * MeV);
  gpb->SetSigmaEnergy(0.1 * MeV);

  INFO("create event");
  auto event = new G4Event;

  INFO("generate primaries");
  G4int nret = gpb->GeneratePrimaries(event);

  INFO("number of generated primaries should be 1");
  REQUIRE(nret == 1);

  INFO("number of primaries in event should also be 1");
  G4int nprim = event->GetNumberOfPrimaryVertex();
  REQUIRE(nprim == 1);

  INFO("energy should be 100 MeV");
  double Ekin = event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
  CHECK(Ekin/MeV == Approx(100.).margin(1.0));
}
