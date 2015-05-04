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
  theRecoMETCollection_    = consumes<pat::METCollection>(ps.getParameter<edm::InputTag>("recoMETCollection"));
  thePVCollection_         = consumes<reco::VertexCollection>(ps.getParameter<edm::InputTag>("PVCollection"));

  theCollectionName_ = ps.getParameter<std::string>("CollectionName");

  //triggerResults_          = consumes<edm::TriggerResults>(ps.getParameter<edm::InputTag>("TriggerResults"));
  //triggerObject_           = consumes<pat::TriggerObjectStandAlone>(ps.getParameter<edm::InputTag>("TriggerObject"));
  //triggerPath_x             = ps.getParameter<std::string>("TriggerPath");

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

  edm::Handle<pat::METCollection> recoMETCollection;
  e.getByToken(theRecoMETCollection_, recoMETCollection);
  if ( !recoMETCollection.isValid() )    
  {
    edm::LogError ("METAnalyzer_MiniAOD") << "invalid collection: recoMET" << "\n";
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
  double genMET = recoMETCollection->begin()->genMET()->pt();

  
  // --------------------------------------
  // thrust
  Thrust t(RecoJetCollection->begin(), RecoJetCollection->end());
  double thrust = t.thrust();
  const math::XYZVector thrust_axis = t.axis();

  if(debug_) std::cout << thrust << std::endl;
  if(debug_) std::cout << thrust_axis.Dot(thrust_axis) << std::endl;

  // --------------------------------------
  // sumet
  double sumET = recoMETCollection->begin()->sumEt();

  if(debug_) std::cout << sumET << std::endl;


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
  double metPt = recoMETCollection->begin()->pt();
  double metPx = recoMETCollection->begin()->px();
  double metPy = recoMETCollection->begin()->py();

  if(debug_)
  std::cout << genMET << "; " << metPt << std::endl;

  if(debug_)
  std::cout << sumET << "; " << HT << std::endl;

  // projection: thrust axis normalized to 1
  math::XYZVector metP3(metPx, metPy, 0.);

  double metproj_thrust = metP3.Dot(thrust_axis);
  double metproj_thrust_orth = TMath::Sqrt((metP3 - metP3.Dot(thrust_axis) * thrust_axis).Mag2());

  if(debug_) std::cout << metPt << std::endl;
  if(debug_) std::cout << metproj_thrust << "; " << metproj_thrust_orth << std::endl;

  // genMET
  double genMetPt = recoMETCollection->begin()->genMET()->pt();
  double genMetPx = recoMETCollection->begin()->genMET()->px();
  double genMetPy = recoMETCollection->begin()->genMET()->py();

  math::XYZVector genMetP3(genMetPx, genMetPy, 0.);

  double genMetproj_thrust = genMetP3.Dot(thrust_axis);
  double genMetproj_thrust_orth = TMath::Sqrt((genMetP3 - genMetP3.Dot(thrust_axis) * thrust_axis).Mag2());


  //-------------------------------
  //--- Fill Histograms
  //-------------------------------
  h_xHT_yMetPt->Fill(HT, metPt / genMetPt);
  h_xNVertex_yMetPt->Fill(N_vertex, metPt / genMetPt);
  h_xThrust_yMetPt->Fill(thrust, metPt / genMetPt);
  h_xSumET_yMetPt->Fill(sumET, metPt / genMetPt);
  h_xNJets_yMetPt->Fill(N_jets, metPt / genMetPt);
  h_xGenMET_yMetPt->Fill(genMET, metPt / genMetPt);

  h_xHT_yMetPx->Fill(HT, metPx / genMetPx);
  h_xNVertex_yMetPx->Fill(N_vertex, metPx / genMetPx);
  h_xThrust_yMetPx->Fill(thrust, metPx / genMetPx);
  h_xSumET_yMetPx->Fill(sumET, metPx / genMetPx);
  h_xNJets_yMetPx->Fill(N_jets, metPx / genMetPx);
  h_xGenMET_yMetPx->Fill(genMET, metPx / genMetPx);

  h_xHT_yMetPy->Fill(HT, metPy / genMetPy);
  h_xNVertex_yMetPy->Fill(N_vertex, metPy / genMetPy);
  h_xThrust_yMetPy->Fill(thrust, metPy / genMetPy);
  h_xSumET_yMetPy->Fill(sumET, metPy / genMetPy);
  h_xNJets_yMetPy->Fill(N_jets, metPy / genMetPy);
  h_xGenMET_yMetPy->Fill(genMET, metPy / genMetPy);

  h_xHT_yMetProj->Fill(HT, metproj_thrust / genMetproj_thrust);
  h_xNVertex_yMetProj->Fill(N_vertex, metproj_thrust / genMetproj_thrust);
  h_xThrust_yMetProj->Fill(thrust, metproj_thrust / genMetproj_thrust);
  h_xSumET_yMetProj->Fill(sumET, metproj_thrust / genMetproj_thrust);
  h_xNJets_yMetProj->Fill(N_jets, metproj_thrust / genMetproj_thrust);
  h_xGenMET_yMetProj->Fill(genMET, metproj_thrust / genMetproj_thrust);

  h_xHT_yMetProjOrth->Fill(HT, metproj_thrust_orth / genMetproj_thrust_orth);
  h_xNVertex_yMetProjOrth->Fill(N_vertex, metproj_thrust_orth / genMetproj_thrust_orth);
  h_xThrust_yMetProjOrth->Fill(thrust, metproj_thrust_orth / genMetproj_thrust_orth);
  h_xSumET_yMetProjOrth->Fill(sumET, metproj_thrust_orth / genMetproj_thrust_orth);
  h_xNJets_yMetProjOrth->Fill(N_jets, metproj_thrust_orth / genMetproj_thrust_orth);
  h_xGenMET_yMetProjOrth->Fill(genMET, metproj_thrust_orth / genMetproj_thrust_orth);


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
  ibooker_.setCurrentFolder(theCollectionName_+"/MetPt");

  h_xHT_yMetPt = ibooker_.book2D("relMetPt_vs_HT", "", 50., 0., 2000., 500.,0.,2.);
  h_xNVertex_yMetPt = ibooker_.book2D("relMetPt_vs_NVertex", "", 50., 0., 50., 500.,0.,2.);
  h_xThrust_yMetPt = ibooker_.book2D("relMetPt_vs_Thrust", "", 50., 0.4, 1.1, 500.,0.,2.);
  h_xSumET_yMetPt = ibooker_.book2D("relMetPt_vs_SumET", "", 50., 0., 2000., 500.,0.,2.);
  h_xNJets_yMetPt = ibooker_.book2D("relMetPt_vs_NJets", "", 20, 0., 20., 500.,0.,2.);
  h_xGenMET_yMetPt = ibooker_.book2D("relMetPt_vs_genMET", "", 50., 0., 2000., 500.,0.,2.);

  ibooker_.setCurrentFolder(theCollectionName_+"/MetPx");

  h_xHT_yMetPx = ibooker_.book2D("relMetPx_vs_HT", "", 50., 0., 2000., 500.,0.,2.);
  h_xNVertex_yMetPx = ibooker_.book2D("relMetPx_vs_NVertex", "", 50., 0., 50., 500.,0.,2.);
  h_xThrust_yMetPx = ibooker_.book2D("relMetPx_vs_Thrust", "", 50., 0.4, 1.1, 500.,0.,2.);
  h_xSumET_yMetPx = ibooker_.book2D("relMetPx_vs_SumET", "", 50., 0., 2000., 500.,0.,2.);
  h_xNJets_yMetPx = ibooker_.book2D("relMetPx_vs_NJets", "", 20, 0., 20., 500.,0.,2.);
  h_xGenMET_yMetPx = ibooker_.book2D("relMetPx_vs_genMET", "", 50., 0., 2000., 500.,0.,2.);

  ibooker_.setCurrentFolder(theCollectionName_+"/MetPy");

  h_xHT_yMetPy = ibooker_.book2D("relMetPy_vs_HT", "", 50., 0., 2000., 500.,0.,2.);
  h_xNVertex_yMetPy = ibooker_.book2D("relMetPy_vs_NVertex", "", 50., 0., 50., 500.,0.,2.);
  h_xThrust_yMetPy = ibooker_.book2D("relMetPy_vs_Thrust", "", 50., 0.4, 1.1, 500.,0.,2.);
  h_xSumET_yMetPy = ibooker_.book2D("relMetPy_vs_SumET", "", 50., 0., 2000., 500.,0.,2.);
  h_xNJets_yMetPy = ibooker_.book2D("relMetPy_vs_NJets", "", 20, 0., 20., 500.,0.,2.);
  h_xGenMET_yMetPy = ibooker_.book2D("relMetPy_vs_genMET", "", 50., 0., 2000., 500.,0.,2.);

  ibooker_.setCurrentFolder(theCollectionName_+"/MetProjThrust");

  h_xHT_yMetProj = ibooker_.book2D("relMetProjThrust_vs_HT", "", 50., 0., 2000., 500.,0.,2.);
  h_xNVertex_yMetProj = ibooker_.book2D("relMetProjThrust_vs_NVertex", "", 50., 0., 50., 500.,0.,2.);
  h_xThrust_yMetProj = ibooker_.book2D("relMetProjThrust_vs_Thrust", "t", 50., 0.4, 1.1, 500.,0.,2.);
  h_xSumET_yMetProj = ibooker_.book2D("relMetProjThrust_vs_SumET", "", 50., 0., 2000., 500.,0.,2.);
  h_xNJets_yMetProj = ibooker_.book2D("relMetProjThrust_vs_NJets", "", 20, 0., 20., 500.,0.,2.);
  h_xGenMET_yMetProj = ibooker_.book2D("relMetProjThrust_vs_genMET", "", 50., 0., 2000., 500.,0.,2.);

  ibooker_.setCurrentFolder(theCollectionName_+"/MetProjThrustOrth");

  h_xHT_yMetProjOrth = ibooker_.book2D("relMetProjThrustOrth_vs_HT", "", 50., 0., 2000., 500.,0.,2.);
  h_xNVertex_yMetProjOrth = ibooker_.book2D("relMetProjThrustOrth_vs_NVertex", "", 50., 0., 50., 500.,0.,2.);
  h_xThrust_yMetProjOrth = ibooker_.book2D("relMetProjThrustOrth_vs_Thrust", "", 50., 0.4, 1.1, 500.,0.,2.);
  h_xSumET_yMetProjOrth = ibooker_.book2D("relMetProjThrustOrth_vs_SumET", "", 50., 0., 2000., 500.,0.,2.);
  h_xNJets_yMetProjOrth = ibooker_.book2D("relMetProjThrustOrth_vs_NJets", "", 20, 0., 20., 500.,0.,2.);
  h_xGenMET_yMetProjOrth = ibooker_.book2D("relMetProjThrustOrth_vs_genMET", "", 50., 0., 2000., 500.,0.,2.);

  ibooker_.cd();  

}


//
// -------------------------------------- functions --------------------------------------------
//

