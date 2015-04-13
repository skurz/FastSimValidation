#include "FastSimValidation/Analyzer_MiniAOD/interface/ElectronAnalyzer_MiniAOD.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// Geometry
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"


#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>

//
// -------------------------------------- Constructor --------------------------------------------
//
ElectronAnalyzer_MiniAOD::ElectronAnalyzer_MiniAOD(const edm::ParameterSet& ps)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "Constructor  ElectronAnalyzer_MiniAOD::ElectronAnalyzer_MiniAOD " << std::endl;

  //fullSim_ = ps.getUntrackedParameter<bool>("fullSim");
  
  // Get parameters from configuration file
  theElectronCollection_         = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("ElectronCollection"));
  theGenElectronCollection_  = consumes<reco::CandidateCollection>(ps.getParameter<edm::InputTag>("GenElectronCollection"));
  theElectronIDs_ = ps.getParameter<std::vector<edm::ParameterSet>>("ElectronIDs");

  theCollectionName_ = ps.getParameter<std::string>("CollectionName");

  // debug
  debug_ = ps.getUntrackedParameter<bool>("Debug");
  

 
}

//
// -- Destructor
//
ElectronAnalyzer_MiniAOD::~ElectronAnalyzer_MiniAOD()
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "Destructor ElectronAnalyzer_MiniAOD::~ElectronAnalyzer_MiniAOD " << std::endl;
}

//
// -------------------------------------- beginRun --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::dqmBeginRun(edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::beginRun" << std::endl;
}
//
// -------------------------------------- bookHistos --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::bookHistograms(DQMStore::IBooker & ibooker_, edm::Run const &, edm::EventSetup const &)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::bookHistograms" << std::endl;
  
  //book at beginRun
  bookHistos(ibooker_);
}
//
// -------------------------------------- beginLuminosityBlock --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                      edm::EventSetup const& context) 
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::beginLuminosityBlock" << std::endl;
}


//
// -------------------------------------- Analyze --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::analyze(edm::Event const& e, edm::EventSetup const& eSetup)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::analyze" << std::endl;

  //-------------------------------
  //--- Get Collections
  //-------------------------------

  // RecoElectrons
  edm::Handle<reco::CandidateCollection> ElectronCollection;
  e.getByToken (theElectronCollection_,ElectronCollection);
  if ( !ElectronCollection.isValid() ) 
  {
    edm::LogError ("ElectronAnalyzer_MiniAOD") << "invalid collection: Electrons" << "\n";
    return;
  }

  std::vector<const pat::Electron*> patElectrons;
  for (reco::CandidateCollection::const_iterator i_recoElectron = ElectronCollection->begin(); i_recoElectron != ElectronCollection->end(); ++i_recoElectron)
  {
    try{
      patElectrons.push_back(dynamic_cast<const pat::Electron*>(&(*i_recoElectron)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection recoElectrons not of type pat::Electron" << std::endl;
    }
  }

  // GenElectrons
  edm::Handle<reco::CandidateCollection> genElectronCollection;
  e.getByToken(theGenElectronCollection_, genElectronCollection);
  if ( !genElectronCollection.isValid() ) 
  {
    edm::LogError ("ElectronAnalyzer_MiniAOD") << "invalid collection: genElectrons" << "\n";
    return;
  }

  std::vector<const pat::PackedGenParticle*> genElectrons;
  for (reco::CandidateCollection::const_iterator i_genElectron = genElectronCollection->begin(); i_genElectron != genElectronCollection->end(); ++i_genElectron)
  {
    try{
      genElectrons.push_back(dynamic_cast<const pat::PackedGenParticle*>(&(*i_genElectron)));
    }catch(std::bad_cast &e){
      std::cout << e.what() << ": collection genElectrons not of type pat::PackedGenParticle" << std::endl;
    }
  }

  // ElectronIDs
  std::vector<std::pair<std::string, double>> idPairs;
  for (std::vector<edm::ParameterSet>::const_iterator it = theElectronIDs_.begin(); it != theElectronIDs_.end(); ++it) {
    idPairs.push_back(std::make_pair(it->getParameter<std::string>("idString"), it->getParameter<double>("cutValue")));
  }


  //-------------------------------
  //--- Create Subsets
  //-------------------------------
  // Create subset for every ID
  std::vector<std::vector<const pat::Electron*>> idElectrons;

  for(std::vector<std::pair<std::string, double>>::const_iterator i_elID = idPairs.begin(); i_elID != idPairs.end(); ++i_elID){

    std::vector<const pat::Electron*> passedEls;
    for (std::vector<const pat::Electron *>::const_iterator i_patElectron = patElectrons.begin(); i_patElectron != patElectrons.end(); ++i_patElectron){
      if((*i_patElectron)->electronID(i_elID->first) > i_elID->second){
        passedEls.push_back((*i_patElectron));
      }
    }

    idElectrons.push_back(passedEls);
  }

  if(debug_)
  for (std::vector<const pat::Electron *>::const_iterator i_patElectron = patElectrons.begin(); i_patElectron != patElectrons.end(); ++i_patElectron)
  {    
    for(std::vector<std::pair<std::string,float>>::const_iterator i_elIDs = (*i_patElectron)->electronIDs().begin(); i_elIDs != (*i_patElectron)->electronIDs().end(); ++i_elIDs){
      std::cout << i_elIDs->first << std::endl;
    }
  }   
  

  //-------------------------------
  // Fill Histrograms
  //-------------------------------

  int histID = 0;
  for(std::vector<std::vector<const pat::Electron*>>::iterator i_idElectrons = idElectrons.begin(); i_idElectrons != idElectrons.end(); ++i_idElectrons){
    fillHisto(histID, &(*i_idElectrons), &genElectrons);
    //std::cout << (&(*it))->size() << std::endl;
    ++histID;
  }

  // Gen
  for (std::vector<const pat::PackedGenParticle*>::const_iterator i_genElectron = genElectrons.begin(); i_genElectron != genElectrons.end(); ++i_genElectron) 
  {
    h_truePt_genParticle->Fill((*i_genElectron)->pt());
    h_trueEta_genParticle->Fill((*i_genElectron)->eta());
  }


  //if(debug_ && genElectrons.size() > 0) std::cout << genElectrons.size() << "; " << looseElectrons.size() << "; " << tightElectrons.size() << std::endl;


}
//
// -------------------------------------- endLuminosityBlock --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::endLuminosityBlock" << std::endl;
}


