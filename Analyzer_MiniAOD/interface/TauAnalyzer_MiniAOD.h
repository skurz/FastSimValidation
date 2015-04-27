#ifndef TauAnalyzer_MiniAOD_H
#define TauAnalyzer_MiniAOD_H

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

// Tau
#include "DataFormats/PatCandidates/interface/Tau.h"

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

 
class TauAnalyzer_MiniAOD: public DQMEDAnalyzer{

public:

  TauAnalyzer_MiniAOD(const edm::ParameterSet& ps);
  virtual ~TauAnalyzer_MiniAOD();
  
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
  void fillHisto(std::string matchedParticle, int histoID, std::vector<const reco::Candidate*>* recoCollection, std::vector<const reco::Candidate*>* genCollection);

  //other functions
  void fillHisto(std::string matchedParticle, int histoID, std::vector<const pat::Tau*>* recoCollection, std::vector<const pat::PackedGenParticle*>* genCollection);
  void fillHisto(std::string matchedParticle, int histoID, std::vector<const pat::Tau*>* recoCollection, std::vector<const reco::GenJet*>* genCollection);
  void fillHisto(std::string matchedParticle, int histoID, std::vector<const pat::Tau*>* recoCollection, std::vector<const reco::GenParticle*>* genCollection);


  //private variables

  //variables from config file
  edm::EDGetTokenT<reco::CandidateCollection> theTauCollection_;
  edm::EDGetTokenT<reco::CandidateCollection> theGenTauCollection_;
  edm::EDGetTokenT<reco::CandidateCollection> theGenMuonCollection_;
  edm::EDGetTokenT<reco::CandidateCollection> theGenElectronCollection_;
  edm::EDGetTokenT<reco::CandidateCollection> theGenJetCollection_;

  std::vector<edm::ParameterSet> theTauIDs_;

  std::string theCollectionName_;
  

  // cuts:

  // config
  bool fullSim_;

  // debug
  bool debug_;


  // Histograms
  MonitorElement* h_xPt_yPt[4][20];
  MonitorElement* h_xPt_yEta[4][20];
  MonitorElement* h_xEta_yPt[4][20];
  MonitorElement* h_xEta_yEta[4][20];

  MonitorElement* h_truePt_recoParticle[4][20];
  MonitorElement* h_trueEta_recoParticle[4][20];

  MonitorElement* h_truePt_genParticle[4];
  MonitorElement* h_trueEta_genParticle[4];
  
};


#endif
