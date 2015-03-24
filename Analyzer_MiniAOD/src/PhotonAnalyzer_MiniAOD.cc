#include "FastSimValidation/Analyzer_MiniAOD/interface/PhotonAnalyzer_MiniAOD.h"
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
PhotonAnalyzer_MiniAOD::PhotonAnalyzer_MiniAOD(const edm::ParameterSet& ps)
{
  edm::LogInfo("PhotonAnalyzer_MiniAOD") <<  "Constructor  PhotonAnalyzer_MiniAOD::PhotonAnalyzer_MiniAOD " << std::endl;

  //fullSim_ = ps.getUntrackedParameter<bool>("fullSim");
  
  // Get parameters from configuration file
  thePhotonCollection_         = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("PhotonCollection"));
  theGenParticleCollection_  = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenPhotonCollection"));

  // debug
  debug_ = ps.getUntrackedParameter<bool>("Debug");

 
}

//
// -- Destructor
//
PhotonAnalyzer_MiniAOD::~PhotonAnalyzer_MiniAOD()
{
  edm::LogInfo("PhotonAnalyzer_MiniAOD") <<  "Destructor PhotonAnalyzer_MiniAOD::~PhotonAnalyzer_MiniAOD " << std::endl;
}

//
// -------------------------------------- beginRun --------------------------------------------
//
void PhotonAnalyzer_MiniAOD::dqmBeginRun(edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("PhotonAnalyzer_MiniAOD") <<  "PhotonAnalyzer_MiniAOD::beginRun" << std::endl;
}
//
// -------------------------------------- bookHistos --------------------------------------------
//
void PhotonAnalyzer_MiniAOD::bookHistograms(DQMStore::IBooker & ibooker_, edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("PhotonAnalyzer_MiniAOD") <<  "PhotonAnalyzer_MiniAOD::bookHistograms" << std::endl;
  
  //book at beginRun
  bookHistos(ibooker_);
}
//
// -------------------------------------- beginLuminosityBlock --------------------------------------------
//
void PhotonAnalyzer_MiniAOD::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                      edm::EventSetup const& context) 
{
  edm::LogInfo("PhotonAnalyzer_MiniAOD") <<  "PhotonAnalyzer_MiniAOD::beginLuminosityBlock" << std::endl;
}


