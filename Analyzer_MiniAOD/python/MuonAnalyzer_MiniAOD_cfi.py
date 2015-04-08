import FWCore.ParameterSet.Config as cms

MuonAnalyzer_MiniAOD = cms.EDAnalyzer("MuonAnalyzer_MiniAOD",
    #fullSim              = cms.untracked.bool(True),  #Choose 'False' for FastSim

    MuonCollection        = cms.InputTag("selectedMuons"),
    GenMuonCollection     = cms.InputTag("selectedGenMuons"),
    PVCollection          = cms.InputTag("offlineSlimmedPrimaryVertices"),

    CollectionName = cms.string("Muons"),

    #Debug
    Debug                 = cms.untracked.bool(False),

)

