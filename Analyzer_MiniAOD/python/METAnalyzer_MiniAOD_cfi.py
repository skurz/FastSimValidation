import FWCore.ParameterSet.Config as cms

METAnalyzer_MiniAOD = cms.EDAnalyzer("METAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    recoJetCollection        = cms.InputTag("slimmedJets"),
    recoMETCollection        = cms.InputTag("slimmedMETs"),
    PVCollection             = cms.InputTag("offlineSlimmedPrimaryVertices"),
    
    #Trigger: To be implemented!
    #TriggerResults           = cms.InputTag('TriggerResults','','HLT'),
    #TriggerObject            = cms.InputTag('TriggerObjectStandAlone','','PAT'),
    #TriggerPath              = cms.InputTag('','',''),


    CollectionName = cms.string("MET"),
    

    #Debug
    Debug                 = cms.untracked.bool(False),

)

