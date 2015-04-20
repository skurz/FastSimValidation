import FWCore.ParameterSet.Config as cms

bJetsAnalyzer_MiniAOD = cms.EDAnalyzer("bJetsAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    recoJetCollection        = cms.InputTag("selectedRecoJets"),
    genJetCollection         = cms.InputTag("selectedGenJets"),
    prunedGenParticleCollection = cms.InputTag("prunedGenParticles"),

    bJetIDs	= cms.VPSet(
      cms.PSet(
  		  idString	= cms.string("combinedSecondaryVertexBJetTags"),
  		  cutValue	= cms.double(0.244),
  		  idShortName = cms.string("CSVL")
  		),
      cms.PSet(
        idString  = cms.string("combinedSecondaryVertexBJetTags"),
        cutValue  = cms.double(0.679),
        idShortName = cms.string("CSVM")
      ),
      cms.PSet(
        idString  = cms.string("combinedSecondaryVertexBJetTags"),
        cutValue  = cms.double(0.898),
        idShortName = cms.string("CSVT")
      ),
	),

    CollectionName = cms.string("ak4PFJetsCHS_bTag"),
    ConeSize = cms.double(0.4),

    #Debug
    Debug                 = cms.untracked.bool(False),

)

  # Content of MiniAOD
  # combinedSecondaryVertexBJetTags
  # pfJetBProbabilityBJetTags
  # pfJetProbabilityBJetTags
  # pfTrackCountingHighPurBJetTags
  # pfTrackCountingHighEffBJetTags
  # pfSimpleSecondaryVertexHighEffBJetTags
  # pfSimpleSecondaryVertexHighPurBJetTags
  # pfCombinedSecondaryVertexV2BJetTags
  # pfCombinedInclusiveSecondaryVertexV2BJetTags
  # pfCombinedSecondaryVertexSoftLeptonBJetTags
  # pfCombinedMVABJetTags

