#include "FastSimValidation/Analyzer_MiniAOD/interface/ElectronAnalyzer_MiniAOD.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// Geometry
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"


#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>

//
// -------------------------------------- Constructor --------------------------------------------
//
ElectronAnalyzer_MiniAOD::ElectronAnalyzer_MiniAOD(const edm::ParameterSet& ps)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "Constructor  ElectronAnalyzer_MiniAOD::ElectronAnalyzer_MiniAOD " << std::endl;

  //fullSim_ = ps.getUntrackedParameter<bool>("fullSim");
  
  // Get parameters from configuration file
  theElectronCollection_         = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("ElectronCollection"));
  theGenParticleCollection_  = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenElectronCollection"));

  // debug
  debug_ = ps.getUntrackedParameter<bool>("Debug");

 
}

//
// -- Destructor
//
ElectronAnalyzer_MiniAOD::~ElectronAnalyzer_MiniAOD()
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "Destructor ElectronAnalyzer_MiniAOD::~ElectronAnalyzer_MiniAOD " << std::endl;
}

//
// -------------------------------------- beginRun --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::dqmBeginRun(edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::beginRun" << std::endl;
}
//
// -------------------------------------- bookHistos --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::bookHistograms(DQMStore::IBooker & ibooker_, edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::bookHistograms" << std::endl;
  
  //book at beginRun
  bookHistos(ibooker_);
}
//
// -------------------------------------- beginLuminosityBlock --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                      edm::EventSetup const& context) 
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::beginLuminosityBlock" << std::endl;
}


