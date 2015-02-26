import FWCore.ParameterSet.Config as cms

JetsAnalyzer_MiniAOD = cms.EDAnalyzer("JetsAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    recoJetCollection        = cms.InputTag("slimmedJets"),
    genJetCollection         = cms.InputTag("slimmedGenJets"),

    #Cuts
    PtThrJet                 = cms.untracked.double(20.0),
    EtaThrJet                 = cms.untracked.double(5.0),

    #Debug
    Debug                 = cms.untracked.bool(False),

)

