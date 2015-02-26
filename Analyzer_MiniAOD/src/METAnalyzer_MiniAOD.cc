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

  // For now: simple Calculation (only jets considered)
  for (pat::JetCollection::const_iterator i_recojet = RecoJetCollection->begin(); i_recojet != RecoJetCollection->end(); ++i_recojet)
  {
    HT += i_recojet->pt();
  }

  if(debug_) std::cout << HT << std::endl;


  // --------------------------------------
  // N_Vertex  
  Int_t N_vertex = vertexHandle->size();

  if(debug_) std::cout << N_vertex << std::endl;


  // --------------------------------------
  // genMET
  reco::MET::LorentzVector metP4(0,0,0,0);

  // For now: simple Calculation (only jets considered)
  for (reco::GenJetCollection::const_iterator i_genjet = GenJetCollection->begin(); i_genjet != GenJetCollection->end(); ++i_genjet)
  {
      metP4 -= i_genjet->begin()->p4();
  }

  double genMET = metP4.pt();

  if(debug_) std::cout << genMET << std::endl;

  
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

  // projection: thrust axis normalized to 1
  math::XYZVector metP3(metPx, metPy, 0.);

  double metproj_thrust = metP3.Dot(thrust_axis);
  double metproj_thrust_orth = TMath::Sqrt((metP3 - metP3.Dot(thrust_axis) * thrust_axis).Mag2());

  if(debug_) std::cout << metPt << std::endl;
  if(debug_) std::cout << metproj_thrust << "; " << metproj_thrust_orth << std::endl;


  //-------------------------------
  //--- Fill Histograms
  //-------------------------------
  h_HTvsmetPt->Fill(HT, metPt);
  h_NvertexvsmetPt->Fill(N_vertex, metPt);
  h_ThrustvsmetPt->Fill(thrust, metPt);
  h_SumETvsmetPt->Fill(ET, metPt);
  h_NJetsvsmetPt->Fill(N_jets, metPt);
  h_genMETvsmetPt->Fill(genMET, metPt);

  h_HTvsmetPx->Fill(HT, metPx);
  h_NvertexvsmetPx->Fill(N_vertex, metPx);
  h_ThrustvsmetPx->Fill(thrust, metPx);
  h_SumETvsmetPx->Fill(ET, metPx);
  h_NJetsvsmetPx->Fill(N_jets, metPx);
  h_genMETvsmetPx->Fill(genMET, metPx);

  h_HTvsmetPy->Fill(HT, metPy);
  h_NvertexvsmetPy->Fill(N_vertex, metPy);
  h_ThrustvsmetPy->Fill(thrust, metPy);
  h_SumETvsmetPy->Fill(ET, metPy);
  h_NJetsvsmetPy->Fill(N_jets, metPy);
  h_genMETvsmetPy->Fill(genMET, metPy);

  h_HTvsmetProj->Fill(HT, metproj_thrust);
  h_NvertexvsmetProj->Fill(N_vertex, metproj_thrust);
  h_ThrustvsmetProj->Fill(thrust, metproj_thrust);
  h_SumETvsmetProj->Fill(ET, metproj_thrust);
  h_NJetsvsmetProj->Fill(N_jets, metproj_thrust);
  h_genMETvsmetProj->Fill(genMET, metproj_thrust);

  h_HTvsmetProjOrth->Fill(HT, metproj_thrust_orth);
  h_NvertexvsmetProjOrth->Fill(N_vertex, metproj_thrust_orth);
  h_ThrustvsmetProjOrth->Fill(thrust, metproj_thrust_orth);
  h_SumETvsmetProjOrth->Fill(ET, metproj_thrust_orth);
  h_NJetsvsmetProjOrth->Fill(N_jets, metproj_thrust_orth);
  h_genMETvsmetProjOrth->Fill(genMET, metproj_thrust_orth);


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
  ibooker_.setCurrentFolder("MET");


  h_HTvsmetPt = ibooker_.book2D("HTvsmetPt", "metPt vs HT", 100, 0., 2000., 100, 0., 2000.);
  h_NvertexvsmetPt = ibooker_.book2D("NvertexvsmetPt", "metPt vs # vertex", 100, 0., 100., 100, 0., 2000.);
  h_ThrustvsmetPt = ibooker_.book2D("ThrustvsmetPt", "metPt vs Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumETvsmetPt = ibooker_.book2D("SumETvsmetPt", "metPt vs SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJetsvsmetPt = ibooker_.book2D("NJetsvsmetPt", "metPt vs # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMETvsmetPt = ibooker_.book2D("genMETvsmetPt", "metPt vs genMET", 100, 0., 2000., 100, 0., 2000.);

  h_HTvsmetPx = ibooker_.book2D("HTvsmetPx", "metPx vs HT", 100, 0., 2000., 100, 0., 2000.);
  h_NvertexvsmetPx = ibooker_.book2D("NvertexvsmetPx", "metPx vs # vertex", 100, 0., 100., 100, 0., 2000.);
  h_ThrustvsmetPx = ibooker_.book2D("ThrustvsmetPx", "metPx vs Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumETvsmetPx = ibooker_.book2D("SumETvsmetPx", "metPx vs SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJetsvsmetPx = ibooker_.book2D("NJetsvsmetPx", "metPx vs # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMETvsmetPx = ibooker_.book2D("genMETvsmetPx", "metPx vs genMET", 100, 0., 2000., 100, 0., 2000.);

  h_HTvsmetPy = ibooker_.book2D("HTvsmetPy", "metPy vs HT", 100, 0., 2000., 100, 0., 2000.);
  h_NvertexvsmetPy = ibooker_.book2D("NvertexvsmetPy", "metPy vs # vertex", 100, 0., 100., 100, 0., 2000.);
  h_ThrustvsmetPy = ibooker_.book2D("ThrustvsmetPy", "metPy vs Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumETvsmetPy = ibooker_.book2D("SumETvsmetPy", "metPy vs SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJetsvsmetPy = ibooker_.book2D("NJetsvsmetPy", "metPy vs # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMETvsmetPy = ibooker_.book2D("genMETvsmetPy", "metPy vs genMET", 100, 0., 2000., 100, 0., 2000.);

  h_HTvsmetProj = ibooker_.book2D("HTvsmetProj", "metProj_thrust vs HT", 100, 0., 2000., 100, 0., 2000.);
  h_NvertexvsmetProj = ibooker_.book2D("NvertexvsmetProj", "metProj_thrust vs # vertex", 100, 0., 100., 100, 0., 2000.);
  h_ThrustvsmetProj = ibooker_.book2D("ThrustvsmetProj", "metProj_thrust vs Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumETvsmetProj = ibooker_.book2D("SumETvsmetProj", "metProj_thrust vs SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJetsvsmetProj = ibooker_.book2D("NJetsvsmetProj", "metProj_thrust vs # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMETvsmetProj = ibooker_.book2D("genMETvsmetProj", "metProj_thrust vs genMET", 100, 0., 2000., 100, 0., 2000.);

  h_HTvsmetProjOrth = ibooker_.book2D("HTvsmetProjOrth", "metProj_thrustOrth vs HT", 100, 0., 2000., 100, 0., 2000.);
  h_NvertexvsmetProjOrth = ibooker_.book2D("NvertexvsmetProjOrth", "metProj_thrustOrth vs # vertex", 100, 0., 100., 100, 0., 2000.);
  h_ThrustvsmetProjOrth = ibooker_.book2D("ThrustvsmetProjOrth", "metProj_thrustOrth vs Thrust", 100, 0.4, 1.1, 100, 0., 2000.);
  h_SumETvsmetProjOrth = ibooker_.book2D("SumETvsmetProjOrth", "metProj_thrustOrth vs SumET", 100, 0., 2000., 100, 0., 2000.);
  h_NJetsvsmetProjOrth = ibooker_.book2D("NJetsvsmetProjOrth", "metProj_thrustOrth vs # jets", 20, 0., 20., 100, 0., 2000.);
  h_genMETvsmetProjOrth = ibooker_.book2D("genMETvsmetProjOrth", "metProj_thrustOrth vs genMET", 100, 0., 2000., 100, 0., 2000.);

  ibooker_.cd();  

}


//
// -------------------------------------- functions --------------------------------------------
//

