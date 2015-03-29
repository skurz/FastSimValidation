import FWCore.ParameterSet.Config as cms

ElectronAnalyzer_MiniAOD = cms.EDAnalyzer("ElectronAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    ElectronCollection        = cms.InputTag("selectedElectrons"),
    GenElectronCollection     = cms.InputTag("selectedGenElectrons"),

    ElectronIDs	= cms.VPSet(
    	cms.PSet(
    		idString	= cms.string("cutBasedElectronID-CSA14-PU20bx25-V0-standalone-veto"),
    		idShortName = cms.string("veto"),
    		cutValue	= cms.double(0.5)
    		),
    	cms.PSet(
    		idString	= cms.string("cutBasedElectronID-CSA14-PU20bx25-V0-standalone-loose"),
    		idShortName = cms.string("loose"),
    		cutValue	= cms.double(0.5)
    		),
    	cms.PSet(
    		idString	= cms.string("cutBasedElectronID-CSA14-PU20bx25-V0-standalone-medium"),
    		idShortName = cms.string("medium"),
    		cutValue	= cms.double(0.5)
    		),
    	cms.PSet(
    		idString	= cms.string("cutBasedElectronID-CSA14-PU20bx25-V0-standalone-tight"),
    		idShortName = cms.string("tight"),
    		cutValue	= cms.double(0.5)
    		)
    	),


    #Debug
    Debug                     = cms.untracked.bool(False)

)

#      Content of electronIDs()
#      cutBasedElectronID-CSA14-50ns-V1-standalone-loose  -> (0,1)
#      cutBasedElectronID-CSA14-50ns-V1-standalone-medium -> (0,1)
#      cutBasedElectronID-CSA14-50ns-V1-standalone-tight -> (0,1)
#      cutBasedElectronID-CSA14-50ns-V1-standalone-veto -> (0,1)
#      cutBasedElectronID-CSA14-PU20bx25-V0-standalone-loose -> (0,1)
#      cutBasedElectronID-CSA14-PU20bx25-V0-standalone-medium -> (0,1)
#      cutBasedElectronID-CSA14-PU20bx25-V0-standalone-tight -> (0,1)
#      cutBasedElectronID-CSA14-PU20bx25-V0-standalone-veto -> (0,1)
#      eidLoose
#      eidRobustHighEnergy
#      eidRobustLoose
#      eidRobustTight
#      eidTight
#      heepElectronID-HEEPV50-CSA14-25ns
#      heepElectronID-HEEPV50-CSA14-startup
      