#include "FastSimValidation/Analyzer_MiniAOD/interface/MuonAnalyzer_MiniAOD.h"
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
MuonAnalyzer_MiniAOD::MuonAnalyzer_MiniAOD(const edm::ParameterSet& ps)
{
  edm::LogInfo("MuonAnalyzer_MiniAOD") <<  "Constructor  MuonAnalyzer_MiniAOD::MuonAnalyzer_MiniAOD " << std::endl;

  //fullSim_ = ps.getUntrackedParameter<bool>("fullSim");
  
  // Get parameters from configuration file
  theMuonCollection_         = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("MuonCollection"));
  theGenPhotonCollection_  = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenMuonCollection"));
  thePVCollection_           = consumes<reco::VertexCollection>(ps.getParameter<edm::InputTag>("PVCollection"));


  // debug
  debug_ = ps.getUntrackedParameter<bool>("Debug");

 
}

//
// -- Destructor
//
MuonAnalyzer_MiniAOD::~MuonAnalyzer_MiniAOD()
{
  edm::LogInfo("MuonAnalyzer_MiniAOD") <<  "Destructor MuonAnalyzer_MiniAOD::~MuonAnalyzer_MiniAOD " << std::endl;
}

//
// -------------------------------------- beginRun --------------------------------------------
//
void MuonAnalyzer_MiniAOD::dqmBeginRun(edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("MuonAnalyzer_MiniAOD") <<  "MuonAnalyzer_MiniAOD::beginRun" << std::endl;
}
//
// -------------------------------------- bookHistos --------------------------------------------
//
void MuonAnalyzer_MiniAOD::bookHistograms(DQMStore::IBooker & ibooker_, edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("MuonAnalyzer_MiniAOD") <<  "MuonAnalyzer_MiniAOD::bookHistograms" << std::endl;
  
  //book at beginRun
  bookHistos(ibooker_);
}
//
// -------------------------------------- beginLuminosityBlock --------------------------------------------
//
void MuonAnalyzer_MiniAOD::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                      edm::EventSetup const& context) 
{
  edm::LogInfo("MuonAnalyzer_MiniAOD") <<  "MuonAnalyzer_MiniAOD::beginLuminosityBlock" << std::endl;
}