//
// -------------------------------------- Analyze --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::analyze(edm::Event const& e, edm::EventSetup const& eSetup)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::analyze" << std::endl;

  //-------------------------------
  //--- Get Collections
  //-------------------------------

  // RecoElectrons
  edm::Handle<reco::CandidateCollection> ElectronCollection;
  e.getByToken (theElectronCollection_,ElectronCollection);
  if ( !ElectronCollection.isValid() ) 
  {
    edm::LogError ("ElectronAnalyzer_MiniAOD") << "invalid collection: Electrons" << "\n";
    return;
  }

  std::vector<const pat::Electron*> patElectrons;
  for (reco::CandidateCollection::const_iterator i_recoElectron = ElectronCollection->begin(); i_recoElectron != ElectronCollection->end(); ++i_recoElectron)
  {
    try{
      patElectrons.push_back(dynamic_cast<const pat::Electron*>(&(*i_recoElectron)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection recoElectrons not of type pat::Electron" << std::endl;
    }
  }

  // GenElectrons
  edm::Handle<reco::CandidateCollection> genParticleCollection;
  e.getByToken(theGenParticleCollection_, genParticleCollection);
  if ( !genParticleCollection.isValid() ) 
  {
    edm::LogError ("ElectronAnalyzer_MiniAOD") << "invalid collection: genParticles" << "\n";
    return;
  }

  std::vector<const pat::PackedGenParticle*> genElectrons;
  for (reco::CandidateCollection::const_iterator i_genElectron = genParticleCollection->begin(); i_genElectron != genParticleCollection->end(); ++i_genElectron)
  {
    try{
      genElectrons.push_back(dynamic_cast<const pat::PackedGenParticle*>(&(*i_genElectron)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection genElectrons not of type pat::PackedGenParticle" << std::endl;
    }
  }


  //-------------------------------
  //--- Create Subsets
  //-------------------------------
  // veto/loose/medium/tight ID
  std::vector<const pat::Electron*> vetoElectrons;
  std::vector<const pat::Electron*> looseElectrons;
  std::vector<const pat::Electron*> mediumElectrons;
  std::vector<const pat::Electron*> tightElectrons;

  for (std::vector<const pat::Electron *>::const_iterator i_patElectron = patElectrons.begin(); i_patElectron != patElectrons.end(); ++i_patElectron)
  {
    const pat::Electron* i_patElectronPtr = *i_patElectron;

    if(debug_)
    for(std::vector<std::pair<std::string,float>>::const_iterator i_elIDs = i_patElectronPtr->electronIDs().begin(); i_elIDs != i_patElectronPtr->electronIDs().end(); ++i_elIDs){
      if(debug_) std::cout << i_elIDs->first << std::endl;
    }

      /* Content of electronIDs()
      cutBasedElectronID-CSA14-50ns-V1-standalone-loose
      cutBasedElectronID-CSA14-50ns-V1-standalone-medium
      cutBasedElectronID-CSA14-50ns-V1-standalone-tight
      cutBasedElectronID-CSA14-50ns-V1-standalone-veto
      cutBasedElectronID-CSA14-PU20bx25-V0-standalone-loose
      cutBasedElectronID-CSA14-PU20bx25-V0-standalone-medium
      cutBasedElectronID-CSA14-PU20bx25-V0-standalone-tight
      cutBasedElectronID-CSA14-PU20bx25-V0-standalone-veto
      eidLoose
      eidRobustHighEnergy
      eidRobustLoose
      eidRobustTight
      eidTight
      heepElectronID-HEEPV50-CSA14-25ns
      heepElectronID-HEEPV50-CSA14-startup
      */

      if(i_patElectronPtr->electronID("cutBasedElectronID-CSA14-PU20bx25-V0-standalone-veto") > 0.5) vetoElectrons.push_back(i_patElectronPtr);
      if(i_patElectronPtr->electronID("cutBasedElectronID-CSA14-PU20bx25-V0-standalone-loose") > 0.5) looseElectrons.push_back(i_patElectronPtr);
      if(i_patElectronPtr->electronID("cutBasedElectronID-CSA14-PU20bx25-V0-standalone-medium") > 0.5) mediumElectrons.push_back(i_patElectronPtr);
      if(i_patElectronPtr->electronID("cutBasedElectronID-CSA14-PU20bx25-V0-standalone-tight") > 0.5) tightElectrons.push_back(i_patElectronPtr);
  }


  //-------------------------------
  // Fill Histrograms
  //-------------------------------

  // Veto
  for (std::vector<const pat::Electron *>::const_iterator i_vetoElectron = vetoElectrons.begin(); i_vetoElectron != vetoElectrons.end(); ++i_vetoElectron)
  {
    const pat::Electron* i_vetoElectronPtr = *i_vetoElectron;
    
    // Match Gen <-> Reco
    const pat::PackedGenParticle* matchedGenElectron = NULL;
    for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genElectron = genElectrons.begin(); i_genElectron != genElectrons.end(); ++i_genElectron) {
        if(deltaR(*i_vetoElectronPtr, **i_genElectron) < 0.1) matchedGenElectron = (*i_genElectron);
    }
    if(!matchedGenElectron) continue;
	
  	h_VetoID_PtvsrecoElectron->Fill(matchedGenElectron->pt());
  	h_VetoID_EtavsrecoElectron->Fill(matchedGenElectron->eta());

    h_VetoID_TruePtvsFracPtTruePt->Fill(matchedGenElectron->pt(), i_vetoElectronPtr->pt() / matchedGenElectron->pt());
    h_VetoID_EtavsFracPtTruePt->Fill(matchedGenElectron->eta(), i_vetoElectronPtr->pt() / matchedGenElectron->pt());
  }

  // Loose
  for (std::vector<const pat::Electron *>::const_iterator i_looseElectron = looseElectrons.begin(); i_looseElectron != looseElectrons.end(); ++i_looseElectron)
  {
    const pat::Electron* i_looseElectronPtr = *i_looseElectron;
    
    // Match Gen <-> Reco
    const pat::PackedGenParticle* matchedGenElectron = NULL;
    for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genElectron = genElectrons.begin(); i_genElectron != genElectrons.end(); ++i_genElectron) {
        if(deltaR(*i_looseElectronPtr, **i_genElectron) < 0.1) matchedGenElectron = (*i_genElectron);
    }
    if(!matchedGenElectron) continue;
  
    h_LooseID_PtvsrecoElectron->Fill(matchedGenElectron->pt());
    h_LooseID_EtavsrecoElectron->Fill(matchedGenElectron->eta());

    h_LooseID_TruePtvsFracPtTruePt->Fill(matchedGenElectron->pt(), i_looseElectronPtr->pt() / matchedGenElectron->pt());
    h_LooseID_EtavsFracPtTruePt->Fill(matchedGenElectron->eta(), i_looseElectronPtr->pt() / matchedGenElectron->pt());
  }

  // Medium
  for (std::vector<const pat::Electron *>::const_iterator i_mediumElectron = mediumElectrons.begin(); i_mediumElectron != mediumElectrons.end(); ++i_mediumElectron)
  {
    const pat::Electron* i_mediumElectronPtr = *i_mediumElectron;
    
    // Match Gen <-> Reco
    const pat::PackedGenParticle* matchedGenElectron = NULL;
    for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genElectron = genElectrons.begin(); i_genElectron != genElectrons.end(); ++i_genElectron) {
        if(deltaR(*i_mediumElectronPtr, **i_genElectron) < 0.1) matchedGenElectron = (*i_genElectron);
    }
    if(!matchedGenElectron) continue;
  
    h_MediumID_PtvsrecoElectron->Fill(matchedGenElectron->pt());
    h_MediumID_EtavsrecoElectron->Fill(matchedGenElectron->eta());

    h_MediumID_TruePtvsFracPtTruePt->Fill(matchedGenElectron->pt(), i_mediumElectronPtr->pt() / matchedGenElectron->pt());
    h_MediumID_EtavsFracPtTruePt->Fill(matchedGenElectron->eta(), i_mediumElectronPtr->pt() / matchedGenElectron->pt());
  }

  // Tight
  for (std::vector<const pat::Electron *>::const_iterator i_tightElectron = tightElectrons.begin(); i_tightElectron != tightElectrons.end(); ++i_tightElectron)
  {
    const pat::Electron* i_tightElectronPtr = *i_tightElectron;
    
    // Match Gen <-> Reco
    const pat::PackedGenParticle* matchedGenElectron = NULL;
    for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genElectron = genElectrons.begin(); i_genElectron != genElectrons.end(); ++i_genElectron) {
        if(deltaR(*i_tightElectronPtr, **i_genElectron) < 0.1) matchedGenElectron = (*i_genElectron);
    }
    if(!matchedGenElectron) continue;
	
  	h_TightID_PtvsrecoElectron->Fill(matchedGenElectron->pt());
  	h_TightID_EtavsrecoElectron->Fill(matchedGenElectron->eta());

    h_TightID_TruePtvsFracPtTruePt->Fill(matchedGenElectron->pt(), i_tightElectronPtr->pt() / matchedGenElectron->pt());
    h_TightID_EtavsFracPtTruePt->Fill(matchedGenElectron->eta(), i_tightElectronPtr->pt() / matchedGenElectron->pt());
  }

  // Gen
  for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genElectron = genElectrons.begin(); i_genElectron != genElectrons.end(); ++i_genElectron) 
  {
    h_PtvsgenElectron->Fill((*i_genElectron)->pt());
    h_EtavsgenElectron->Fill((*i_genElectron)->eta());
  }


  if(debug_ && genElectrons.size() > 0) std::cout << genElectrons.size() << "; " << looseElectrons.size() << "; " << tightElectrons.size() << std::endl;


}
//
// -------------------------------------- endLuminosityBlock --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::endLuminosityBlock" << std::endl;
}


