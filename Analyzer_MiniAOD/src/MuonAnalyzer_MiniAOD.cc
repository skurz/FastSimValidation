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
  theMuonCollection_    = consumes<pat::MuonCollection>(ps.getParameter<edm::InputTag>("MuonCollection"));
  theGenParticleCollection_     = consumes<pat::PackedGenParticleCollection>(ps.getParameter<edm::InputTag>("packedGenParticleCollection"));
  thePVCollection_         = consumes<reco::VertexCollection>(ps.getParameter<edm::InputTag>("PVCollection"));
  
  //triggerResults_          = consumes<edm::TriggerResults>(ps.getParameter<edm::InputTag>("TriggerResults"));
  //triggerObject_           = consumes<pat::TriggerObjectStandAlone>(ps.getParameter<edm::InputTag>("TriggerObject"));
  //triggerPath_             = ps.getParameter<std::string>("TriggerPath");

  // cuts:


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
  edm::Handle<pat::MuonCollection> MuonCollection;
  e.getByToken (theMuonCollection_,MuonCollection);
  if ( !MuonCollection.isValid() ) 
  {
    edm::LogError ("MuonAnalyzer_MiniAOD") << "invalid collection: muons" << "\n";
    return;
  }

  edm::Handle<pat::PackedGenParticleCollection> genParticleCollection;
  e.getByToken(theGenParticleCollection_, genParticleCollection);
  if ( !genParticleCollection.isValid() ) 
  {
    edm::LogError ("MuonAnalyzer_MiniAOD") << "invalid collection: genParticles" << "\n";
    return;
  }

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

  std::vector<pat::PackedGenParticle> genMuons;
  std::vector<pat::Muon> recoMuons; 
  std::vector<pat::Muon> looselooseMuons;
  std::vector<pat::Muon> loosetightMuons;
  std::vector<pat::Muon> tightlooseMuons; 
  std::vector<pat::Muon> tighttightMuons; 

  // Find genMuons
  for (pat::PackedGenParticleCollection::const_iterator i_genParticle = genParticleCollection->begin(); i_genParticle != genParticleCollection->end(); ++i_genParticle)
  {
    if(i_genParticle->pt() < 10 || abs(i_genParticle->eta()) > 2.5) continue;

    if(abs(i_genParticle->pdgId()) == 13 && i_genParticle->status() == 1)    genMuons.push_back(*i_genParticle);
  }

  // Loose/Tight Muon ID/Iso
  for (pat::MuonCollection::const_iterator i_recoMuon = MuonCollection->begin(); i_recoMuon != MuonCollection->end(); ++i_recoMuon)
  {
    double muonIso = (i_recoMuon->pfIsolationR04().sumChargedParticlePt + i_recoMuon->pfIsolationR04().sumNeutralHadronEt + i_recoMuon->pfIsolationR04().sumPhotonEt) / i_recoMuon->pt();
    reco::VertexCollection::const_iterator pVertex = vertexHandle->begin();

    if(i_recoMuon->pt() < 10 || abs(i_recoMuon->eta()) > 2.5) continue;

    recoMuons.push_back(*i_recoMuon);

    if(i_recoMuon->isLooseMuon()){
      if(muonIso < 0.20) looselooseMuons.push_back(*i_recoMuon);
      if(muonIso < 0.12) loosetightMuons.push_back(*i_recoMuon);
    }
    if(i_recoMuon->isTightMuon(*pVertex)){
      if(muonIso < 0.20) tightlooseMuons.push_back(*i_recoMuon);
      if(muonIso < 0.12) tighttightMuons.push_back(*i_recoMuon);
    }
  }

  if(debug_ && genMuons.size() > 0) std::cout << genMuons.size() << "; " << looselooseMuons.size() << "; " << tighttightMuons.size() << std::endl;

  //-------------------------------
  //--- Fill Histograms
  //-------------------------------


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
  ibooker_.cd();
  ibooker_.setCurrentFolder("Muon");


  h_HTvsmetPt = ibooker_.book2D("aaa", "metPt vs HT", 100, 0., 2000., 100, 0., 2000.);

  ibooker_.cd();  

}


//
// -------------------------------------- functions --------------------------------------------
//

