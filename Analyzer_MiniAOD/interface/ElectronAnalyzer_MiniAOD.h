#ifndef ElectronAnalyzer_MiniAOD_H
#define ElectronAnalyzer_MiniAOD_H

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

//GenLevel
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

// Trigger
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"

 
class ElectronAnalyzer_MiniAOD: public DQMEDAnalyzer{

public:

  ElectronAnalyzer_MiniAOD(const edm::ParameterSet& ps);
  virtual ~ElectronAnalyzer_MiniAOD();
  
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

  //variables from config file
  edm::EDGetTokenT<reco::CandidateCollection> theElectronCollection_;
  edm::EDGetTokenT<reco::CandidateCollection> theGenParticleCollection_;

  // cuts:

  // config
  bool fullSim_;

  // debug
  bool debug_;


  // Histograms
  MonitorElement* h_VetoID_TruePtvsFracPtTruePt;
  MonitorElement* h_VetoID_EtavsFracPtTruePt;  

  MonitorElement* h_LooseID_TruePtvsFracPtTruePt;
  MonitorElement* h_LooseID_EtavsFracPtTruePt;

  MonitorElement* h_MediumID_TruePtvsFracPtTruePt;
  MonitorElement* h_MediumID_EtavsFracPtTruePt;

  MonitorElement* h_TightID_TruePtvsFracPtTruePt;
  MonitorElement* h_TightID_EtavsFracPtTruePt;

  MonitorElement* h_PtvsgenElectron;
  MonitorElement* h_EtavsgenElectron;
  MonitorElement* h_VetoID_PtvsrecoElectron;
  MonitorElement* h_VetoID_EtavsrecoElectron;
  MonitorElement* h_LooseID_PtvsrecoElectron;
  MonitorElement* h_LooseID_EtavsrecoElectron;
  MonitorElement* h_MediumID_PtvsrecoElectron;
  MonitorElement* h_MediumID_EtavsrecoElectron;
  MonitorElement* h_TightID_PtvsrecoElectron;
  MonitorElement* h_TightID_EtavsrecoElectron;
  
};


#endif