//
// -------------------------------------- endRun --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::endRun" << std::endl;
}


//
// -------------------------------------- book histograms --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::bookHistos(DQMStore::IBooker & ibooker_)
{
  ibooker_.cd();
  ibooker_.setCurrentFolder("Electron");


  h_VetoID_TruePtvsFracPtTruePt = ibooker_.book2D("VetoID_TruePtvsFracPtTruePt", "TruePt vs Pt / TruePt for Veto ID", 50,0.,500., 20, 0.8, 1.2);
  h_VetoID_EtavsFracPtTruePt = ibooker_.book2D("VetoID_EtavsFracPtTruePt", "Eta vs Pt / TruePt for Veto ID", 50,-5.,5., 20, 0.8, 1.2);

  h_LooseID_TruePtvsFracPtTruePt = ibooker_.book2D("LooseID_TruePtvsFracPtTruePt", "TruePt vs Pt / TruePt for Loose ID", 50,0.,500., 20, 0.8, 1.2);
  h_LooseID_EtavsFracPtTruePt = ibooker_.book2D("LooseID_EtavsFracPtTruePt", "Eta vs Pt / TruePt for Loose ID", 50,-5.,5., 20, 0.8, 1.2);
  
  h_MediumID_TruePtvsFracPtTruePt = ibooker_.book2D("MediumID_TruePtvsFracPtTruePt", "TruePt vs Pt / TruePt for Medium ID", 50,0.,500., 20, 0.8, 1.2);
  h_MediumID_EtavsFracPtTruePt = ibooker_.book2D("MediumID_EtavsFracPtTruePt", "Eta vs Pt / TruePt for Medium ID", 50,-5.,5., 20, 0.8, 1.2);

  h_TightID_TruePtvsFracPtTruePt = ibooker_.book2D("TightID_TruePtvsFracPtTruePt", "TruePt vs Pt / TruePt for Tight ID", 50,0.,500., 20, 0.8, 1.2);
  h_TightID_EtavsFracPtTruePt = ibooker_.book2D("TightID_EtavsFracPtTruePt", "Eta vs Pt / TruePt for Tight ID", 50,-5.,5., 20, 0.8, 1.2);

  ibooker_.setCurrentFolder("Electron/Helpers");
  
  h_PtvsgenElectron = ibooker_.book1D("PtvsgenElectron","# genElectrons vs pt",50,0.,500.);
  h_EtavsgenElectron = ibooker_.book1D("EtavsgenElectron","# genElectrons vs eta",50,-5.,5.);

  h_VetoID_PtvsrecoElectron = ibooker_.book1D("VetoID_PtvsrecoElectron","# recoElectrons vs pt for Veto ID",50,0.,500.);
  h_VetoID_EtavsrecoElectron = ibooker_.book1D("VetoID_EtavsrecoElectron","# recoElectrons vs eta for Veto ID",50,-5.,5.);
  h_LooseID_PtvsrecoElectron = ibooker_.book1D("LooseID_PtvsrecoElectron","# recoElectrons vs pt for Loose ID",50,0.,500.);
  h_LooseID_EtavsrecoElectron = ibooker_.book1D("LooseID_EtavsrecoElectron","# recoElectrons vs eta for Loose ID",50,-5.,5.);
  h_MediumID_PtvsrecoElectron = ibooker_.book1D("MediumID_PtvsrecoElectron","# recoElectrons vs pt for Medium ID",50,0.,500.);
  h_MediumID_EtavsrecoElectron = ibooker_.book1D("MediumID_EtavsrecoElectron","# recoElectrons vs eta for Medium ID",50,-5.,5.);
  h_TightID_PtvsrecoElectron = ibooker_.book1D("TightID_PtvsrecoElectron","# recoElectrons vs pt for Tight ID",50,0.,500.);
  h_TightID_EtavsrecoElectron = ibooker_.book1D("TightID_EtavsrecoElectron","# recoElectrons vs eta for Tight ID",50,-5.,5.);
  
  ibooker_.cd();  

}


//
// -------------------------------------- functions --------------------------------------------
//

