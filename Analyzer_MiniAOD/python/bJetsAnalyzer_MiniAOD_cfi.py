import FWCore.ParameterSet.Config as cms

bJetsAnalyzer_MiniAOD = cms.EDAnalyzer("bJetsAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    recoJetCollection        = cms.InputTag("selectedRecoJets"),
    genJetCollection         = cms.InputTag("selectedGenJets"),
    prunedGenParticleCollection = cms.InputTag("prunedGenParticles"),

    bJetIDs	= cms.VPSet(
      cms.PSet(
  		  idString	= cms.string("combinedInclusiveSecondaryVertexV2BJetTags"),
  		  cutValue	= cms.double(0.5),
  		  idShortName = cms.string("CISVV2")
  		),
	),

    CollectionName = cms.string("ak4PFJetsCHS_bTag"),
    ConeSize = cms.double(0.4),

    #Debug
    Debug                 = cms.untracked.bool(False),

)

  # Content of MiniAOD
  # jetBProbabilityBJetTags
  # jetProbabilityBJetTags
  # trackCountingHighPurBJetTags
  # trackCountingHighEffBJetTags
  # simpleSecondaryVertexHighEffBJetTags
  # simpleSecondaryVertexHighPurBJetTags
  # combinedInclusiveSecondaryVertexV2BJetTags
  # pfCombinedSecondaryVertexBJetTags
  # combinedMVABJetTags
