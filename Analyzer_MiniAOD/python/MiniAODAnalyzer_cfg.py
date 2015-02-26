import FWCore.ParameterSet.Config as cms

def MiniAODAnalyzer(process,
                    testfileName,
                    isFullSim,
                    numProcessedEvt):

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

    # my analyzers
    process.load('FastSimValidation.Analyzer_MiniAOD.JetsAnalyzer_MiniAOD_cfi')
    process.load('FastSimValidation.Analyzer_MiniAOD.METAnalyzer_MiniAOD_cfi')
    process.load('FastSimValidation.Analyzer_MiniAOD.MuonAnalyzer_MiniAOD_cfi')


    process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(numProcessedEvt)
    )


    # Input source
    process.source = cms.Source("PoolSource",
        secondaryFileNames = cms.untracked.vstring(),
        fileNames = cms.untracked.vstring(testfileName)
    )

    # Define properties of output
    if isFullSim:
    	outFile_ = "Analyzer_MiniAOD_FullSim.root"
        print "Dataset: FullSim"
    else:
        outFile_ = "Analyzer_MiniAOD_FastSim.root"
        print "Dataset: FastSim"    	

    #process.JetsAnalyzer_MiniAOD.fullSim = cms.untracked.bool(isFullSim)
    #process.METAnalyzer_MiniAOD.fullSim = cms.untracked.bool(isFullSim)


    process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
                                         fileName = cms.untracked.string(outFile_))

    # Other statements
    from Configuration.AlCa.GlobalTag import GlobalTag
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')  #for MC
    

    # Path and EndPath definitions
    process.dqmoffline_step = cms.Path(process.JetsAnalyzer_MiniAOD *
                                        process.METAnalyzer_MiniAOD *
                                        process.MuonAnalyzer_MiniAOD
        )
    process.DQMoutput_step = cms.EndPath(process.DQMoutput)


    # Schedule definition
    process.schedule = cms.Schedule(
        process.dqmoffline_step,
        process.DQMoutput_step
        )
