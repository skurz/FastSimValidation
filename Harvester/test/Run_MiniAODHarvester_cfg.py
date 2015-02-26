import FWCore.ParameterSet.Config as cms

process = cms.Process('HARVESTING')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EDMtoMEAtRunEnd_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# load DQM
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

# my client and my Tests
process.load('FastSimValidation.Harvester.MiniAODHarvester_cfi')
process.load('FastSimValidation.Harvester.MiniAOD_GenericClient_cfi')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
isFullSim_ = True

if isFullSim_:
	fileName_ = "file:Analyzer_MiniAOD_FullSim.root"
	workflow_ = '/FullSim/MinAOD/DQM'
else:
	fileName_ = "file:Analyzer_MiniAOD_FastSim.root"
	workflow_ = '/FastSim/MinAOD/DQM'

process.source = cms.Source("DQMRootSource",
                            fileNames = cms.untracked.vstring(fileName_))


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')  #for MC



# Path and EndPath definitions
#process.myHarvesting = cms.Path(process.MiniAODHarvester_cfi)
process.jetIDeff = cms.Path(process.MiniAOD_GenericClient)
process.dqmsave_step = cms.Path(process.DQMSaver)

# Schedule definition
process.schedule = cms.Schedule(
                                #process.myHarvesting,
                                process.jetIDeff,
                                process.dqmsave_step
    )

process.dqmSaver.workflow = workflow_
process.dqmSaver.convention = 'Offline'
