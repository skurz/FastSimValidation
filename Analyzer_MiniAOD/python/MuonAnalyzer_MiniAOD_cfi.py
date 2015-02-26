import FWCore.ParameterSet.Config as cms

MuonAnalyzer_MiniAOD = cms.EDAnalyzer("MuonAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    MuonCollection                  = cms.InputTag("slimmedMuons"),
    packedGenParticleCollection           = cms.InputTag("packedGenParticles"),
    PVCollection             = cms.InputTag("offlineSlimmedPrimaryVertices"),
    
    #Trigger: To be implemented!
    #TriggerResults           = cms.InputTag('TriggerResults','','HLT'),
    #TriggerObject            = cms.InputTag('TriggerObjectStandAlone','','PAT'),
    #TriggerPath              = cms.InputTag('','',''),

    #Cuts


    #Debug
    Debug                 = cms.untracked.bool(False),

)

