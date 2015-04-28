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

  theCollectionName_ = ps.getParameter<std::string>("CollectionName");


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
  std::vector<std::string> tagNamesShort;
  tagNamesShort.push_back("loose");
  tagNamesShort.push_back("tight");

  ibooker_.cd();
  ibooker_.setCurrentFolder(theCollectionName_+"/Gen");

  h_truePt_genParticle = ibooker_.book1D("truePt_gen","",50,0.,500.);
  h_trueEta_genParticle = ibooker_.book1D("trueEta_gen","",50,-5.,5.);

  int histoID = 0;
  for(std::vector<std::string>::const_iterator i_shortName = tagNamesShort.begin(); i_shortName != tagNamesShort.end(); ++i_shortName){
    ibooker_.setCurrentFolder(theCollectionName_+"/"+*i_shortName+"IDIso");

    h_xPt_yPt[histoID] = ibooker_.book2D("relPt_vs_truePt", "", 50,0.,500., 100,0.,2.);
    h_xPt_yEta[histoID] = ibooker_.book2D("etaDiff_vs_truePt", "", 50,0.,500., 100,-0.5,0.5);
    h_xEta_yPt[histoID] = ibooker_.book2D("relPt_vs_trueEta", "", 50,-5.,5., 100,0.,2.);
    h_xEta_yEta[histoID] = ibooker_.book2D("etaDiff_vs_trueEta", "", 50,-5.,5., 100,-0.5,0.5);

    h_truePt_recoParticle[histoID] = ibooker_.book1D("truePt_matched","",50,0.,500.);
    h_trueEta_recoParticle[histoID] = ibooker_.book1D("trueEta_matched","",50,-5.,5.);
    
    ++histoID;
  }
  
  ibooker_.cd();  

}

//
// -------------------------------------- fill histograms --------------------------------------------
//

void MuonAnalyzer_MiniAOD::fillHisto(int histoID, std::vector<const reco::Candidate*>* recoCollection, std::vector<const reco::Candidate*>* genCollection){

  for (std::vector<const reco::Candidate*>::const_iterator i_genParticle = genCollection->begin(); i_genParticle != genCollection->end(); ++i_genParticle)
  {
     
    // Match Gen <-> Reco
    const reco::Candidate* matchedParticle = NULL;
    for (std::vector<const reco::Candidate*>::const_iterator i_recoParticle = recoCollection->begin(); i_recoParticle != recoCollection->end(); ++i_recoParticle) {
      if(deltaR(**i_recoParticle, **i_genParticle) < 0.3){
        matchedParticle = *i_recoParticle;
        break;
      }
    }

    if(!matchedParticle) continue;
  
    h_truePt_recoParticle[histoID]->Fill((*i_genParticle)->pt());
    h_trueEta_recoParticle[histoID]->Fill((*i_genParticle)->eta());

    h_xPt_yPt[histoID]->Fill((*i_genParticle)->pt(), matchedParticle->pt()/(*i_genParticle)->pt()); 
    h_xPt_yEta[histoID]->Fill((*i_genParticle)->pt(), matchedParticle->eta()-(*i_genParticle)->eta());
    h_xEta_yPt[histoID]->Fill((*i_genParticle)->eta(), matchedParticle->pt()/(*i_genParticle)->pt());
    h_xEta_yEta[histoID]->Fill((*i_genParticle)->eta(), matchedParticle->eta()-(*i_genParticle)->eta());
  }

}

//
// -------------------------------------- functions --------------------------------------------
//

void MuonAnalyzer_MiniAOD::fillHisto(int histoID, std::vector<const pat::Muon*>* recoCollection, std::vector<const pat::PackedGenParticle*>* genCollection){
  fillHisto(histoID, (std::vector<const reco::Candidate*>*) recoCollection, (std::vector<const reco::Candidate*>*) genCollection);
}
