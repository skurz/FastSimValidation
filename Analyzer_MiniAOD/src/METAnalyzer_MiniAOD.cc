#include "FastSimValidation/Analyzer_MiniAOD/interface/METAnalyzer_MiniAOD.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// Geometry
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"
#include "PhysicsTools/CandUtils/interface/Thrust.h"
#include "DataFormats/Math/interface/Vector3D.h"


#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>

//
// -------------------------------------- Constructor --------------------------------------------
//
METAnalyzer_MiniAOD::METAnalyzer_MiniAOD(const edm::ParameterSet& ps)
{
  edm::LogInfo("METAnalyzer_MiniAOD") <<  "Constructor  METAnalyzer_MiniAOD::METAnalyzer_MiniAOD " << std::endl;

  //fullSim_ = ps.getUntrackedParameter<bool>("fullSim");
  
  // Get parameters from configuration file
  theRecoJetCollection_    = consumes<pat::JetCollection>(ps.getParameter<edm::InputTag>("recoJetCollection"));
  theGenJetCollection_     = consumes<reco::GenJetCollection>(ps.getParameter<edm::InputTag>("genJetCollection"));
  theMETCollection_        = consumes<pat::METCollection>(ps.getParameter<edm::InputTag>("METCollection"));
  thePVCollection_         = consumes<reco::VertexCollection>(ps.getParameter<edm::InputTag>("PVCollection"));

  theCollectionName_ = ps.getParameter<std::string>("CollectionName");

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
METAnalyzer_MiniAOD::~METAnalyzer_MiniAOD()
{
  edm::LogInfo("METAnalyzer_MiniAOD") <<  "Destructor METAnalyzer_MiniAOD::~METAnalyzer_MiniAOD " << std::endl;
}

//
// -------------------------------------- beginRun --------------------------------------------
//
void METAnalyzer_MiniAOD::dqmBeginRun(edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("METAnalyzer_MiniAOD") <<  "METAnalyzer_MiniAOD::beginRun" << std::endl;
}
//
// -------------------------------------- bookHistos --------------------------------------------
//
void METAnalyzer_MiniAOD::bookHistograms(DQMStore::IBooker & ibooker_, edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("METAnalyzer_MiniAOD") <<  "METAnalyzer_MiniAOD::bookHistograms" << std::endl;
  
  //book at beginRun
  bookHistos(ibooker_);
}
//
// -------------------------------------- beginLuminosityBlock --------------------------------------------
//
void METAnalyzer_MiniAOD::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                      edm::EventSetup const& context) 
{
  edm::LogInfo("METAnalyzer_MiniAOD") <<  "METAnalyzer_MiniAOD::beginLuminosityBlock" << std::endl;
}


