#include "FastSimValidation/Analyzer_MiniAOD/interface/bJetsAnalyzer_MiniAOD.h"
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
bJetsAnalyzer_MiniAOD::bJetsAnalyzer_MiniAOD(const edm::ParameterSet& ps)
{
  edm::LogInfo("bJetsAnalyzer_MiniAOD") <<  "Constructor  bJetsAnalyzer_MiniAOD::bJetsAnalyzer_MiniAOD " << std::endl;

  //fullSim_ = ps.getUntrackedParameter<bool>("fullSim");
  
  // Get parameters from configuration file
  theRecoJetCollection_    = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("recoJetCollection"));
  theGenJetCollection_     = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("genJetCollection"));
  thePrunedGenParticleCollection_     = consumes<reco::GenParticleCollection>(ps.getParameter<edm::InputTag>("prunedGenParticleCollection"));

  theBjetIDs_ = ps.getParameter<std::vector<edm::ParameterSet>>("bJetIDs");

  theCollectionName_ = ps.getParameter<std::string>("CollectionName");

  coneSize_ = ps.getParameter<double>("ConeSize");

  // debug
  debug_ = ps.getUntrackedParameter<bool>("Debug");

 
}

//
// -- Destructor
//
bJetsAnalyzer_MiniAOD::~bJetsAnalyzer_MiniAOD()
{
  edm::LogInfo("bJetsAnalyzer_MiniAOD") <<  "Destructor bJetsAnalyzer_MiniAOD::~bJetsAnalyzer_MiniAOD " << std::endl;
}

//
// -------------------------------------- beginRun --------------------------------------------
//
void bJetsAnalyzer_MiniAOD::dqmBeginRun(edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("bJetsAnalyzer_MiniAOD") <<  "bJetsAnalyzer_MiniAOD::beginRun" << std::endl;
}
//
// -------------------------------------- bookHistos --------------------------------------------
//
void bJetsAnalyzer_MiniAOD::bookHistograms(DQMStore::IBooker & ibooker_, edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("bJetsAnalyzer_MiniAOD") <<  "bJetsAnalyzer_MiniAOD::bookHistograms" << std::endl;
  
  //book at beginRun
  bookHistos(ibooker_);
}
//
// -------------------------------------- beginLuminosityBlock --------------------------------------------
//
void bJetsAnalyzer_MiniAOD::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                      edm::EventSetup const& context) 
{
  edm::LogInfo("bJetsAnalyzer_MiniAOD") <<  "bJetsAnalyzer_MiniAOD::beginLuminosityBlock" << std::endl;
}


