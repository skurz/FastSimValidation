import FWCore.ParameterSet.Config as cms

ElectronAnalyzer_MiniAOD = cms.EDAnalyzer("ElectronAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    ElectronCollection                  = cms.InputTag("selectedElectrons"),
    GenElectronCollection     = cms.InputTag("selectedGenElectrons"),

    #Debug
    Debug                 = cms.untracked.bool(False),

)

