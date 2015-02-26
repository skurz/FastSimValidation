#include "FastSimValidation/Harvester/interface/MiniAODHarvester.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>

//
// -------------------------------------- Constructor --------------------------------------------
//
MiniAODHarvester::MiniAODHarvester(const edm::ParameterSet& ps)
{
  edm::LogInfo("MiniAODHarvester") <<  "Constructor  MiniAODHarvester::MiniAODHarvester " << std::endl;

  // Get parameters from configuration file
  numMonitorName_      =  ps.getParameter<std::string>("numMonitorName");
  denMonitorName_      =  ps.getParameter<std::string>("denMonitorName");

}

//
// -- Destructor
//
MiniAODHarvester::~MiniAODHarvester()
{
  edm::LogInfo("MiniAODHarvester") <<  "Destructor MiniAODHarvester::~MiniAODHarvester " << std::endl;
}

//
// -------------------------------------- beginJob --------------------------------------------
//
void MiniAODHarvester::beginJob()
{
  edm::LogInfo("MiniAODHarvester") <<  "MiniAODHarvester::beginJob " << std::endl;
}
//
// -------------------------------------- get and book in the endJob --------------------------------------------
//
void MiniAODHarvester::dqmEndJob(DQMStore::IBooker& ibooker_, DQMStore::IGetter& igetter_)
{
  // create and cd into new folder
  ibooker_.setCurrentFolder("What_I_do_in_the_client/Ratio");

  //get available histograms
  MonitorElement* numerator = igetter_.get(numMonitorName_);
  MonitorElement* denominator = igetter_.get(denMonitorName_);

  if (!numerator || !denominator)
    {
      edm::LogError("MiniAODHarvester") <<  "MEs not found!" << std::endl;
      return;
    }


  //book new histogram
  h_ptRatio = ibooker_.book1D("test","Test",50,-5.,5.);
  h_ptRatio->setAxisTitle("eta");

  for (int iBin=1; iBin<numerator->getNbinsX(); ++iBin)
    {
      if(denominator->getBinContent(iBin) == 0)
	h_ptRatio->setBinContent(iBin, 0.);
      else
	h_ptRatio->setBinContent(iBin, numerator->getBinContent(iBin) / denominator->getBinContent(iBin));
    }
}

//
// -------------------------------------- get in the endLumi if needed --------------------------------------------
//
void MiniAODHarvester::dqmEndLuminosityBlock(DQMStore::IBooker & ibooker_, DQMStore::IGetter & igetter_, edm::LuminosityBlock const & iLumi, edm::EventSetup const& iSetup) 
{
  edm::LogInfo("MiniAODHarvester") <<  "MiniAODHarvester::endLumi " << std::endl;
}