//
// -------------------------------------- Analyze --------------------------------------------
//
void PhotonAnalyzer_MiniAOD::analyze(edm::Event const& e, edm::EventSetup const& eSetup)
{
  edm::LogInfo("PhotonAnalyzer_MiniAOD") <<  "PhotonAnalyzer_MiniAOD::analyze" << std::endl;

  //-------------------------------
  //--- Get Collections
  //-------------------------------

  // RecoPhotons
  edm::Handle<reco::CandidateCollection> PhotonCollection;
  e.getByToken (thePhotonCollection_,PhotonCollection);
  if ( !PhotonCollection.isValid() ) 
  {
    edm::LogError ("PhotonAnalyzer_MiniAOD") << "invalid collection: Photons" << "\n";
    return;
  }

  std::vector<const pat::Photon*> patPhotons;
  for (reco::CandidateCollection::const_iterator i_recoPhoton = PhotonCollection->begin(); i_recoPhoton != PhotonCollection->end(); ++i_recoPhoton)
  {
    try{
      patPhotons.push_back(dynamic_cast<const pat::Photon*>(&(*i_recoPhoton)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection recoPhotons not of type pat::Photon" << std::endl;
    }
  }

  // GenPhotons
  edm::Handle<reco::CandidateCollection> genParticleCollection;
  e.getByToken(theGenParticleCollection_, genParticleCollection);
  if ( !genParticleCollection.isValid() ) 
  {
    edm::LogError ("PhotonAnalyzer_MiniAOD") << "invalid collection: genParticles" << "\n";
    return;
  }

  std::vector<const pat::PackedGenParticle*> genPhotons;
  for (reco::CandidateCollection::const_iterator i_genPhoton = genParticleCollection->begin(); i_genPhoton != genParticleCollection->end(); ++i_genPhoton)
  {
    try{
      genPhotons.push_back(dynamic_cast<const pat::PackedGenParticle*>(&(*i_genPhoton)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection genPhotons not of type pat::PackedGenParticle" << std::endl;
    }
  }


  //-------------------------------
  //--- Create Subsets
  //-------------------------------
  // veto/loose/medium/tight ID
  std::vector<const pat::Photon*> vetoPhotons;
  std::vector<const pat::Photon*> loosePhotons;
  std::vector<const pat::Photon*> mediumPhotons;
  std::vector<const pat::Photon*> tightPhotons;

  for (std::vector<const pat::Photon *>::const_iterator i_patPhoton = patPhotons.begin(); i_patPhoton != patPhotons.end(); ++i_patPhoton)
  {
    const pat::Photon* i_patPhotonPtr = *i_patPhoton;

    if(debug_)
    for(std::vector<std::pair<std::string,bool>>::const_iterator i_phIDs = i_patPhotonPtr->photonIDs().begin(); i_phIDs != i_patPhotonPtr->photonIDs().end(); ++i_phIDs){
      //std::cout << i_phIDs->first << std::endl;
    }

      /* Content of photonIDs()
      PhotonCutBasedIDLoose
      PhotonCutBasedIDTight
      */

    if(i_patPhotonPtr->photonID("PhotonCutBasedIDLoose")) loosePhotons.push_back(i_patPhotonPtr);
    //if(i_patPhotonPtr->photonID("cutBasedPhotonID-CSA14-PU20bx25-V0-standalone-medium") > 0.5) mediumPhotons.push_back(i_patPhotonPtr);
    if(i_patPhotonPtr->photonID("PhotonCutBasedIDTight")) tightPhotons.push_back(i_patPhotonPtr);
  }


  //-------------------------------
  // Fill Histrograms
  //-------------------------------

  // Loose
  for (std::vector<const pat::Photon *>::const_iterator i_loosePhoton = loosePhotons.begin(); i_loosePhoton != loosePhotons.end(); ++i_loosePhoton)
  {
    const pat::Photon* i_loosePhotonPtr = *i_loosePhoton;
    
    // Match Gen <-> Reco
    const pat::PackedGenParticle* matchedGenPhoton = NULL;
    for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genPhoton = genPhotons.begin(); i_genPhoton != genPhotons.end(); ++i_genPhoton) {
        if(deltaR(*i_loosePhotonPtr, **i_genPhoton) < 0.3) matchedGenPhoton = (*i_genPhoton);
    }

    if(!matchedGenPhoton){ continue; }
  
    h_LooseID_PtvsrecoPhoton->Fill(matchedGenPhoton->pt());
    h_LooseID_EtavsrecoPhoton->Fill(matchedGenPhoton->eta());

    h_LooseID_TruePtvsFracPtTruePt->Fill(matchedGenPhoton->pt(), i_loosePhotonPtr->pt() / matchedGenPhoton->pt());
    h_LooseID_EtavsFracPtTruePt->Fill(matchedGenPhoton->eta(), i_loosePhotonPtr->pt() / matchedGenPhoton->pt());
  }

/*  // Medium
  for (std::vector<const pat::Photon *>::const_iterator i_mediumPhoton = mediumPhotons.begin(); i_mediumPhoton != mediumPhotons.end(); ++i_mediumPhoton)
  {
    const pat::Photon* i_mediumPhotonPtr = *i_mediumPhoton;
    
    // Match Gen <-> Reco
    const pat::PackedGenParticle* matchedGenPhoton = NULL;
    for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genPhoton = genPhotons.begin(); i_genPhoton != genPhotons.end(); ++i_genPhoton) {
        if(deltaR(*i_mediumPhotonPtr, **i_genPhoton) < 0.3) matchedGenPhoton = (*i_genPhoton);
    }
    if(!matchedGenPhoton) continue;
  
    h_MediumID_PtvsrecoPhoton->Fill(matchedGenPhoton->pt());
    h_MediumID_EtavsrecoPhoton->Fill(matchedGenPhoton->eta());

    h_MediumID_TruePtvsFracPtTruePt->Fill(matchedGenPhoton->pt(), i_mediumPhotonPtr->pt() / matchedGenPhoton->pt());
    h_MediumID_EtavsFracPtTruePt->Fill(matchedGenPhoton->eta(), i_mediumPhotonPtr->pt() / matchedGenPhoton->pt());
  }
*/
  // Tight
  for (std::vector<const pat::Photon *>::const_iterator i_tightPhoton = tightPhotons.begin(); i_tightPhoton != tightPhotons.end(); ++i_tightPhoton)
  {
    const pat::Photon* i_tightPhotonPtr = *i_tightPhoton;
    
    // Match Gen <-> Reco
    const pat::PackedGenParticle* matchedGenPhoton = NULL;
    for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genPhoton = genPhotons.begin(); i_genPhoton != genPhotons.end(); ++i_genPhoton) {
        if(deltaR(*i_tightPhotonPtr, **i_genPhoton) < 0.3) matchedGenPhoton = (*i_genPhoton);
    }
    if(!matchedGenPhoton) continue;
	
  	h_TightID_PtvsrecoPhoton->Fill(matchedGenPhoton->pt());
  	h_TightID_EtavsrecoPhoton->Fill(matchedGenPhoton->eta());

    h_TightID_TruePtvsFracPtTruePt->Fill(matchedGenPhoton->pt(), i_tightPhotonPtr->pt() / matchedGenPhoton->pt());
    h_TightID_EtavsFracPtTruePt->Fill(matchedGenPhoton->eta(), i_tightPhotonPtr->pt() / matchedGenPhoton->pt());
  }

  // Gen
  for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genPhoton = genPhotons.begin(); i_genPhoton != genPhotons.end(); ++i_genPhoton) 
  {
    h_PtvsgenPhoton->Fill((*i_genPhoton)->pt());
    h_EtavsgenPhoton->Fill((*i_genPhoton)->eta());
  }


  if(debug_ && genPhotons.size() > 0) std::cout << genPhotons.size() << "; " << patPhotons.size() << "; " << loosePhotons.size() << "; " << tightPhotons.size() << std::endl;


}
//
// -------------------------------------- endLuminosityBlock --------------------------------------------
//
void PhotonAnalyzer_MiniAOD::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
  edm::LogInfo("PhotonAnalyzer_MiniAOD") <<  "PhotonAnalyzer_MiniAOD::endLuminosityBlock" << std::endl;
}