//
// -------------------------------------- Analyze --------------------------------------------
//
void METAnalyzer_MiniAOD::analyze(edm::Event const& e, edm::EventSetup const& eSetup)
{
  edm::LogInfo("METAnalyzer_MiniAOD") <<  "METAnalyzer_MiniAOD::analyze" << std::endl;

  //-------------------------------
  //--- Get Collections
  //-------------------------------
  edm::Handle<pat::JetCollection> RecoJetCollection;
  e.getByToken (theRecoJetCollection_,RecoJetCollection);
  if ( !RecoJetCollection.isValid() ) 
  {
    edm::LogError ("METAnalyzer_MiniAOD") << "invalid collection: recojets" << "\n";
    return;
  }

  edm::Handle<reco::VertexCollection> vertexHandle;
  e.getByToken(thePVCollection_, vertexHandle);
  if ( !vertexHandle.isValid() ) 
  {
    edm::LogError ("METAnalyzer_MiniAOD") << "invalid collection: vertex" << "\n";
    return;
  }

  edm::Handle<reco::GenJetCollection> GenJetCollection;
  e.getByToken (theGenJetCollection_,GenJetCollection);
  if ( !GenJetCollection.isValid() ) 
  {
    edm::LogError ("METAnalyzer_MiniAOD") << "invalid collection: genjets" << "\n";
    return;
  }

  edm::Handle<pat::METCollection> METCollection;
  e.getByToken(theMETCollection_, METCollection);
  if ( !METCollection.isValid() )    
  {
    edm::LogError ("METAnalyzer_MiniAOD") << "invalid collection: MET" << "\n";
    return;
  }


  //-------------------------------
  //--- Calculate Variables
  //-------------------------------

  // --------------------------------------
  // HT
  Double_t HT = 0;
  reco::MET::LorentzVector metP4(0,0,0,0);

  // For now: simple Calculation (only jets considered)
  for (pat::JetCollection::const_iterator i_recojet = RecoJetCollection->begin(); i_recojet != RecoJetCollection->end(); ++i_recojet)
  {
    HT += i_recojet->pt();
    metP4 -= i_recojet->begin()->p4();
  }

  double recoMET = metP4.pt();

  if(debug_) std::cout << HT << std::endl;


  // --------------------------------------
  // N_Vertex  
  Int_t N_vertex = vertexHandle->size();

  if(debug_) std::cout << N_vertex << std::endl;


  // --------------------------------------
  // genMET
  reco::MET::LorentzVector genMetP4(0,0,0,0);

  // For now: simple Calculation (only jets considered)
  for (reco::GenJetCollection::const_iterator i_genjet = GenJetCollection->begin(); i_genjet != GenJetCollection->end(); ++i_genjet)
  {
      genMetP4 -= i_genjet->begin()->p4();
  }

  double genMET = genMetP4.pt();


  
  // --------------------------------------
  // thrust
  Thrust t(RecoJetCollection->begin(), RecoJetCollection->end());
  double thrust = t.thrust();
  const math::XYZVector thrust_axis = t.axis();

  if(debug_) std::cout << thrust << std::endl;
  if(debug_) std::cout << thrust_axis.Dot(thrust_axis) << std::endl;

  // --------------------------------------
  // sumet
  Double_t ET = 0;

  // For now: simple Calculation (only jets considered)
  for (pat::JetCollection::const_iterator i_recojet = RecoJetCollection->begin(); i_recojet != RecoJetCollection->end(); ++i_recojet)
  {
    ET += i_recojet->et();
  }

  if(debug_) std::cout << ET << std::endl;

  // --------------------------------------
  // N_jets
  Int_t N_jets = 0;

  // For now: simple Calculation (only jets considered)
  for (pat::JetCollection::const_iterator i_recojet = RecoJetCollection->begin(); i_recojet != RecoJetCollection->end(); ++i_recojet)
  { 
    if(i_recojet->pt() > 30)  N_jets++;
  }

  if(debug_) std::cout << N_jets << std::endl;

  // --------------------------------------
  // MET 
  double metPt = METCollection->begin()->pt();
  double metPx = METCollection->begin()->px();
  double metPy = METCollection->begin()->py();

  if(debug_) std::cout << genMET << "; " << metPt << "; " << recoMET << std::endl;

  // projection: thrust axis normalized to 1
  math::XYZVector metP3(metPx, metPy, 0.);

  double metproj_thrust = metP3.Dot(thrust_axis);
  double metproj_thrust_orth = TMath::Sqrt((metP3 - metP3.Dot(thrust_axis) * thrust_axis).Mag2());

  if(debug_) std::cout << metPt << std::endl;
  if(debug_) std::cout << metproj_thrust << "; " << metproj_thrust_orth << std::endl;


  //-------------------------------
  //--- Fill Histograms
  //-------------------------------
  h_HT_metPt->Fill(HT, metPt);
  h_Nvertex_metPt->Fill(N_vertex, metPt);
  h_Thrust_metPt->Fill(thrust, metPt);
  h_SumET_metPt->Fill(ET, metPt);
  h_NJets_metPt->Fill(N_jets, metPt);
  h_genMET_metPt->Fill(genMET, metPt);

  h_HT_metPx->Fill(HT, metPx);
  h_Nvertex_metPx->Fill(N_vertex, metPx);
  h_Thrust_metPx->Fill(thrust, metPx);
  h_SumET_metPx->Fill(ET, metPx);
  h_NJets_metPx->Fill(N_jets, metPx);
  h_genMET_metPx->Fill(genMET, metPx);

  h_HT_metPy->Fill(HT, metPy);
  h_Nvertex_metPy->Fill(N_vertex, metPy);
  h_Thrust_metPy->Fill(thrust, metPy);
  h_SumET_metPy->Fill(ET, metPy);
  h_NJets_metPy->Fill(N_jets, metPy);
  h_genMET_metPy->Fill(genMET, metPy);

  h_HT_metProj->Fill(HT, metproj_thrust);
  h_Nvertex_metProj->Fill(N_vertex, metproj_thrust);
  h_Thrust_metProj->Fill(thrust, metproj_thrust);
  h_SumET_metProj->Fill(ET, metproj_thrust);
  h_NJets_metProj->Fill(N_jets, metproj_thrust);
  h_genMET_metProj->Fill(genMET, metproj_thrust);

  h_HT_metProjOrth->Fill(HT, metproj_thrust_orth);
  h_Nvertex_metProjOrth->Fill(N_vertex, metproj_thrust_orth);
  h_Thrust_metProjOrth->Fill(thrust, metproj_thrust_orth);
  h_SumET_metProjOrth->Fill(ET, metproj_thrust_orth);
  h_NJets_metProjOrth->Fill(N_jets, metproj_thrust_orth);
  h_genMET_metProjOrth->Fill(genMET, metproj_thrust_orth);


}
//
// -------------------------------------- endLuminosityBlock --------------------------------------------
//
void METAnalyzer_MiniAOD::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
  edm::LogInfo("METAnalyzer_MiniAOD") <<  "METAnalyzer_MiniAOD::endLuminosityBlock" << std::endl;
}


