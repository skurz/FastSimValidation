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

// GenMET
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

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
  edm::EDGetTokenT<pat::JetCollection> theRecoJetCollection_;
  edm::EDGetTokenT<pat::METCollection> theRecoMETCollection_;
  edm::EDGetTokenT<reco::VertexCollection> thePVCollection_;

  std::string theCollectionName_;

  // debug
  bool debug_;


  // Histograms
  MonitorElement* h_xHT_yMetPt;
  MonitorElement* h_xNVertex_yMetPt;
  MonitorElement* h_xThrust_yMetPt;
  MonitorElement* h_xSumET_yMetPt;
  MonitorElement* h_xNJets_yMetPt;
  MonitorElement* h_xGenMET_yMetPt;

  MonitorElement* h_xHT_yMetPx;
  MonitorElement* h_xNVertex_yMetPx;
  MonitorElement* h_xThrust_yMetPx;
  MonitorElement* h_xSumET_yMetPx;
  MonitorElement* h_xNJets_yMetPx;
  MonitorElement* h_xGenMET_yMetPx;

  MonitorElement* h_xHT_yMetPy;
  MonitorElement* h_xNVertex_yMetPy;
  MonitorElement* h_xThrust_yMetPy;
  MonitorElement* h_xSumET_yMetPy;
  MonitorElement* h_xNJets_yMetPy;
  MonitorElement* h_xGenMET_yMetPy;

  MonitorElement* h_xHT_yMetProj;
  MonitorElement* h_xNVertex_yMetProj;
  MonitorElement* h_xThrust_yMetProj;
  MonitorElement* h_xSumET_yMetProj;
  MonitorElement* h_xNJets_yMetProj;
  MonitorElement* h_xGenMET_yMetProj;

  MonitorElement* h_xHT_yMetProjOrth;
  MonitorElement* h_xNVertex_yMetProjOrth;
  MonitorElement* h_xThrust_yMetProjOrth;
  MonitorElement* h_xSumET_yMetProjOrth;
  MonitorElement* h_xNJets_yMetProjOrth;
  MonitorElement* h_xGenMET_yMetProjOrth;

};


#endif
