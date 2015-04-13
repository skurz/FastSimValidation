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
  theGenPhotonCollection_  = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenPhotonCollection"));
  thePhotonIDs_ = ps.getParameter<std::vector<edm::ParameterSet>>("PhotonIDs");

  theCollectionName_ = ps.getParameter<std::string>("CollectionName");


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
  e.getByToken(theGenPhotonCollection_, genParticleCollection);
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

  // PhotonIDs
  std::vector<std::string> idStrings;
  for (std::vector<edm::ParameterSet>::const_iterator it = thePhotonIDs_.begin(); it != thePhotonIDs_.end(); ++it) {
    idStrings.push_back(it->getParameter<std::string>("idString"));
  }


  //-------------------------------
  //--- Create Subsets
  //-------------------------------
  // Create subset for every ID
  std::vector<std::vector<const pat::Photon*>> idPhotons;

  for(std::vector<std::string>::const_iterator i_phID = idStrings.begin(); i_phID != idStrings.end(); ++i_phID){

    std::vector<const pat::Photon*> passedPhs;
    for (std::vector<const pat::Photon *>::const_iterator i_patPhoton = patPhotons.begin(); i_patPhoton != patPhotons.end(); ++i_patPhoton){
      if((*i_patPhoton)->photonID(*i_phID)){
        passedPhs.push_back((*i_patPhoton));
      }
    }

    idPhotons.push_back(passedPhs);
  }

  /*// MediumID not included (at least in MiniAOD)
    // Perform cuts manually?
    
    if(std::abs(i_patPhotonPtr->eta()) < 1.5){
      if(i_patPhotonPtr->passElectronVeto() && i_patPhotonPtr->hadTowOverEm() < 0.05 && i_patPhotonPtr->sigmaIetaIeta() < 0.012 && i_patPhotonPtr->chargedHadronIso() < 2.6 && i_patPhotonPtr->neutralHadronIso() < 3.5 + 0.04*i_patPhotonPtr->pt() && i_patPhotonPtr->photonIso() < 1.3 + 0.005*i_patPhotonPtr->pt()) loose.push_back(i_patPhotonPtr);
    }else
      if(i_patPhotonPtr->passElectronVeto() && i_patPhotonPtr->hadTowOverEm() < 0.05 && i_patPhotonPtr->sigmaIetaIeta() < 0.034 && i_patPhotonPtr->chargedHadronIso() < 2.3 && i_patPhotonPtr->neutralHadronIso() < 2.9 + 0.04*i_patPhotonPtr->pt()) loose.push_back(i_patPhotonPtr);
    //Use Rho-corrected Areas??
  */


  if(debug_)
  for (std::vector<const pat::Photon *>::const_iterator i_patPhoton = patPhotons.begin(); i_patPhoton != patPhotons.end(); ++i_patPhoton)
  {    
    for(std::vector<std::pair<std::string,bool>>::const_iterator i_phIDs = (*i_patPhoton)->photonIDs().begin(); i_phIDs != (*i_patPhoton)->photonIDs().end(); ++i_phIDs){
      //std::cout << i_phIDs->first << std::endl;
    }
  }


  //-------------------------------
  // Fill Histrograms
  //-------------------------------

  int histID = 0;
  for(std::vector<std::vector<const pat::Photon*>>::iterator i_idPhotons = idPhotons.begin(); i_idPhotons != idPhotons.end(); ++i_idPhotons){
    fillHisto(histID, &(*i_idPhotons), &genPhotons);
    //std::cout << (&(*it))->size() << std::endl;
    ++histID;
  }


  // Gen
  for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genPhoton = genPhotons.begin(); i_genPhoton != genPhotons.end(); ++i_genPhoton) 
  {
    h_truePt_genParticle->Fill((*i_genPhoton)->pt());
    h_trueEta_genParticle->Fill((*i_genPhoton)->eta());
  }

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
  std::vector<std::string> tagNamesShort;
  for (std::vector<edm::ParameterSet>::const_iterator it = thePhotonIDs_.begin(); it != thePhotonIDs_.end(); ++it) {
    tagNamesShort.push_back(it->getParameter<std::string>("idShortName"));
  }

  ibooker_.cd();

  ibooker_.setCurrentFolder(theCollectionName_+"/Gen");
  
  h_truePt_genParticle = ibooker_.book1D("truePt_gen","true pt vs total # genPhotons",50,0.,500.);
  h_trueEta_genParticle = ibooker_.book1D("trueEta_gen","true eta vs total # genPhotons",50,-5.,5.);


  int histoID = 0;
  for(std::vector<std::string>::const_iterator i_shortName = tagNamesShort.begin(); i_shortName != tagNamesShort.end(); ++i_shortName){
    ibooker_.setCurrentFolder(theCollectionName_+"/"+*i_shortName+"ID");


    h_truePt_pt[histoID] = ibooker_.book2D("truePt_vs_pt", "true pt vs (reco pt - true pt) / (true pt) for " + *i_shortName + " id", 50,0.,500., 50,-1.,1.);
    h_truePt_eta[histoID] = ibooker_.book2D("truePt_vs_eta", "true pt vs (reco eta - true eta) for " + *i_shortName + " id", 50,0.,500., 50,-0.5,0.5);
    h_trueEta_pt[histoID] = ibooker_.book2D("trueEta_vs_pt", "true eta vs (reco pt - true pt) / (true pt) for " + *i_shortName + " id", 50,-5.,5., 50,-1.,1.);
    h_trueEta_eta[histoID] = ibooker_.book2D("trueEta_vs_eta", "true eta vs (reco eta - true eta) for " + *i_shortName + " id", 50,-5.,5., 50,-0.5,0.5);

    h_truePt_recoParticle[histoID] = ibooker_.book1D("truePt_matched","true pt vs total # matchedPhotons for " + *i_shortName + " id",50,0.,500.);
    h_trueEta_recoParticle[histoID] = ibooker_.book1D("trueEta_matched","true eta vs total # matchedPhotons for " + *i_shortName + " id",50,-5.,5.);

    ++histoID;
  }


  ibooker_.cd();  

}