//
// -------------------------------------- endRun --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
  edm::LogInfo("ElectronAnalyzer_MiniAOD") <<  "ElectronAnalyzer_MiniAOD::endRun" << std::endl;
}


//
// -------------------------------------- book histograms --------------------------------------------
//
void ElectronAnalyzer_MiniAOD::bookHistos(DQMStore::IBooker & ibooker_)
{
  std::vector<std::string> tagNamesShort;
  for (std::vector<edm::ParameterSet>::const_iterator it = theElectronIDs_.begin(); it != theElectronIDs_.end(); ++it) {
    tagNamesShort.push_back(it->getParameter<std::string>("idShortName"));
  }

  ibooker_.cd();

  int histoID = 0;
  for(std::vector<std::string>::const_iterator i_shortName = tagNamesShort.begin(); i_shortName != tagNamesShort.end(); ++i_shortName){
    ibooker_.setCurrentFolder(theCollectionName_+"/"+(*i_shortName)+"ID");

    h_truePt_pt[histoID] = ibooker_.book2D("truePt_vs_pt", "true pt vs (reco pt - true pt) / (true pt) for " + *i_shortName + " id", 50,0.,500., 50,-1.,1.);
    h_truePt_eta[histoID] = ibooker_.book2D("truePt_vs_eta", "true pt vs (reco eta - true eta) for " + *i_shortName + " id", 50,0.,500., 50,-0.5,0.5);
    h_trueEta_pt[histoID] = ibooker_.book2D("trueEta_vs_pt", "true eta vs (reco pt - true pt) / (true pt) for " + *i_shortName + " id", 50,-5.,5., 50,-1.,1.);
    h_trueEta_eta[histoID] = ibooker_.book2D("trueEta_vs_eta", "true eta vs (reco eta - true eta) for " + *i_shortName + " id", 50,-5.,5., 50,-0.5,0.5);

    h_truePt_recoParticle[histoID] = ibooker_.book1D("truePt_matched","true pt vs total # matchedElectrons for " + *i_shortName + " id",50,0.,500.);
    h_trueEta_recoParticle[histoID] = ibooker_.book1D("trueEta_matched","true eta vs total # matchedElectrons for " + *i_shortName + " id",50,-5.,5.);
    ++histoID;
  }
  
  ibooker_.setCurrentFolder(theCollectionName_+"/Gen");

  h_truePt_genParticle = ibooker_.book1D("truePt_gen","true pt vs total # genElectrons",50,0.,500.);
  h_trueEta_genParticle = ibooker_.book1D("trueEta_gen","true eta vs total # genElectrons",50,-5.,5.);
  
  ibooker_.cd();  
}

//
// -------------------------------------- fill histograms --------------------------------------------
//

void ElectronAnalyzer_MiniAOD::fillHisto(int histoID, std::vector<const reco::Candidate*>* recoCollection, std::vector<const reco::Candidate*>* genCollection){

  for (std::vector<const reco::Candidate*>::const_iterator i_genParticle = genCollection->begin(); i_genParticle != genCollection->end(); ++i_genParticle){
     
    // Match Gen <-> Reco
    const reco::Candidate* matchedParticle = NULL;
    for (std::vector<const reco::Candidate*>::const_iterator i_recoParticle = recoCollection->begin(); i_recoParticle != recoCollection->end(); ++i_recoParticle){
      if(deltaR(**i_recoParticle, **i_genParticle) < 0.3){
        matchedParticle = *i_recoParticle;
        break;
      }
    }
    if(!matchedParticle) continue;
  
    h_truePt_recoParticle[histoID]->Fill((*i_genParticle)->pt());
    h_trueEta_recoParticle[histoID]->Fill((*i_genParticle)->eta());

    h_truePt_pt[histoID]->Fill((*i_genParticle)->pt(), (matchedParticle->pt()-(*i_genParticle)->pt())/(*i_genParticle)->pt()); 
    h_truePt_eta[histoID]->Fill((*i_genParticle)->pt(), matchedParticle->eta()-(*i_genParticle)->eta());
    h_trueEta_pt[histoID]->Fill((*i_genParticle)->eta(), (matchedParticle->pt()-(*i_genParticle)->pt())/(*i_genParticle)->pt());
    h_trueEta_eta[histoID]->Fill((*i_genParticle)->eta(), matchedParticle->eta()-(*i_genParticle)->eta());
  }

}


//
// -------------------------------------- functions --------------------------------------------
//

void ElectronAnalyzer_MiniAOD::fillHisto(int histoID, std::vector<const pat::Electron*>* recoCollection, std::vector<const pat::PackedGenParticle*>* genCollection){
  fillHisto(histoID, (std::vector<const reco::Candidate*>*) recoCollection, (std::vector<const reco::Candidate*>*) genCollection);
}