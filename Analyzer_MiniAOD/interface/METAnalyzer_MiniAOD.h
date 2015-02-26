#ifndef METAnalyzer_MiniAOD_H
#define METAnalyzer_MiniAOD_H

//Framework
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//event
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

//DQM
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//Candidate handling
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

// Electron
#include "DataFormats/PatCandidates/interface/Electron.h"

// Muon
#include "DataFormats/PatCandidates/interface/Muon.h"

// PFMET
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"

// Vertex utilities
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// CaloJets
#include "DataFormats/PatCandidates/interface/Jet.h"

// Trigger
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"

 
class METAnalyzer_MiniAOD: public DQMEDAnalyzer{

public:

  METAnalyzer_MiniAOD(const edm::ParameterSet& ps);
  virtual ~METAnalyzer_MiniAOD();
  
protected:

  void dqmBeginRun(edm::Run const &, edm::EventSetup const &) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;
  void analyze(edm::Event const& e, edm::EventSetup const& eSetup);
  void beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup) ;
  void endLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup);
  void endRun(edm::Run const& run, edm::EventSetup const& eSetup);

private:
  //histos booking function
  void bookHistos(DQMStore::IBooker &);

  //other functions


  //private variables
  math::XYZPoint PVPoint_;

  //variables from config file
  edm::EDGetTokenT<pat::ElectronCollection> theElectronCollection_;
  edm::EDGetTokenT<pat::MuonCollection> theMuonCollection_;
  edm::EDGetTokenT<pat::JetCollection> theRecoJetCollection_;
  edm::EDGetTokenT<reco::GenJetCollection> theGenJetCollection_;
  edm::EDGetTokenT<pat::METCollection> theMETCollection_;
  edm::EDGetTokenT<reco::VertexCollection> thePVCollection_;

  // cuts:
  double ptThrJet_;

  // config
  bool fullSim_;

  // debug
  bool debug_;


  // Histograms
  MonitorElement* h_HTvsmetPt;
  MonitorElement* h_NvertexvsmetPt;
  MonitorElement* h_ThrustvsmetPt;
  MonitorElement* h_SumETvsmetPt;
  MonitorElement* h_NJetsvsmetPt;
  MonitorElement* h_genMETvsmetPt;

  MonitorElement* h_HTvsmetPx;
  MonitorElement* h_NvertexvsmetPx;
  MonitorElement* h_ThrustvsmetPx;
  MonitorElement* h_SumETvsmetPx;
  MonitorElement* h_NJetsvsmetPx;
  MonitorElement* h_genMETvsmetPx;

  MonitorElement* h_HTvsmetPy;
  MonitorElement* h_NvertexvsmetPy;
  MonitorElement* h_ThrustvsmetPy;
  MonitorElement* h_SumETvsmetPy;
  MonitorElement* h_NJetsvsmetPy;
  MonitorElement* h_genMETvsmetPy;

  MonitorElement* h_HTvsmetProj;
  MonitorElement* h_NvertexvsmetProj;
  MonitorElement* h_ThrustvsmetProj;
  MonitorElement* h_SumETvsmetProj;
  MonitorElement* h_NJetsvsmetProj;
  MonitorElement* h_genMETvsmetProj;

  MonitorElement* h_HTvsmetProjOrth;
  MonitorElement* h_NvertexvsmetProjOrth;
  MonitorElement* h_ThrustvsmetProjOrth;
  MonitorElement* h_SumETvsmetProjOrth;
  MonitorElement* h_NJetsvsmetProjOrth;
  MonitorElement* h_genMETvsmetProjOrth;

};


#endif