//
// -------------------------------------- endRun --------------------------------------------
//
void PhotonAnalyzer_MiniAOD::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
  edm::LogInfo("PhotonAnalyzer_MiniAOD") <<  "PhotonAnalyzer_MiniAOD::endRun" << std::endl;
}


//
// -------------------------------------- book histograms --------------------------------------------
//
void PhotonAnalyzer_MiniAOD::bookHistos(DQMStore::IBooker & ibooker_)
{
  ibooker_.cd();
  ibooker_.setCurrentFolder("Photon");

  h_LooseID_TruePtvsFracPtTruePt = ibooker_.book2D("LooseID_TruePtvsFracPtTruePt", "TruePt vs Pt / TruePt for Loose ID", 50,0.,500., 20, 0.8, 1.2);
  h_LooseID_EtavsFracPtTruePt = ibooker_.book2D("LooseID_EtavsFracPtTruePt", "Eta vs Pt / TruePt for Loose ID", 50,-5.,5., 20, 0.8, 1.2);
  
//  h_MediumID_TruePtvsFracPtTruePt = ibooker_.book2D("MediumID_TruePtvsFracPtTruePt", "TruePt vs Pt / TruePt for Medium ID", 50,0.,500., 20, 0.8, 1.2);
//  h_MediumID_EtavsFracPtTruePt = ibooker_.book2D("MediumID_EtavsFracPtTruePt", "Eta vs Pt / TruePt for Medium ID", 50,-5.,5., 20, 0.8, 1.2);

  h_TightID_TruePtvsFracPtTruePt = ibooker_.book2D("TightID_TruePtvsFracPtTruePt", "TruePt vs Pt / TruePt for Tight ID", 50,0.,500., 20, 0.8, 1.2);
  h_TightID_EtavsFracPtTruePt = ibooker_.book2D("TightID_EtavsFracPtTruePt", "Eta vs Pt / TruePt for Tight ID", 50,-5.,5., 20, 0.8, 1.2);

  ibooker_.setCurrentFolder("Photon/Helpers");
  
  h_PtvsgenPhoton = ibooker_.book1D("PtvsgenPhoton","# genPhotons vs pt",50,0.,500.);
  h_EtavsgenPhoton = ibooker_.book1D("EtavsgenPhoton","# genPhotons vs eta",50,-5.,5.);

  h_LooseID_PtvsrecoPhoton = ibooker_.book1D("LooseID_PtvsrecoPhoton","# recoPhotons vs pt for Loose ID",50,0.,500.);
  h_LooseID_EtavsrecoPhoton = ibooker_.book1D("LooseID_EtavsrecoPhoton","# recoPhotons vs eta for Loose ID",50,-5.,5.);
//  h_MediumID_PtvsrecoPhoton = ibooker_.book1D("MediumID_PtvsrecoPhoton","# recoPhotons vs pt for Medium ID",50,0.,500.);
//  h_MediumID_EtavsrecoPhoton = ibooker_.book1D("MediumID_EtavsrecoPhoton","# recoPhotons vs eta for Medium ID",50,-5.,5.);
  h_TightID_PtvsrecoPhoton = ibooker_.book1D("TightID_PtvsrecoPhoton","# recoPhotons vs pt for Tight ID",50,0.,500.);
  h_TightID_EtavsrecoPhoton = ibooker_.book1D("TightID_EtavsrecoPhoton","# recoPhotons vs eta for Tight ID",50,-5.,5.);
  
  ibooker_.cd();  

}


//
// -------------------------------------- functions --------------------------------------------
//

