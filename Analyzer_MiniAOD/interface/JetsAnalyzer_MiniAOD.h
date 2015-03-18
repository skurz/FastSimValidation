#ifndef JetsAnalyzer_MiniAOD_H
#define JetsAnalyzer_MiniAOD_H

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

// Jets
#include "DataFormats/PatCandidates/interface/Jet.h"

// Trigger
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"

// PtrVector
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"
 
class JetsAnalyzer_MiniAOD: public DQMEDAnalyzer{

public:

  JetsAnalyzer_MiniAOD(const edm::ParameterSet& ps);
  virtual ~JetsAnalyzer_MiniAOD();
  
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


  //variables from config file
  edm::EDGetTokenT<reco::CandidateCollection> theRecoJetCollection_;
  edm::EDGetTokenT<reco::CandidateCollection> theGenJetCollection_;

  // config
  bool fullSim_;

  // debug
  bool debug_;


  // Histograms
  MonitorElement* h_PtvsjetRelPtDiff;
  MonitorElement* h_EtavsjetRelPtDiff;

  MonitorElement* h_PtvsjetEtaDiff;
  MonitorElement* h_EtavsjetEtaDiff;

  MonitorElement* h_PtvsjetPhiDiff;
  MonitorElement* h_EtavsjetPhiDiff;

  MonitorElement* h_PtvsgenJet;
  MonitorElement* h_EtavsgenJet;
  MonitorElement* h_PtvsrecoJet;
  MonitorElement* h_EtavsrecoJet;

};


#endif
