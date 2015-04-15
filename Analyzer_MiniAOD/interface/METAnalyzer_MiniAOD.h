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
  MonitorElement* h_HT_metPt;
  MonitorElement* h_Nvertex_metPt;
  MonitorElement* h_Thrust_metPt;
  MonitorElement* h_SumET_metPt;
  MonitorElement* h_NJets_metPt;
  MonitorElement* h_genMET_metPt;

  MonitorElement* h_HT_metPx;
  MonitorElement* h_Nvertex_metPx;
  MonitorElement* h_Thrust_metPx;
  MonitorElement* h_SumET_metPx;
  MonitorElement* h_NJets_metPx;
  MonitorElement* h_genMET_metPx;

  MonitorElement* h_HT_metPy;
  MonitorElement* h_Nvertex_metPy;
  MonitorElement* h_Thrust_metPy;
  MonitorElement* h_SumET_metPy;
  MonitorElement* h_NJets_metPy;
  MonitorElement* h_genMET_metPy;

  MonitorElement* h_HT_metProj;
  MonitorElement* h_Nvertex_metProj;
  MonitorElement* h_Thrust_metProj;
  MonitorElement* h_SumET_metProj;
  MonitorElement* h_NJets_metProj;
  MonitorElement* h_genMET_metProj;

  MonitorElement* h_HT_metProjOrth;
  MonitorElement* h_Nvertex_metProjOrth;
  MonitorElement* h_Thrust_metProjOrth;
  MonitorElement* h_SumET_metProjOrth;
  MonitorElement* h_NJets_metProjOrth;
  MonitorElement* h_genMET_metProjOrth;

};


#endif