//
// -------------------------------------- fill histograms --------------------------------------------
//

void PhotonAnalyzer_MiniAOD::fillHisto(int histoID, std::vector<const reco::Candidate*>* recoCollection, std::vector<const reco::Candidate*>* genCollection){

  for (std::vector<const reco::Candidate*>::const_iterator i_genParticle = genCollection->begin(); i_genParticle != genCollection->end(); ++i_genParticle) 
  {
     
    // Match Gen <-> Reco
    const reco::Candidate* matchedParticle = NULL;
    for (std::vector<const reco::Candidate*>::const_iterator i_recoParticle = recoCollection->begin(); i_recoParticle != recoCollection->end(); ++i_recoParticle){
        if(deltaR(**i_recoParticle, **i_genParticle) < 0.3){
          matchedParticle = *i_recoParticle;
          break;
        }
    }
    if(!matchedParticle) continue;
  
    h_truePt_recoParticle[histoID]->Fill((*i_genParticle)->pt());
    h_trueEta_recoParticle[histoID]->Fill((*i_genParticle)->eta());

    h_truePt_pt[histoID]->Fill((*i_genParticle)->pt(), (matchedParticle->pt()-(*i_genParticle)->pt())/(*i_genParticle)->pt()); 
    h_truePt_eta[histoID]->Fill((*i_genParticle)->pt(), matchedParticle->eta()-(*i_genParticle)->eta());
    h_trueEta_pt[histoID]->Fill((*i_genParticle)->eta(), (matchedParticle->pt()-(*i_genParticle)->pt())/(*i_genParticle)->pt());
    h_trueEta_eta[histoID]->Fill((*i_genParticle)->eta(), matchedParticle->eta()-(*i_genParticle)->eta());
  }

}


//
// -------------------------------------- functions --------------------------------------------
//

void PhotonAnalyzer_MiniAOD::fillHisto(int histoID, std::vector<const pat::Photon*>* recoCollection, std::vector<const pat::PackedGenParticle*>* genCollection){
  fillHisto(histoID, (std::vector<const reco::Candidate*>*) recoCollection, (std::vector<const reco::Candidate*>*) genCollection);
}

