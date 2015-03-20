#include "FastSimValidation/Analyzer_MiniAOD/interface/JetsAnalyzer_MiniAOD.h"
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
JetsAnalyzer_MiniAOD::JetsAnalyzer_MiniAOD(const edm::ParameterSet& ps)
{
  edm::LogInfo("JetsAnalyzer_MiniAOD") <<  "Constructor  JetsAnalyzer_MiniAOD::JetsAnalyzer_MiniAOD " << std::endl;

  //fullSim_ = ps.getUntrackedParameter<bool>("fullSim");
  
  // Get parameters from configuration file
  theRecoJetCollection_    = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("recoJetCollection"));
  theGenJetCollection_     = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("genJetCollection"));
  
  //triggerResults_          = consumes<edm::TriggerResults>(ps.getParameter<edm::InputTag>("TriggerResults"));
  //triggerObject_           = consumes<pat::TriggerObjectStandAlone>(ps.getParameter<edm::InputTag>("TriggerObject"));
  //triggerPath_             = ps.getParameter<std::string>("TriggerPath");

  // debug
  debug_ = ps.getUntrackedParameter<bool>("Debug");

 
}

//
// -- Destructor
//
JetsAnalyzer_MiniAOD::~JetsAnalyzer_MiniAOD()
{
  edm::LogInfo("JetsAnalyzer_MiniAOD") <<  "Destructor JetsAnalyzer_MiniAOD::~JetsAnalyzer_MiniAOD " << std::endl;
}

//
// -------------------------------------- beginRun --------------------------------------------
//
void JetsAnalyzer_MiniAOD::dqmBeginRun(edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("JetsAnalyzer_MiniAOD") <<  "JetsAnalyzer_MiniAOD::beginRun" << std::endl;
}
//
// -------------------------------------- bookHistos --------------------------------------------
//
void JetsAnalyzer_MiniAOD::bookHistograms(DQMStore::IBooker & ibooker_, edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("JetsAnalyzer_MiniAOD") <<  "JetsAnalyzer_MiniAOD::bookHistograms" << std::endl;
  
  //book at beginRun
  bookHistos(ibooker_);
}
//
// -------------------------------------- beginLuminosityBlock --------------------------------------------
//
void JetsAnalyzer_MiniAOD::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                      edm::EventSetup const& context) 
{
  edm::LogInfo("JetsAnalyzer_MiniAOD") <<  "JetsAnalyzer_MiniAOD::beginLuminosityBlock" << std::endl;
}


