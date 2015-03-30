#include "FastSimValidation/Analyzer_MiniAOD/interface/TauAnalyzer_MiniAOD.h"
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
TauAnalyzer_MiniAOD::TauAnalyzer_MiniAOD(const edm::ParameterSet& ps)
{
  edm::LogInfo("TauAnalyzer_MiniAOD") <<  "Constructor  TauAnalyzer_MiniAOD::TauAnalyzer_MiniAOD " << std::endl;

  //fullSim_ = ps.getUntrackedParameter<bool>("fullSim");
  
  // Get parameters from configuration file
  theTauCollection_         = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("TauCollection"));
  theGenTauCollection_      = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenTauCollection"));
  theGenElectronCollection_ = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenElectronCollection"));
  theGenMuonCollection_     = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenMuonCollection"));
  theGenJetCollection_      = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenJetCollection"));

  theTauIDs_ = ps.getParameter<std::vector<edm::ParameterSet>>("TauIDs");


  // debug
  debug_ = ps.getUntrackedParameter<bool>("Debug");

 
}

//
// -- Destructor
//
TauAnalyzer_MiniAOD::~TauAnalyzer_MiniAOD()
{
  edm::LogInfo("TauAnalyzer_MiniAOD") <<  "Destructor TauAnalyzer_MiniAOD::~TauAnalyzer_MiniAOD " << std::endl;
}

//
// -------------------------------------- beginRun --------------------------------------------
//
void TauAnalyzer_MiniAOD::dqmBeginRun(edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("TauAnalyzer_MiniAOD") <<  "TauAnalyzer_MiniAOD::beginRun" << std::endl;
}
//
// -------------------------------------- bookHistos --------------------------------------------
//
void TauAnalyzer_MiniAOD::bookHistograms(DQMStore::IBooker & ibooker_, edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("TauAnalyzer_MiniAOD") <<  "TauAnalyzer_MiniAOD::bookHistograms" << std::endl;
  
  //book at beginRun
  bookHistos(ibooker_);
}
//
// -------------------------------------- beginLuminosityBlock --------------------------------------------
//
void TauAnalyzer_MiniAOD::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                      edm::EventSetup const& context) 
{
  edm::LogInfo("TauAnalyzer_MiniAOD") <<  "TauAnalyzer_MiniAOD::beginLuminosityBlock" << std::endl;
}