//
// -------------------------------------- Analyze --------------------------------------------
//
void MuonAnalyzer_MiniAOD::analyze(edm::Event const& e, edm::EventSetup const& eSetup)
{
  edm::LogInfo("MuonAnalyzer_MiniAOD") <<  "MuonAnalyzer_MiniAOD::analyze" << std::endl;

  //-------------------------------
  //--- Get Collections
  //-------------------------------

  // RecoMuons
  edm::Handle<reco::CandidateCollection> MuonCollection;
  e.getByToken (theMuonCollection_,MuonCollection);
  if ( !MuonCollection.isValid() ) 
  {
    edm::LogError ("MuonAnalyzer_MiniAOD") << "invalid collection: muons" << "\n";
    return;
  }

  std::vector<const pat::Muon*> patMuons;
  for (reco::CandidateCollection::const_iterator i_recoMuon = MuonCollection->begin(); i_recoMuon != MuonCollection->end(); ++i_recoMuon)
  {
    try{
      patMuons.push_back(dynamic_cast<const pat::Muon*>(&(*i_recoMuon)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection recomuons not of type pat::Muon" << std::endl;
    }
  }

  // GenMuons
  edm::Handle<reco::CandidateCollection> genMuonCollection;
  e.getByToken(theGenPhotonCollection_, genMuonCollection);
  if ( !genMuonCollection.isValid() ) 
  {
    edm::LogError ("MuonAnalyzer_MiniAOD") << "invalid collection: genParticles" << "\n";
    return;
  }

  std::vector<const pat::PackedGenParticle*> genMuons;
  for (reco::CandidateCollection::const_iterator i_genMuon = genMuonCollection->begin(); i_genMuon != genMuonCollection->end(); ++i_genMuon)
  {
    try{
      genMuons.push_back(dynamic_cast<const pat::PackedGenParticle*>(&(*i_genMuon)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection genmuons not of type pat::PackedGenParticle" << std::endl;
    }
  }

  // Vertices
  edm::Handle<reco::VertexCollection> vertexHandle;
  e.getByToken(thePVCollection_, vertexHandle);
  if ( !vertexHandle.isValid() ) 
  {
    edm::LogError ("METAnalyzer_MiniAOD") << "invalid collection: vertex" << "\n";
    return;
  }



  //-------------------------------
  //--- Create Subsets
  //-------------------------------
  // loose ID and Iso
  std::vector<const pat::Muon*> looseMuons;
  // tight ID and Iso
  std::vector<const pat::Muon*> tightMuons;

  // Primary Vertex: Needed for tightIso
  reco::VertexCollection::const_iterator pVertex = vertexHandle->begin();


  // Loose/Tight Muon ID/Iso
  for (std::vector<const pat::Muon *>::const_iterator i_patMuon = patMuons.begin(); i_patMuon != patMuons.end(); ++i_patMuon)
  {
    const pat::Muon* i_patMuonPtr = *i_patMuon;

    double muonIso = (i_patMuonPtr->pfIsolationR04().sumChargedParticlePt + i_patMuonPtr->pfIsolationR04().sumNeutralHadronEt + i_patMuonPtr->pfIsolationR04().sumPhotonEt) / i_patMuonPtr->pt();

    if(i_patMuonPtr->isLooseMuon() && muonIso < 0.20) looseMuons.push_back(i_patMuonPtr);
    if(i_patMuonPtr->isTightMuon(*pVertex) && muonIso < 0.12) tightMuons.push_back(i_patMuonPtr);
  }
  

  //-------------------------------
  // Fill Histrograms
  //-------------------------------

  fillHisto(0, &looseMuons, &genMuons);
  fillHisto(1, &tightMuons, &genMuons);
  
  // Gen
  for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genMuon = genMuons.begin(); i_genMuon != genMuons.end(); ++i_genMuon) 
  {
    h_truePt_genParticle->Fill((*i_genMuon)->pt());
    h_trueEta_genParticle->Fill((*i_genMuon)->eta());
  }


  if(debug_ && genMuons.size() > 0) std::cout << genMuons.size() << "; " << looseMuons.size() << "; " << tightMuons.size() << std::endl;

}
//
// -------------------------------------- endLuminosityBlock --------------------------------------------
//
void MuonAnalyzer_MiniAOD::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
  edm::LogInfo("MuonAnalyzer_MiniAOD") <<  "MuonAnalyzer_MiniAOD::endLuminosityBlock" << std::endl;
}


//
// -------------------------------------- endRun --------------------------------------------
//
void MuonAnalyzer_MiniAOD::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
  edm::LogInfo("MuonAnalyzer_MiniAOD") <<  "MuonAnalyzer_MiniAOD::endRun" << std::endl;
}


//
// -------------------------------------- book histograms --------------------------------------------
//
void MuonAnalyzer_MiniAOD::bookHistos(DQMStore::IBooker & ibooker_)
{
  std::vector<TString> tagNamesShort;
  tagNamesShort.push_back("loose");
  tagNamesShort.push_back("tight");

  ibooker_.cd();
  ibooker_.setCurrentFolder("Muon");

  int histoID = 0;
  for(std::vector<TString>::const_iterator i_shortName = tagNamesShort.begin(); i_shortName != tagNamesShort.end(); ++i_shortName){
    h_truePt_pt[histoID] = ibooker_.book2D(*i_shortName + "IDIso_truePt_pt", "true pt vs pt for " + *i_shortName + " id/iso", 50,0.,500., 50,0.,500.);
    h_truePt_eta[histoID] = ibooker_.book2D(*i_shortName + "IDIso_truePt_eta", "true pt vs eta for " + *i_shortName + " id/iso", 50,0.,500., 50,-5.,5.);
    h_trueEta_pt[histoID] = ibooker_.book2D(*i_shortName + "IDIso_trueEta_pt", "true eta vs pt for " + *i_shortName + " id/iso", 50,-5.,5., 50,0.,500.);
    h_trueEta_eta[histoID] = ibooker_.book2D(*i_shortName + "IDIso_trueEta_eta", "true eta vs eta for " + *i_shortName + " id/iso", 50,-5.,5., 50,-5.,5.);
    ++histoID;
  }

  ibooker_.setCurrentFolder("Muon/Helpers");
  
  h_truePt_genParticle = ibooker_.book1D("truePt_genMuon","true pt vs total# genMuons",50,0.,500.);
  h_trueEta_genParticle = ibooker_.book1D("trueEta_genMuon","true eta vs total# genMuons",50,-5.,5.);

  histoID = 0;
  for(std::vector<TString>::const_iterator i_shortName = tagNamesShort.begin(); i_shortName != tagNamesShort.end(); ++i_shortName){
    h_truePt_recoParticle[histoID] = ibooker_.book1D(*i_shortName + "IDIso_truePt_recoMuon","true pt vs total# recoMuons for " + *i_shortName + " id/iso",50,0.,500.);
    h_trueEta_recoParticle[histoID] = ibooker_.book1D(*i_shortName + "IDIso_trueEta_recoMuon","true eta vs total# recoMuons for " + *i_shortName + " id/iso",50,-5.,5.);
    ++histoID;
  }
  
  ibooker_.cd();  

}

//
// -------------------------------------- fill histograms --------------------------------------------
//

void MuonAnalyzer_MiniAOD::fillHisto(int histoID, std::vector<const reco::Candidate*>* recoCollection, std::vector<const reco::Candidate*>* genCollection){

   for (std::vector<const reco::Candidate*>::const_iterator i_recoParticle = recoCollection->begin(); i_recoParticle != recoCollection->end(); ++i_recoParticle)
  {
     
    // Match Gen <-> Reco
    const reco::Candidate* matchedGenParticle = NULL;
    for (std::vector<const reco::Candidate*>::const_iterator i_genParticle = genCollection->begin(); i_genParticle != genCollection->end(); ++i_genParticle) {
        if(deltaR(**i_recoParticle, **i_genParticle) < 0.3) matchedGenParticle = (*i_genParticle);
    }
    if(!matchedGenParticle) continue;
  
    h_truePt_recoParticle[histoID]->Fill(matchedGenParticle->pt());
    h_trueEta_recoParticle[histoID]->Fill(matchedGenParticle->eta());

    h_truePt_pt[histoID]->Fill(matchedGenParticle->pt(), (*i_recoParticle)->pt()); 
    h_truePt_eta[histoID]->Fill(matchedGenParticle->pt(), (*i_recoParticle)->eta());
    h_trueEta_pt[histoID]->Fill(matchedGenParticle->eta(), (*i_recoParticle)->pt());
    h_trueEta_eta[histoID]->Fill(matchedGenParticle->eta(), (*i_recoParticle)->eta());
  }

}

//
// -------------------------------------- functions --------------------------------------------
//

void MuonAnalyzer_MiniAOD::fillHisto(int histoID, std::vector<const pat::Muon*>* recoCollection, std::vector<const pat::PackedGenParticle*>* genCollection){
  fillHisto(histoID, (std::vector<const reco::Candidate*>*) recoCollection, (std::vector<const reco::Candidate*>*) genCollection);
}
