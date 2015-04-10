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

  theCollectionName_ = ps.getParameter<std::string>("CollectionName");

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


//-------------------------------
// Fill Histrograms
//-------------------------------

for (std::vector<const reco::GenJet*>::const_iterator i_genjet = genJets.begin(); i_genjet != genJets.end(); ++i_genjet){

  // Match Gen <-> Reco
  const pat::Jet* matchedJet = NULL;
  for (std::vector<const pat::Jet *>::const_iterator i_patJet = patJets.begin(); i_patJet != patJets.end(); ++i_patJet)  {
    if(deltaR(**i_patJet, **i_genjet) < 0.3){
      matchedJet = (*i_patJet);
      break;
    }
  }
  if(!matchedJet) continue;

  // Fill histograms 
  h_truePt_recoJet->Fill((*i_genjet)->pt());
  h_trueEta_recoJet->Fill((*i_genjet)->eta());

  h_truePt_jetRelPtDiff->Fill((*i_genjet)->pt(), (matchedJet->pt()-(*i_genjet)->pt())/(*i_genjet)->pt());
  h_trueEta_jetRelPtDiff->Fill((*i_genjet)->eta(), (matchedJet->pt()-(*i_genjet)->pt())/(*i_genjet)->pt());

  h_truePt_jetEtaDiff->Fill((*i_genjet)->pt(), matchedJet->eta()-(*i_genjet)->eta());
  h_trueEta_jetEtaDiff->Fill((*i_genjet)->eta(), matchedJet->eta()-(*i_genjet)->eta());

  h_truePt_jetPhiDiff->Fill((*i_genjet)->pt(), matchedJet->phi()-(*i_genjet)->phi());
  h_trueEta_jetPhiDiff->Fill((*i_genjet)->eta(), matchedJet->phi()-(*i_genjet)->phi());  
}

// Gen
for (std::vector<const reco::GenJet*>::const_iterator i_genjet = genJets.begin(); i_genjet != genJets.end(); ++i_genjet) 
{
    h_truePt_genJet->Fill((*i_genjet)->pt());
    h_trueEta_genJet->Fill((*i_genjet)->eta());
}


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

  ibooker_.setCurrentFolder(theCollectionName_+"/Gen");

  h_truePt_genJet = ibooker_.book1D("truePt_gen","true pt vs total # genJets",50,0.,500.);
  h_trueEta_genJet = ibooker_.book1D("trueEta_gen","true eta vs total # genJets",50,-5.,5.);


  ibooker_.setCurrentFolder(theCollectionName_+"/LooseID");

  h_truePt_jetRelPtDiff = ibooker_.book2D("truePt_vs_jetRelPtDiff","true pt vs (reco pt - true pt) / (true pt) for loose id",50,0.,500.,20,-1.,1.);
  h_trueEta_jetRelPtDiff = ibooker_.book2D("trueEta_vs_jetRelPtDiff","true eta vs (reco pt - true pt) / (true pt) for loose id",50,-5.,5.,20,-1.,1.);

  h_truePt_jetEtaDiff = ibooker_.book2D("truePt_vs_jetEtaDiff","true pt vs (reco eta - true eta) for loose id",50,0.,500.,20,-0.5,0.5);
  h_trueEta_jetEtaDiff = ibooker_.book2D("trueEta_vs_jetEtaDiff","true eta vs (reco eta - true eta) for loose id",50,-5.,5.,20,-0.5,0.5);

  h_truePt_jetPhiDiff = ibooker_.book2D("truePt_vs_jetPhiDiff","true pt vs (reco phi - true phi) for loose id",50,0.,500.,20,-0.5,0.5);
  h_trueEta_jetPhiDiff = ibooker_.book2D("trueEta_vs_jetPhiDiff","true eta vs (reco phi - true phi) for loose id",50,-5.,5.,20,-0.5,0.5);

  h_truePt_recoJet = ibooker_.book1D("truePt_matched","true pt vs total # matchedJets for loose id",50,0.,500.);
  h_trueEta_recoJet = ibooker_.book1D("trueEta_matched","true eta vs total # matchedJets for loose id",50,-5.,5.);


  ibooker_.cd();  

}

//
// -------------------------------------- fill histograms --------------------------------------------
//



//
// -------------------------------------- functions --------------------------------------------
//

