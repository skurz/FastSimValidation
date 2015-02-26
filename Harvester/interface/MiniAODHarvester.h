#ifndef MiniAODHarvester_H
#define MiniAODHarvester_H

//Framework
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//DQM
#include "DQMServices/Core/interface/DQMEDHarvester.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
 
class MiniAODHarvester: public DQMEDHarvester{

public:

  MiniAODHarvester(const edm::ParameterSet& ps);
  virtual ~MiniAODHarvester();
  
protected:

  void beginJob();
  void dqmEndLuminosityBlock(DQMStore::IBooker &, DQMStore::IGetter &, edm::LuminosityBlock const &, edm::EventSetup const&);  //performed in the endLumi
  void dqmEndJob(DQMStore::IBooker &, DQMStore::IGetter &) override;  //performed in the endJob

private:

  //private variables

  //variables from config file
  std::string numMonitorName_;
  std::string denMonitorName_;

  // Histograms
  MonitorElement* h_ptRatio;

};


#endif
