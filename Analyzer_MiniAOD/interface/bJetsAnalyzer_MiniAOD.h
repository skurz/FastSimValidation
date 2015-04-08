#ifndef bJetsAnalyzer_MiniAOD_H
#define bJetsAnalyzer_MiniAOD_H

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
 
class bJetsAnalyzer_MiniAOD: public DQMEDAnalyzer{

public:

  bJetsAnalyzer_MiniAOD(const edm::ParameterSet& ps);
  virtual ~bJetsAnalyzer_MiniAOD();
  
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
  void fillHisto(std::string matchedParticle, int histoID, std::vector<const pat::Jet*>* recoCollection, std::vector<const reco::GenJet*>* genCollection);

  //other functions


  //variables from config file
  edm::EDGetTokenT<reco::CandidateCollection> theRecoJetCollection_;
  edm::EDGetTokenT<reco::CandidateCollection> theGenJetCollection_;
  edm::EDGetTokenT<reco::GenParticleCollection> thePrunedGenParticleCollection_;

  std::vector<edm::ParameterSet> theBjetIDs_;

  std::string theCollectionName_;

  double coneSize_;
  
  // config
  bool fullSim_;

  // debug
  bool debug_;


  // Histograms
  MonitorElement* h_truePt_recoParticle[3][20];
  MonitorElement* h_trueEta_recoParticle[3][20];

  MonitorElement* h_truePt_genParticle[3];
  MonitorElement* h_trueEta_genParticle[3];

};


#endif