//
// -------------------------------------- Analyze --------------------------------------------
//
void TauAnalyzer_MiniAOD::analyze(edm::Event const& e, edm::EventSetup const& eSetup)
{
  edm::LogInfo("TauAnalyzer_MiniAOD") <<  "TauAnalyzer_MiniAOD::analyze" << std::endl;

  //-------------------------------
  //--- Get Collections
  //-------------------------------

  // RecoTaus
  edm::Handle<reco::CandidateCollection> TauCollection;
  e.getByToken (theTauCollection_,TauCollection);
  if ( !TauCollection.isValid() ) 
  {
    edm::LogError ("TauAnalyzer_MiniAOD") << "invalid collection: Taus" << "\n";
    return;
  }

  std::vector<const pat::Tau*> patTaus;
  for (reco::CandidateCollection::const_iterator i_recoTau = TauCollection->begin(); i_recoTau != TauCollection->end(); ++i_recoTau)
  {
    try{
      patTaus.push_back(dynamic_cast<const pat::Tau*>(&(*i_recoTau)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection recoTaus not of type pat::Tau" << std::endl;
    }
  }

  // GenTaus
  edm::Handle<reco::CandidateCollection> genTauCollection;
  e.getByToken(theGenTauCollection_, genTauCollection);
  if ( !genTauCollection.isValid() ) 
  {
    edm::LogError ("TauAnalyzer_MiniAOD") << "invalid collection: genTaus" << "\n";
    return;
  }

  std::vector<const pat::PackedGenParticle*> genTaus;
  for (reco::CandidateCollection::const_iterator i_genTau = genTauCollection->begin(); i_genTau != genTauCollection->end(); ++i_genTau)
  {
    try{
      genTaus.push_back(dynamic_cast<const pat::PackedGenParticle*>(&(*i_genTau)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection genTaus not of type pat::PackedGenParticle" << std::endl;
    }
  }

  // GenElectrons
  edm::Handle<reco::CandidateCollection> genElectronCollection;
  e.getByToken(theGenElectronCollection_, genElectronCollection);
  if ( !genElectronCollection.isValid() ) 
  {
    edm::LogError ("TauAnalyzer_MiniAOD") << "invalid collection: genElectrons" << "\n";
    return;
  }

  std::vector<const pat::PackedGenParticle*> genElectrons;
  for (reco::CandidateCollection::const_iterator i_genTau = genElectronCollection->begin(); i_genTau != genElectronCollection->end(); ++i_genTau)
  {
    try{
      genElectrons.push_back(dynamic_cast<const pat::PackedGenParticle*>(&(*i_genTau)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection genElectrons not of type pat::PackedGenParticle" << std::endl;
    }
  }

  // GenMuons
  edm::Handle<reco::CandidateCollection> genMuonCollection;
  e.getByToken(theGenMuonCollection_, genMuonCollection);
  if ( !genMuonCollection.isValid() ) 
  {
    edm::LogError ("TauAnalyzer_MiniAOD") << "invalid collection: genMuons" << "\n";
    return;
  }

  std::vector<const pat::PackedGenParticle*> genMuons;
  for (reco::CandidateCollection::const_iterator i_genTau = genMuonCollection->begin(); i_genTau != genMuonCollection->end(); ++i_genTau)
  {
    try{
      genMuons.push_back(dynamic_cast<const pat::PackedGenParticle*>(&(*i_genTau)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection genElectrons not of type pat::PackedGenParticle" << std::endl;
    }
  }

  // genJets
  edm::Handle<reco::CandidateCollection> GenJetCollection;
  e.getByToken (theGenJetCollection_,GenJetCollection);
  if ( !GenJetCollection.isValid() ) 
  {
    edm::LogError ("TauAnalyzer_MiniAOD") << "invalid collection: genjets" << "\n";
    return;
  }

std::vector<const reco::GenJet*> genJets;
for (reco::CandidateCollection::const_iterator i_genjet = GenJetCollection->begin(); i_genjet != GenJetCollection->end(); ++i_genjet)
  {
    try{
      genJets.push_back(dynamic_cast<const reco::GenJet*>(&(*i_genjet)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection genjets not of type reco::GenJet" << std::endl;
    }
  }

  // TauIDs
  std::vector<std::vector<std::pair<TString, double>>> idPairs;
  for (std::vector<edm::ParameterSet>::const_iterator it = theTauIDs_.begin(); it != theTauIDs_.end(); ++it) {
    std::vector<std::string> idStrings = it->getParameter<std::vector<std::string>>("idStrings");
    std::vector<double> cutValues = it->getParameter<std::vector<double>>("cutValues");

    std::vector<std::pair<TString, double>> i_idPair;
    std::vector<std::string>::const_iterator i_string; std::vector<double>::const_iterator i_double;
    for (i_string = idStrings.begin(), i_double = cutValues.begin(); (i_string != idStrings.end()) && (i_double != cutValues.end()); ++i_string, ++i_double) {
      i_idPair.push_back(std::make_pair(*i_string, *i_double));
    }

    idPairs.push_back(i_idPair);
  }


  //-------------------------------
  //--- Create Subsets
  //-------------------------------
    // Create subset for every ID
  std::vector<std::vector<const pat::Tau*>> idTaus;

  for(std::vector<std::vector<std::pair<TString, double>>>::const_iterator i_tauIDvec = idPairs.begin(); i_tauIDvec != idPairs.end(); ++i_tauIDvec){

    std::vector<const pat::Tau*> passedTaus;
    bool passedAll = true;
    for (std::vector<const pat::Tau *>::const_iterator i_patTau = patTaus.begin(); i_patTau != patTaus.end(); ++i_patTau){
      for (std::vector<std::pair<TString, double>>::const_iterator i_tauID = i_tauIDvec->begin(); i_tauID != i_tauIDvec->end(); ++i_tauID){
        if((*i_patTau)->tauID(i_tauID->first) < i_tauID->second) passedAll = false;
      }
     
      if(passedAll) passedTaus.push_back((*i_patTau));
    }

    idTaus.push_back(passedTaus);
  }

  if(debug_)
  for (std::vector<const pat::Tau *>::const_iterator i_patTau = patTaus.begin(); i_patTau != patTaus.end(); ++i_patTau)
  {    
    for(std::vector<std::pair<std::string,float>>::const_iterator i_tauIDs = (*i_patTau)->tauIDs().begin(); i_tauIDs != (*i_patTau)->tauIDs().end(); ++i_tauIDs){
      std::cout << i_tauIDs->first << std::endl;
    }
  }   
  

  //-------------------------------
  // Fill Histrograms
  //-------------------------------

  std::string matchedParticleNames[4] = {"matchedTau", "matchedEl", "matchedMu", "matchedJet"};

  for(int i_particle = 0; i_particle < 4; ++i_particle){
    std::vector<const pat::PackedGenParticle*>* genCollection;
    if(i_particle == 0) genCollection = &genTaus;
    if(i_particle == 1) genCollection = &genElectrons;
    if(i_particle == 2) genCollection = &genMuons;

    int histID = 0;
    if(i_particle < 3)
      for(std::vector<std::vector<const pat::Tau*>>::iterator i_idTaus = idTaus.begin(); i_idTaus != idTaus.end(); ++i_idTaus){
        fillHisto(matchedParticleNames[i_particle], histID, &(*i_idTaus), genCollection);
        //std::cout << (&(*it))->size() << std::endl;
        ++histID;
      }
    else
      for(std::vector<std::vector<const pat::Tau*>>::iterator i_idTaus = idTaus.begin(); i_idTaus != idTaus.end(); ++i_idTaus){
        fillHisto(matchedParticleNames[i_particle], histID, &(*i_idTaus), &genJets);
        //std::cout << (&(*it))->size() << std::endl;
        ++histID;
      }
  }


  // Gen
  for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genTau = genTaus.begin(); i_genTau != genTaus.end(); ++i_genTau) 
  {
    h_truePt_genParticle->Fill((*i_genTau)->pt());
    h_trueEta_genParticle->Fill((*i_genTau)->eta());
  }


}
//
// -------------------------------------- endLuminosityBlock --------------------------------------------
//
void TauAnalyzer_MiniAOD::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
  edm::LogInfo("TauAnalyzer_MiniAOD") <<  "TauAnalyzer_MiniAOD::endLuminosityBlock" << std::endl;
}


//
// -------------------------------------- endRun --------------------------------------------
//
void TauAnalyzer_MiniAOD::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
  edm::LogInfo("TauAnalyzer_MiniAOD") <<  "TauAnalyzer_MiniAOD::endRun" << std::endl;
}


//
// -------------------------------------- book histograms --------------------------------------------
//
void TauAnalyzer_MiniAOD::bookHistos(DQMStore::IBooker & ibooker_)
{
  std::vector<TString> tagNamesShort;
  for (std::vector<edm::ParameterSet>::const_iterator it = theTauIDs_.begin(); it != theTauIDs_.end(); ++it) {
    tagNamesShort.push_back(it->getParameter<std::string>("idShortName"));
  }

  std::string matchedParticleNames[4] = {"matchedTau", "matchedEl", "matchedMu", "matchedJet"};

  ibooker_.cd();
  ibooker_.setCurrentFolder("Tau");

  for(int i_particle = 0; i_particle < 4; ++i_particle){
    int histoID = 0;
    for(std::vector<TString>::const_iterator i_shortName = tagNamesShort.begin(); i_shortName != tagNamesShort.end(); ++i_shortName){
      h_truePt_pt[i_particle][histoID] = ibooker_.book2D(matchedParticleNames[i_particle] + "_" + *i_shortName + "ID_truePt_pt", "true pt vs pt for " + *i_shortName + " id", 50,0.,500., 50,0.,500.);
      h_truePt_eta[i_particle][histoID] = ibooker_.book2D(matchedParticleNames[i_particle] + "_" + *i_shortName + "ID_truePt_eta", "true pt vs eta for " + *i_shortName + " id", 50,0.,500., 50,-5.,5.);
      h_trueEta_pt[i_particle][histoID] = ibooker_.book2D(matchedParticleNames[i_particle] + "_" + *i_shortName + "ID_trueEta_pt", "true eta vs pt for " + *i_shortName + " id", 50,-5.,5., 50,0.,500.);
      h_trueEta_eta[i_particle][histoID] = ibooker_.book2D(matchedParticleNames[i_particle] + "_" + *i_shortName + "ID_trueEta_eta", "true eta vs eta for " + *i_shortName + " id", 50,-5.,5., 50,-5.,5.);
      ++histoID;
    }
  }

  ibooker_.setCurrentFolder("Tau/Helpers");
  
  h_truePt_genParticle = ibooker_.book1D("truePt_genTau","true pt vs total# genTaus",50,0.,500.);
  h_trueEta_genParticle = ibooker_.book1D("trueEta_genTau","true eta vs total# genTaus",50,-5.,5.);

  for(int i_particle = 0; i_particle < 4; ++i_particle){
    int histoID = 0;
    for(std::vector<TString>::const_iterator i_shortName = tagNamesShort.begin(); i_shortName != tagNamesShort.end(); ++i_shortName){
      h_truePt_recoParticle[i_particle][histoID] = ibooker_.book1D(matchedParticleNames[i_particle] + "_" + *i_shortName + "ID_truePt_recoTau","true pt vs total# recoTaus for " + *i_shortName + " id",50,0.,500.);
      h_trueEta_recoParticle[i_particle][histoID] = ibooker_.book1D(matchedParticleNames[i_particle] + "_" + *i_shortName + "ID_trueEta_recoTau","true eta vs total# recoTaus for " + *i_shortName + " id",50,-5.,5.);
      ++histoID;
    }
  }
  
  ibooker_.cd();  
}

//
// -------------------------------------- fill histograms --------------------------------------------
//

void TauAnalyzer_MiniAOD::fillHisto(std::string matchedParticle, int histoID, std::vector<const reco::Candidate*>* recoCollection, std::vector<const reco::Candidate*>* genCollection){

  int particleNr = 0;
  if(matchedParticle.compare("matchedTau") == 0) particleNr = 0;
  else if(matchedParticle.compare("matchedEl") == 0) particleNr = 1;
  else if(matchedParticle.compare("matchedMu") == 0) particleNr = 2;
  else if(matchedParticle.compare("matchedJet") == 0) particleNr = 3;
  else{std::cout << "TauAnalyzer_MiniAOD: String for histogram filling has to be element of {matchedTau, matchedEl, matchedMu, matchedJet}." << std::endl; return;}


   for (std::vector<const reco::Candidate*>::const_iterator i_recoParticle = recoCollection->begin(); i_recoParticle != recoCollection->end(); ++i_recoParticle)
  {
     
    // Match Gen <-> Reco
    const reco::Candidate* matchedGenParticle = NULL;
    for (std::vector<const reco::Candidate*>::const_iterator i_genParticle = genCollection->begin(); i_genParticle != genCollection->end(); ++i_genParticle) {
        if(deltaR(**i_recoParticle, **i_genParticle) < 0.3) matchedGenParticle = (*i_genParticle);
    }
    if(!matchedGenParticle) continue;
  
    h_truePt_recoParticle[particleNr][histoID]->Fill(matchedGenParticle->pt());
    h_trueEta_recoParticle[particleNr][histoID]->Fill(matchedGenParticle->eta());

    h_truePt_pt[particleNr][histoID]->Fill(matchedGenParticle->pt(), (*i_recoParticle)->pt()); 
    h_truePt_eta[particleNr][histoID]->Fill(matchedGenParticle->pt(), (*i_recoParticle)->eta());
    h_trueEta_pt[particleNr][histoID]->Fill(matchedGenParticle->eta(), (*i_recoParticle)->pt());
    h_trueEta_eta[particleNr][histoID]->Fill(matchedGenParticle->eta(), (*i_recoParticle)->eta());
  }

}


//
// -------------------------------------- functions --------------------------------------------
//

void TauAnalyzer_MiniAOD::fillHisto(std::string matchedParticle, int histoID, std::vector<const pat::Tau*>* recoCollection, std::vector<const pat::PackedGenParticle*>* genCollection){
  fillHisto(matchedParticle, histoID, (std::vector<const reco::Candidate*>*) recoCollection, (std::vector<const reco::Candidate*>*) genCollection);
}

void TauAnalyzer_MiniAOD::fillHisto(std::string matchedParticle, int histoID, std::vector<const pat::Tau*>* recoCollection, std::vector<const reco::GenJet*>* genCollection){
  fillHisto(matchedParticle, histoID, (std::vector<const reco::Candidate*>*) recoCollection, (std::vector<const reco::Candidate*>*) genCollection);
}