//
// -------------------------------------- Analyze --------------------------------------------
//
void JetsAnalyzer_MiniAOD::analyze(edm::Event const& e, edm::EventSetup const& eSetup)
{
  edm::LogInfo("JetsAnalyzer_MiniAOD") <<  "JetsAnalyzer_MiniAOD::analyze" << std::endl;

// patJets
  edm::Handle<reco::CandidateCollection> RecoJetCollection;
  e.getByToken (theRecoJetCollection_,RecoJetCollection);
  if ( !RecoJetCollection.isValid() ) 
  {
    edm::LogError ("JetsAnalyzer_MiniAOD") << "invalid collection: recojets" << "\n";
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
    edm::LogError ("JetsAnalyzer_MiniAOD") << "invalid collection: genjets" << "\n";
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


// Analyze distributions
for (std::vector<const pat::Jet *>::const_iterator i_patJet = patJets.begin(); i_patJet != patJets.end(); ++i_patJet)
{
  const pat::Jet* i_patJetPtr = *i_patJet;

  // Match Gen <-> Reco
  const reco::GenJet* matchedGenJet = NULL;
  for (std::vector<const reco::GenJet*>::const_iterator i_genjet = genJets.begin(); i_genjet != genJets.end(); ++i_genjet) {
      if(deltaR(*i_patJetPtr, **i_genjet) < 0.3) matchedGenJet = (*i_genjet);
  }
  if(!matchedGenJet) continue;

  // Fill histograms 
  h_PtvsrecoJet->Fill(matchedGenJet->pt());
  h_EtavsrecoJet->Fill(matchedGenJet->eta());

  h_PtvsjetRelPtDiff->Fill(matchedGenJet->pt(), (i_patJetPtr->pt()-matchedGenJet->pt())/matchedGenJet->pt());
  h_EtavsjetRelPtDiff->Fill(matchedGenJet->eta(), (i_patJetPtr->pt()-matchedGenJet->pt())/matchedGenJet->pt());

  h_PtvsjetEtaDiff->Fill(matchedGenJet->pt(), i_patJetPtr->eta()-matchedGenJet->eta());
  h_EtavsjetEtaDiff->Fill(matchedGenJet->eta(), i_patJetPtr->eta()-matchedGenJet->eta());

  h_PtvsjetPhiDiff->Fill(matchedGenJet->pt(), i_patJetPtr->phi()-matchedGenJet->phi());
  h_EtavsjetPhiDiff->Fill(matchedGenJet->eta(), i_patJetPtr->phi()-matchedGenJet->phi());  


  if(debug_ && (abs(i_patJetPtr->partonFlavour()) == 5 || i_patJetPtr->bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags") > 0.941))
    std::cout << (abs(i_patJetPtr->partonFlavour()) == 5) << "; " << (i_patJetPtr->bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags") > 0.941) << std::endl;
}

for (std::vector<const reco::GenJet*>::const_iterator i_genjet = genJets.begin(); i_genjet != genJets.end(); ++i_genjet) 
{
    h_PtvsgenJet->Fill((*i_genjet)->pt());
    h_EtavsgenJet->Fill((*i_genjet)->eta());
}

  // b Tagging
/*  const std::vector<std::pair<std::string,float>> discriVec = patJets.begin()->getPairDiscri();

  if(debug_)
  for(std::vector<std::pair<std::string,float>>::const_iterator i_discriVec = discriVec.begin(); i_discriVec != discriVec.end(); ++i_discriVec){
    //if(i_discriVec->first != NULL) std::cout << i_discriVec->first << std::endl;
  }
*/
/*Content of MiniAOD
  jetBProbabilityBJetTags
  jetProbabilityBJetTags
  trackCountingHighPurBJetTags
  trackCountingHighEffBJetTags
  simpleSecondaryVertexHighEffBJetTags
  simpleSecondaryVertexHighPurBJetTags
  combinedInclusiveSecondaryVertexV2BJetTags
  pfCombinedSecondaryVertexBJetTags
  combinedMVABJetTags
*/



}
//
// -------------------------------------- endLuminosityBlock --------------------------------------------
//
void JetsAnalyzer_MiniAOD::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
  edm::LogInfo("JetsAnalyzer_MiniAOD") <<  "JetsAnalyzer_MiniAOD::endLuminosityBlock" << std::endl;
}


//
// -------------------------------------- endRun --------------------------------------------
//
void JetsAnalyzer_MiniAOD::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
  edm::LogInfo("JetsAnalyzer_MiniAOD") <<  "JetsAnalyzer_MiniAOD::endRun" << std::endl;
}


//
// -------------------------------------- book histograms --------------------------------------------
//
void JetsAnalyzer_MiniAOD::bookHistos(DQMStore::IBooker & ibooker_)
{
  ibooker_.cd();
  ibooker_.setCurrentFolder("Jets");
  
  // TH2

  h_PtvsjetRelPtDiff = ibooker_.book2D("PtvsjetRelPtDiff","(reco pt - true pt) / (true pt) vs true pt",50,0.,500.,20,-1.,1.);
  h_EtavsjetRelPtDiff = ibooker_.book2D("EtavsjetRelPtDiff","(reco pt - true pt) / (true pt) vs true eta",50,-5.,5.,20,-1.,1.);

  h_PtvsjetEtaDiff = ibooker_.book2D("PtvsjetEtaDiff","(reco eta - true eta) vs true pt",50,0.,500.,20,-0.5,0.5);
  h_EtavsjetEtaDiff = ibooker_.book2D("EtavsjetEtaDiff","(reco eta - true eta) vs true eta",50,-5.,5.,20,-1.,1.);

  h_PtvsjetPhiDiff = ibooker_.book2D("PtvsjetPhiDiff","(reco phi - true phi) vs true pt",50,0.,500.,20,-0.5,0.5);
  h_EtavsjetPhiDiff = ibooker_.book2D("EtavsjetPhiDiff","(reco phi - true phi) vs true eta",50,-5.,5.,20,-1.,1.);


  ibooker_.setCurrentFolder("Jets/Helpers");

  h_PtvsgenJet = ibooker_.book1D("PtvsgenJet","# genJets vs pt",50,0.,500.);
  h_EtavsgenJet = ibooker_.book1D("EtavsgenJet","# genJets vs eta",50,-5.,5.);

  h_PtvsrecoJet = ibooker_.book1D("PtvsrecoJet","# recoJets vs pt",50,0.,500.);
  h_EtavsrecoJet = ibooker_.book1D("EtavsrecoJet","# genJets vs eta",50,-5.,5.);

  ibooker_.cd();  

}


//
// -------------------------------------- functions --------------------------------------------
//

