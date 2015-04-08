import FWCore.ParameterSet.Config as cms

JetsAnalyzer_MiniAOD = cms.EDAnalyzer("JetsAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    recoJetCollection        = cms.InputTag("selectedRecoJets"),
    genJetCollection         = cms.InputTag("selectedGenJets"),

    CollectionName = cms.string("ak4PFJetsCHS"),
    
    #Debug
    Debug                 = cms.untracked.bool(False),

)

