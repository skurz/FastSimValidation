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
  theGenParticleCollection_  = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenMuonCollection"));
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
  edm::Handle<reco::CandidateCollection> genParticleCollection;
  e.getByToken(theGenParticleCollection_, genParticleCollection);
  if ( !genParticleCollection.isValid() ) 
  {
    edm::LogError ("MuonAnalyzer_MiniAOD") << "invalid collection: genParticles" << "\n";
    return;
  }

  std::vector<const pat::PackedGenParticle*> genMuons;
  for (reco::CandidateCollection::const_iterator i_genMuon = genParticleCollection->begin(); i_genMuon != genParticleCollection->end(); ++i_genMuon)
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

  // Fill Histrograms

  // Loose
  for (std::vector<const pat::Muon *>::const_iterator i_looseMuon = looseMuons.begin(); i_looseMuon != looseMuons.end(); ++i_looseMuon)
  {
    const pat::Muon* i_looseMuonPtr = *i_looseMuon;
    
    // Match Gen <-> Reco
    const pat::PackedGenParticle* matchedGenMuon = NULL;
    for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genMuon = genMuons.begin(); i_genMuon != genMuons.end(); ++i_genMuon) {
        if(deltaR(*i_looseMuonPtr, **i_genMuon) < 0.1) matchedGenMuon = (*i_genMuon);
    }
    if(!matchedGenMuon) continue;
	
	h_LooseIDIso_PtvsrecoMuon->Fill(matchedGenMuon->pt());
	h_LooseIDIso_EtavsrecoMuon->Fill(matchedGenMuon->eta());

    h_LooseIDIso_TruePtvsFracPtTruePt->Fill(matchedGenMuon->pt(), i_looseMuonPtr->pt() / matchedGenMuon->pt());
    h_LooseIDIso_EtavsFracPtTruePt->Fill(matchedGenMuon->eta(), i_looseMuonPtr->pt() / matchedGenMuon->pt());
  }

  // Tight
  for (std::vector<const pat::Muon *>::const_iterator i_tightMuon = looseMuons.begin(); i_tightMuon != looseMuons.end(); ++i_tightMuon)
  {
    const pat::Muon* i_tightMuonPtr = *i_tightMuon;
    
    // Match Gen <-> Reco
    const pat::PackedGenParticle* matchedGenMuon = NULL;
    for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genMuon = genMuons.begin(); i_genMuon != genMuons.end(); ++i_genMuon) {
        if(deltaR(*i_tightMuonPtr, **i_genMuon) < 0.1) matchedGenMuon = (*i_genMuon);
    }
    if(!matchedGenMuon) continue;
	
	h_TightIDIso_PtvsrecoMuon->Fill(matchedGenMuon->pt());
	h_TightIDIso_EtavsrecoMuon->Fill(matchedGenMuon->eta());

    h_TightIDIso_TruePtvsFracPtTruePt->Fill(matchedGenMuon->pt(), i_tightMuonPtr->pt() / matchedGenMuon->pt());
    h_TightIDIso_EtavsFracPtTruePt->Fill(matchedGenMuon->eta(), i_tightMuonPtr->pt() / matchedGenMuon->pt());
  }
  
  for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genMuon = genMuons.begin(); i_genMuon != genMuons.end(); ++i_genMuon) 
  {
    h_PtvsgenMuon->Fill((*i_genMuon)->pt());
    h_EtavsgenMuon->Fill((*i_genMuon)->eta());
  }


  if(debug_ && genMuons.size() > 0) std::cout << genMuons.size() << "; " << looseMuons.size() << "; " << tightMuons.size() << std::endl;

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


  h_LooseIDIso_TruePtvsFracPtTruePt = ibooker_.book2D("LooseIDIso_TruePtvsFracPtTruePt", "TruePt vs Pt / TruePt for Loose ID/Iso", 50,0.,500., 20, 0.8, 1.2);
  h_LooseIDIso_EtavsFracPtTruePt = ibooker_.book2D("LooseIDIso_EtavsFracPtTruePt", "Eta vs Pt / TruePt for Loose ID/Iso", 50,-5.,5., 20, 0.8, 1.2);

  h_TightIDIso_TruePtvsFracPtTruePt = ibooker_.book2D("TightIDIso_TruePtvsFracPtTruePt", "TruePt vs Pt / TruePt for Tight ID/Iso", 50,0.,500., 20, 0.8, 1.2);
  h_TightIDIso_EtavsFracPtTruePt = ibooker_.book2D("TightIDIso_EtavsFracPtTruePt", "Eta vs Pt / TruePt for Tight ID/Iso", 50,-5.,5., 20, 0.8, 1.2);

  ibooker_.setCurrentFolder("Muon/Helpers");
  
  h_PtvsgenMuon = ibooker_.book1D("PtvsgenMuon","# genMuons vs pt",50,0.,500.);
  h_EtavsgenMuon = ibooker_.book1D("EtavsgenMuon","# genMuons vs eta",50,-5.,5.);
  h_LooseIDIso_PtvsrecoMuon = ibooker_.book1D("LooseIDIso_PtvsrecoMuon","# recoMuons vs pt for Loose ID/Iso",50,0.,500.);
  h_LooseIDIso_EtavsrecoMuon = ibooker_.book1D("LooseIDIso_EtavsrecoMuon","# recoMuons vs eta for Loose ID/Iso",50,-5.,5.);
  h_TightIDIso_PtvsrecoMuon = ibooker_.book1D("TightIDIso_PtvsrecoMuon","# recoMuons vs pt for Tight ID/Iso",50,0.,500.);
  h_TightIDIso_EtavsrecoMuon = ibooker_.book1D("TightIDIso_EtavsrecoMuon","# recoMuons vs eta for Tight ID/Iso",50,-5.,5.);
  
  ibooker_.cd();  

}


//
// -------------------------------------- functions --------------------------------------------
//