//
// -------------------------------------- Analyze --------------------------------------------
//
void bJetsAnalyzer_MiniAOD::analyze(edm::Event const& e, edm::EventSetup const& eSetup)
{
  edm::LogInfo("bJetsAnalyzer_MiniAOD") <<  "bJetsAnalyzer_MiniAOD::analyze" << std::endl;

// patJets
  edm::Handle<reco::CandidateCollection> RecoJetCollection;
  e.getByToken (theRecoJetCollection_,RecoJetCollection);
  if ( !RecoJetCollection.isValid() ) 
  {
    edm::LogError ("bJetsAnalyzer_MiniAOD") << "invalid collection: recojets" << "\n";
    return;
  }

std::vector<const pat::Jet*> patJets;
for (reco::CandidateCollection::const_iterator i_recojet = RecoJetCollection->begin(); i_recojet != RecoJetCollection->end(); ++i_recojet)
  {
    try{
      patJets.push_back(dynamic_cast<const pat::Jet*>(&(*i_recojet)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection recojets not of type pat::Jet" << std::endl;
    }
  }

// genJets
  edm::Handle<reco::CandidateCollection> GenJetCollection;
  e.getByToken (theGenJetCollection_,GenJetCollection);
  if ( !GenJetCollection.isValid() ) 
  {
    edm::LogError ("bJetsAnalyzer_MiniAOD") << "invalid collection: genjets" << "\n";
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

// GenParticles for bJet identification
edm::Handle<reco::GenParticleCollection> genParticleCollection;
e.getByToken(thePrunedGenParticleCollection_, genParticleCollection);
if ( !genParticleCollection.isValid() ) 
{
  edm::LogError ("bJetsAnalyzer_MiniAOD") << "invalid collection: prunedGenParticleCollection" << "\n";
  return;
}

// JetIDs
std::vector<std::pair<std::string, double>> idPairs;
for (std::vector<edm::ParameterSet>::const_iterator it = theBjetIDs_.begin(); it != theBjetIDs_.end(); ++it) {
  idPairs.push_back(std::make_pair(it->getParameter<std::string>("idString"), it->getParameter<double>("cutValue")));
}

//-------------------------------
//--- Create Subsets
//-------------------------------
// Create subset for every ID
std::vector<std::vector<const pat::Jet*>> bJets;

for(std::vector<std::pair<std::string, double>>::const_iterator i_bJetID = idPairs.begin(); i_bJetID != idPairs.end(); ++i_bJetID){

  std::vector<const pat::Jet*> passedJets;
  for (std::vector<const pat::Jet *>::const_iterator i_patJet = patJets.begin(); i_patJet != patJets.end(); ++i_patJet){
    if((*i_patJet)->bDiscriminator(i_bJetID->first) > i_bJetID->second){
      passedJets.push_back((*i_patJet));
    }
  }

  bJets.push_back(passedJets);
}

if(debug_)
for (std::vector<const pat::Jet *>::const_iterator i_patJet = patJets.begin(); i_patJet != patJets.end(); ++i_patJet)
{    
  for(std::vector<std::pair<std::string,float>>::const_iterator i_bJetIDs = (*i_patJet)->getPairDiscri().begin(); i_bJetIDs != (*i_patJet)->getPairDiscri().end(); ++i_bJetIDs){
    std::cout << i_bJetIDs->first << std::endl;
  }
}

// select B-/C-mesons
std::vector<const reco::GenParticle*> bMesons;
std::vector<const reco::GenParticle*> cMesons;

for (reco::GenParticleCollection::const_iterator i_genParticle = genParticleCollection->begin(); i_genParticle != genParticleCollection->end(); ++i_genParticle)
{
  int pdgId = std::abs(i_genParticle->pdgId());
  pdgId = pdgId % 10000;

  if((pdgId > 500 && pdgId < 600) || (pdgId > 5000 && pdgId < 6000)) bMesons.push_back(&(*i_genParticle));
  else if((pdgId > 400 && pdgId < 500) || (pdgId > 4000 && pdgId < 5000)) cMesons.push_back(&(*i_genParticle));
}

// match to genJets
std::vector<const reco::GenJet*> genBjets;
std::vector<const reco::GenJet*> genCjets;
std::vector<const reco::GenJet*> genUDSGjets;


for (std::vector<const reco::GenJet *>::const_iterator i_genJet = genJets.begin(); i_genJet != genJets.end(); ++i_genJet){
  bool foundMatch = false;

  for (std::vector<const reco::GenParticle*>::const_iterator i_bMeson = bMesons.begin(); i_bMeson != bMesons.end(); ++i_bMeson)
  {
    if(deltaR(**i_bMeson, **i_genJet) < coneSize_){
      genBjets.push_back(*i_genJet);
      foundMatch = true;
      break;
    }
  }

  if(!foundMatch){
    for (std::vector<const reco::GenParticle*>::const_iterator i_cMeson = cMesons.begin(); i_cMeson != cMesons.end(); ++i_cMeson)
    {
      if(deltaR(**i_cMeson, **i_genJet) < coneSize_){
        genCjets.push_back(*i_genJet);
        foundMatch = true;
        break;
      }
    }
  }

  if(!foundMatch) genUDSGjets.push_back(*i_genJet);
}

//std::cout << genBjets.size() << "; " << genCjets.size() << "; " << genUDSGjets.size()  << std::endl;


//-------------------------------
// Fill Histrograms
//-------------------------------

// std::string matchedParticleNames[3] = {"matchedBjet", "matchedCjet", "matchedUDSGjet"};

int histID = 0;
for(std::vector<std::vector<const pat::Jet*>>::iterator i_bJets = bJets.begin(); i_bJets != bJets.end(); ++i_bJets){
  fillHisto("matchedBjet", histID, &(*i_bJets), &genBjets);
  fillHisto("matchedCjet", histID, &(*i_bJets), &genCjets);
  fillHisto("matchedUDSGjet", histID, &(*i_bJets), &genUDSGjets);

  ++histID;
}


// Gen
for(int i_particle = 0; i_particle < 3; ++i_particle){
  std::vector<const reco::GenJet*> * genFlavourJets;
  if(i_particle == 0) genFlavourJets = &genBjets;
  else if(i_particle == 1) genFlavourJets = &genCjets;
  else genFlavourJets = &genUDSGjets;

  for (std::vector<const reco::GenJet*>::const_iterator i_genJets = genFlavourJets->begin(); i_genJets != genFlavourJets->end(); ++i_genJets) 
  {
    h_truePt_genParticle[i_particle]->Fill((*i_genJets)->pt());
    h_trueEta_genParticle[i_particle]->Fill((*i_genJets)->eta());
  }
}


}
//
// -------------------------------------- endLuminosityBlock --------------------------------------------
//
void bJetsAnalyzer_MiniAOD::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
  edm::LogInfo("bJetsAnalyzer_MiniAOD") <<  "bJetsAnalyzer_MiniAOD::endLuminosityBlock" << std::endl;
}


//
// -------------------------------------- endRun --------------------------------------------
//
void bJetsAnalyzer_MiniAOD::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
  edm::LogInfo("bJetsAnalyzer_MiniAOD") <<  "bJetsAnalyzer_MiniAOD::endRun" << std::endl;
}


//
// -------------------------------------- book histograms --------------------------------------------
//
void bJetsAnalyzer_MiniAOD::bookHistos(DQMStore::IBooker & ibooker_)
{
  std::vector<std::string> tagNamesShort;
  for (std::vector<edm::ParameterSet>::const_iterator it = theBjetIDs_.begin(); it != theBjetIDs_.end(); ++it) {
    tagNamesShort.push_back(it->getParameter<std::string>("idShortName"));
  }

  std::string matchedParticleNames[3] = {"matchedBjet", "matchedCjet", "matchedUDSGjet"};

  ibooker_.cd();
    
  for(int i_particle = 0; i_particle < 3; ++i_particle){
    ibooker_.setCurrentFolder(theCollectionName_+"/"+"Gen"+"/"+matchedParticleNames[i_particle]);
    
    h_truePt_genParticle[i_particle] = ibooker_.book1D("truePt_gen","true pt vs total # genJets",50,0.,500.);
    h_trueEta_genParticle[i_particle] = ibooker_.book1D("trueEta_gen","true eta vs total # genJets",50,-5.,5.);
  }

  for(int i_particle = 0; i_particle < 3; ++i_particle){
    int histoID = 0;
    for(std::vector<std::string>::const_iterator i_shortName = tagNamesShort.begin(); i_shortName != tagNamesShort.end(); ++i_shortName){
      ibooker_.setCurrentFolder(theCollectionName_+"/"+*i_shortName+"/"+matchedParticleNames[i_particle]);

      h_truePt_recoParticle[i_particle][histoID] = ibooker_.book1D("truePt_matched","true pt vs total # matchedJets for " + *i_shortName + " id",50,0.,500.);
      h_trueEta_recoParticle[i_particle][histoID] = ibooker_.book1D("trueEta_matched","true eta vs total # matchedJets for " + *i_shortName + " id",50,-5.,5.);
      ++histoID;
    }
  }
  
  ibooker_.cd();  
}

//
// -------------------------------------- fill histograms --------------------------------------------
//

void bJetsAnalyzer_MiniAOD::fillHisto(std::string matchedParticle, int histoID, std::vector<const reco::Candidate*>* recoCollection, std::vector<const reco::Candidate*>* genCollection){

  int particleNr = 0;
  if(matchedParticle.compare("matchedBjet") == 0) particleNr = 0;
  else if(matchedParticle.compare("matchedCjet") == 0) particleNr = 1;
  else if(matchedParticle.compare("matchedUDSGjet") == 0) particleNr = 2;
  else{std::cout << "JetsAnalyzer_MiniAOD: String for histogram filling has to be element of {matchedBjet, matchedCjet, matchedUDSGjet}." << std::endl; return;}

  for (std::vector<const reco::Candidate*>::const_iterator i_genParticle = genCollection->begin(); i_genParticle != genCollection->end(); ++i_genParticle){   
    // Match Gen <-> Reco
    const reco::Candidate* matchedParticle = NULL;
    for (std::vector<const reco::Candidate*>::const_iterator i_recoParticle = recoCollection->begin(); i_recoParticle != recoCollection->end(); ++i_recoParticle){
        if(deltaR(**i_recoParticle, **i_genParticle) < 0.3){
          matchedParticle = *i_recoParticle;
          break;
        }
    }
    if(!matchedParticle) continue;
  
    h_truePt_recoParticle[particleNr][histoID]->Fill((*i_genParticle)->pt());
    h_trueEta_recoParticle[particleNr][histoID]->Fill((*i_genParticle)->eta());
  }

}


//
// -------------------------------------- functions --------------------------------------------
//

void bJetsAnalyzer_MiniAOD::fillHisto(std::string matchedParticle, int histoID, std::vector<const pat::Jet*>* recoCollection, std::vector<const reco::GenJet*>* genCollection){
  fillHisto(matchedParticle, histoID, (std::vector<const reco::Candidate*>*) recoCollection, (std::vector<const reco::Candidate*>*) genCollection);
}