import FWCore.ParameterSet.Config as cms

PhotonAnalyzer_MiniAOD = cms.EDAnalyzer("PhotonAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    PhotonCollection        = cms.InputTag("selectedPhotons"),
    GenPhotonCollection     = cms.InputTag("selectedGenPhotons"),

    #Debug
    Debug                     = cms.untracked.bool(False),

)