//
// -------------------------------------- endRun --------------------------------------------
//
void METAnalyzer_MiniAOD::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
  edm::LogInfo("METAnalyzer_MiniAOD") <<  "METAnalyzer_MiniAOD::endRun" << std::endl;
}


//
// -------------------------------------- book histograms --------------------------------------------
//
void METAnalyzer_MiniAOD::bookHistos(DQMStore::IBooker & ibooker_)
{
  ibooker_.cd();
  ibooker_.setCurrentFolder(theCollectionName_);


  h_HT_metPt = ibooker_.book2D("HT_metPt", "metPt _ HT", 100, 0., 2000., 100, 0., 2000.);
  h_Nvertex_metPt = ibooker_.book2D("Nvertex_metPt", "metPt _ # vertex", 100, 0., 100., 100, 0., 2000.);
  h_Thrust_metPt = ibooker_.book2D("Thrust_metPt", "metPt _ Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumET_metPt = ibooker_.book2D("SumET_metPt", "metPt _ SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJets_metPt = ibooker_.book2D("NJets_metPt", "metPt _ # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMET_metPt = ibooker_.book2D("genMET_metPt", "metPt _ genMET", 100, 0., 2000., 100, 0., 2000.);

  h_HT_metPx = ibooker_.book2D("HT_metPx", "metPx _ HT", 100, 0., 2000., 100, 0., 2000.);
  h_Nvertex_metPx = ibooker_.book2D("Nvertex_metPx", "metPx _ # vertex", 100, 0., 100., 100, 0., 2000.);
  h_Thrust_metPx = ibooker_.book2D("Thrust_metPx", "metPx _ Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumET_metPx = ibooker_.book2D("SumET_metPx", "metPx _ SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJets_metPx = ibooker_.book2D("NJets_metPx", "metPx _ # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMET_metPx = ibooker_.book2D("genMET_metPx", "metPx _ genMET", 100, 0., 2000., 100, 0., 2000.);

  h_HT_metPy = ibooker_.book2D("HT_metPy", "metPy _ HT", 100, 0., 2000., 100, 0., 2000.);
  h_Nvertex_metPy = ibooker_.book2D("Nvertex_metPy", "metPy _ # vertex", 100, 0., 100., 100, 0., 2000.);
  h_Thrust_metPy = ibooker_.book2D("Thrust_metPy", "metPy _ Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumET_metPy = ibooker_.book2D("SumET_metPy", "metPy _ SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJets_metPy = ibooker_.book2D("NJets_metPy", "metPy _ # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMET_metPy = ibooker_.book2D("genMET_metPy", "metPy _ genMET", 100, 0., 2000., 100, 0., 2000.);

  h_HT_metProj = ibooker_.book2D("HT_metProj", "metProj_thrust _ HT", 100, 0., 2000., 100, 0., 2000.);
  h_Nvertex_metProj = ibooker_.book2D("Nvertex_metProj", "metProj_thrust _ # vertex", 100, 0., 100., 100, 0., 2000.);
  h_Thrust_metProj = ibooker_.book2D("Thrust_metProj", "metProj_thrust _ Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumET_metProj = ibooker_.book2D("SumET_metProj", "metProj_thrust _ SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJets_metProj = ibooker_.book2D("NJets_metProj", "metProj_thrust _ # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMET_metProj = ibooker_.book2D("genMET_metProj", "metProj_thrust _ genMET", 100, 0., 2000., 100, 0., 2000.);

  h_HT_metProjOrth = ibooker_.book2D("HT_metProjOrth", "metProj_thrustOrth _ HT", 100, 0., 2000., 100, 0., 2000.);
  h_Nvertex_metProjOrth = ibooker_.book2D("Nvertex_metProjOrth", "metProj_thrustOrth _ # vertex", 100, 0., 100., 100, 0., 2000.);
  h_Thrust_metProjOrth = ibooker_.book2D("Thrust_metProjOrth", "metProj_thrustOrth _ Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumET_metProjOrth = ibooker_.book2D("SumET_metProjOrth", "metProj_thrustOrth _ SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJets_metProjOrth = ibooker_.book2D("NJets_metProjOrth", "metProj_thrustOrth _ # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMET_metProjOrth = ibooker_.book2D("genMET_metProjOrth", "metProj_thrustOrth _ genMET", 100, 0., 2000., 100, 0., 2000.);

  ibooker_.cd();  

}


//
// -------------------------------------- functions --------------------------------------------
//

