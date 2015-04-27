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

  h_xPt_yPt->Fill((*i_genjet)->pt(), matchedJet->pt()/(*i_genjet)->pt());
  h_xEta_yPt->Fill((*i_genjet)->eta(), matchedJet->pt()/(*i_genjet)->pt());

  h_xPt_yEta->Fill((*i_genjet)->pt(), matchedJet->eta()-(*i_genjet)->eta());
  h_xEta_yEta->Fill((*i_genjet)->eta(), matchedJet->eta()-(*i_genjet)->eta());

  h_xPt_yPhi->Fill((*i_genjet)->pt(), matchedJet->phi()-(*i_genjet)->phi());
  h_xEta_yPhi->Fill((*i_genjet)->eta(), matchedJet->phi()-(*i_genjet)->phi());  
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

  h_truePt_genJet = ibooker_.book1D("truePt_gen","",50,0.,500.);
  h_trueEta_genJet = ibooker_.book1D("trueEta_gen","",50,-5.,5.);


  ibooker_.setCurrentFolder(theCollectionName_+"/LooseID");

  h_xPt_yPt = ibooker_.book2D("relPt_vs_truePt","",50,0.,500.,50,0.,2.);
  h_xEta_yPt = ibooker_.book2D("relPt_vs_trueEta","",50,-5.,5.,50,0.,2.);

  h_xPt_yEta = ibooker_.book2D("etaDiff_vs_truePt","",50,0.,500.,50,-0.5,0.5);
  h_xEta_yEta = ibooker_.book2D("etaDiff_vs_trueEta","",50,-5.,5.,50,-0.5,0.5);

  h_xPt_yPhi = ibooker_.book2D("phiDiff_vs_truePt","",50,0.,500.,50,-0.5,0.5);
  h_xEta_yPhi = ibooker_.book2D("phiDiff_vs_trueEta","",50,-5.,5.,50,-0.5,0.5);

  h_truePt_recoJet = ibooker_.book1D("truePt_matched","",50,0.,500.);
  h_trueEta_recoJet = ibooker_.book1D("trueEta_matched","",50,-5.,5.);


  ibooker_.cd();  

}

//
// -------------------------------------- fill histograms --------------------------------------------
//



//
// -------------------------------------- functions --------------------------------------------
//

