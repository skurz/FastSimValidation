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

  theCollectionName_ = ps.getParameter<std::string>("CollectionName");


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

  std::vector<const reco::GenParticle*> genTaus;
  for (reco::CandidateCollection::const_iterator i_genTau = genTauCollection->begin(); i_genTau != genTauCollection->end(); ++i_genTau)
  {
    try{
      genTaus.push_back(dynamic_cast<const reco::GenParticle*>(&(*i_genTau)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection genTaus not of type reco::GenParticle" << std::endl;
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
  std::vector<std::vector<std::pair<std::string, double>>> idPairs;
  for (std::vector<edm::ParameterSet>::const_iterator it = theTauIDs_.begin(); it != theTauIDs_.end(); ++it) {
    std::vector<std::string> idStrings = it->getParameter<std::vector<std::string>>("idStrings");
    std::vector<double> cutValues = it->getParameter<std::vector<double>>("cutValues");

    std::vector<std::pair<std::string, double>> i_idPair;
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

  // Taus that pass ID selection
  for(std::vector<std::vector<std::pair<std::string, double>>>::const_iterator i_tauIDvec = idPairs.begin(); i_tauIDvec != idPairs.end(); ++i_tauIDvec){

    std::vector<const pat::Tau*> passedTaus;
    bool passedAll = true;
    for (std::vector<const pat::Tau *>::const_iterator i_patTau = patTaus.begin(); i_patTau != patTaus.end(); ++i_patTau){
      for (std::vector<std::pair<std::string, double>>::const_iterator i_tauID = i_tauIDvec->begin(); i_tauID != i_tauIDvec->end(); ++i_tauID){
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


  // Make sure there is no overlap between status 2 and 3 taus
  std::vector<const reco::GenParticle*> cleanedGenTaus;
  std::vector<const reco::GenParticle*> stat2Taus;
  for (std::vector<const reco::GenParticle*>::const_iterator i_genTau = genTaus.begin(); i_genTau != genTaus.end(); ++i_genTau){
    if((*i_genTau)->status() == 3) cleanedGenTaus.push_back(*i_genTau);
    if((*i_genTau)->status() == 2) stat2Taus.push_back(*i_genTau);
  }

  for (std::vector<const reco::GenParticle*>::const_iterator i_statTau = stat2Taus.begin(); i_statTau != stat2Taus.end(); ++i_statTau){
    bool foundMatch = false;

    for (std::vector<const reco::GenParticle*>::const_iterator i_genTau = cleanedGenTaus.begin(); i_genTau != cleanedGenTaus.end(); ++i_genTau){
      if(deltaR(**i_statTau, **i_genTau) < 0.1){
        foundMatch = true;
        break;
      }
    }

    if(!foundMatch) cleanedGenTaus.push_back(*i_statTau);
  }

  // GenTaus from Hard Interaction
  std::vector<const reco::GenParticle*> selectedGenTaus;
  
  for (std::vector<const reco::GenParticle*>::const_iterator i_genTaus = cleanedGenTaus.begin(); i_genTaus != cleanedGenTaus.end(); ++i_genTaus) 
  {
    const reco::Candidate* mom = *i_genTaus;
    while(true){
      if(mom == 0){
        break;
      }else if(abs(mom->pdgId())==23 || abs(mom->pdgId())==24){
        selectedGenTaus.push_back(*i_genTaus);
        break;
      }else if(abs(mom->pdgId())==11 || abs(mom->pdgId())==13 || abs(mom->pdgId()==15)){
        mom = mom->mother(0);
        continue;
      }else break;
    }
  }

  // Hadronic taus on genLevel
  std::vector<const reco::GenParticle*> hadGenTaus;
  for (std::vector<const reco::GenParticle*>::const_iterator i_genTau = selectedGenTaus.begin(); i_genTau != selectedGenTaus.end(); ++i_genTau){
    const reco::Candidate* i_daughter = *i_genTau;
    bool foundLepton = false;
    bool foundHadron = false;

    while(!foundLepton && !foundHadron){
      for(unsigned int dIndex = 0; dIndex < i_daughter->numberOfDaughters(); dIndex++){
        int pdgId = std::abs(i_daughter->daughter(dIndex)->pdgId());
        if(pdgId == 15){
          i_daughter = i_daughter->daughter(dIndex);
          break;
        }else if(pdgId == 11 || pdgId == 13){
          foundLepton = true;
          break;
        }
        if(dIndex == i_daughter->numberOfDaughters()-1){
          foundHadron = true;
        }
      }
    }

    if(foundHadron){
      hadGenTaus.push_back(*i_genTau);
    }
  }
  

  //-------------------------------
  // Fill Histrograms
  //-------------------------------

  //std::string matchedParticleNames[4] = {"matchedTau", "matchedEl", "matchedMu", "matchedJet"};

  int histID = 0;
  for(std::vector<std::vector<const pat::Tau*>>::iterator i_idTaus = idTaus.begin(); i_idTaus != idTaus.end(); ++i_idTaus){
    fillHisto("matchedTau", histID, &(*i_idTaus), &hadGenTaus);
    fillHisto("matchedEl", histID, &(*i_idTaus), &genElectrons);
    fillHisto("matchedMu", histID, &(*i_idTaus), &genMuons);
    fillHisto("matchedJet", histID, &(*i_idTaus), &genJets);

    ++histID;
  }


  // Gen
  for (std::vector<const reco::GenParticle*>::const_iterator i_genTau = hadGenTaus.begin(); i_genTau != hadGenTaus.end(); ++i_genTau) 
  {
    h_truePt_genParticle[0]->Fill((*i_genTau)->pt());
    h_trueEta_genParticle[0]->Fill((*i_genTau)->eta());
  }

  for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genEl = genElectrons.begin(); i_genEl != genElectrons.end(); ++i_genEl) 
  {
    h_truePt_genParticle[1]->Fill((*i_genEl)->pt());
    h_trueEta_genParticle[1]->Fill((*i_genEl)->eta());
  }

  for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genMu = genMuons.begin(); i_genMu != genMuons.end(); ++i_genMu) 
  {
    h_truePt_genParticle[2]->Fill((*i_genMu)->pt());
    h_trueEta_genParticle[2]->Fill((*i_genMu)->eta());
  }

  for (std::vector<const reco::GenJet*>::const_iterator i_genJets = genJets.begin(); i_genJets != genJets.end(); ++i_genJets) 
  {
    h_truePt_genParticle[3]->Fill((*i_genJets)->pt());
    h_trueEta_genParticle[3]->Fill((*i_genJets)->eta());
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
  std::vector<std::string> tagNamesShort;
  for (std::vector<edm::ParameterSet>::const_iterator it = theTauIDs_.begin(); it != theTauIDs_.end(); ++it) {
    tagNamesShort.push_back(it->getParameter<std::string>("idShortName"));
  }

  std::string matchedParticleNames[4] = {"matchedTau", "matchedEl", "matchedMu", "matchedJet"};

  ibooker_.cd();

  for(int i_particle = 0; i_particle < 4; ++i_particle){
    ibooker_.setCurrentFolder(theCollectionName_+"/Gen/"+matchedParticleNames[i_particle]);
    
    h_truePt_genParticle[i_particle] = ibooker_.book1D("truePt_gen","",50,0.,500.);
    h_trueEta_genParticle[i_particle] = ibooker_.book1D("trueEta_gen","",50,-5.,5.);
  }

  for(int i_particle = 0; i_particle < 4; ++i_particle){
    int histoID = 0;
    for(std::vector<std::string>::const_iterator i_shortName = tagNamesShort.begin(); i_shortName != tagNamesShort.end(); ++i_shortName){
      ibooker_.setCurrentFolder(theCollectionName_+"/"+*i_shortName+"/"+matchedParticleNames[i_particle]);

      h_xPt_yPt[i_particle][histoID] = ibooker_.book2D("relPt_vs_truePt", "", 50,0.,500., 500,0.,2.);
      h_xPt_yEta[i_particle][histoID] = ibooker_.book2D("etaDiff_vs_truePt", "", 50,0.,500., 500,-0.5,0.5);
      h_xEta_yPt[i_particle][histoID] = ibooker_.book2D("relPt_vs_trueEta", "", 50,-5.,5., 500,0.,2.);
      h_xEta_yEta[i_particle][histoID] = ibooker_.book2D("etaDiff_vs_trueEta", "", 50,-5.,5., 500,-0.5,0.5);
        
      h_truePt_recoParticle[i_particle][histoID] = ibooker_.book1D("truePt_matched","",50,0.,500.);
      h_trueEta_recoParticle[i_particle][histoID] = ibooker_.book1D("trueEta_matched","",50,-5.,5.);
      
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

  for (std::vector<const reco::Candidate*>::const_iterator i_genParticle = genCollection->begin(); i_genParticle != genCollection->end(); ++i_genParticle) {
     
    // Match Gen <-> Reco
    const reco::Candidate* matchedParticle = NULL;
    for (std::vector<const reco::Candidate*>::const_iterator i_recoParticle = recoCollection->begin(); i_recoParticle != recoCollection->end(); ++i_recoParticle){    
        if(deltaR(**i_recoParticle, **i_genParticle) < 0.3) matchedParticle = (*i_recoParticle);
        break;
    }
    if(!matchedParticle) continue;
  
    h_truePt_recoParticle[particleNr][histoID]->Fill((*i_genParticle)->pt());
    h_trueEta_recoParticle[particleNr][histoID]->Fill((*i_genParticle)->eta());

    h_xPt_yPt[particleNr][histoID]->Fill((*i_genParticle)->pt(), matchedParticle->pt()/(*i_genParticle)->pt()); 
    h_xPt_yEta[particleNr][histoID]->Fill((*i_genParticle)->pt(), matchedParticle->eta()-(*i_genParticle)->eta());
    h_xEta_yPt[particleNr][histoID]->Fill((*i_genParticle)->eta(), matchedParticle->pt()/(*i_genParticle)->pt());
    h_xEta_yEta[particleNr][histoID]->Fill((*i_genParticle)->eta(), matchedParticle->eta()-(*i_genParticle)->eta());

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

void TauAnalyzer_MiniAOD::fillHisto(std::string matchedParticle, int histoID, std::vector<const pat::Tau*>* recoCollection, std::vector<const reco::GenParticle*>* genCollection){
  fillHisto(matchedParticle, histoID, (std::vector<const reco::Candidate*>*) recoCollection, (std::vector<const reco::Candidate*>*) genCollection);
}