import FWCore.ParameterSet.Config as cms

# Create subset of RecoJets
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
preSelectedRecoJets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
    filterParams = pfJetIDSelector.clone(),
    idLevel = cms.string("LOOSE"),
    src = cms.InputTag("slimmedJets"),
    filter = cms.bool(False)
    )

selectedRecoJets = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("preSelectedRecoJets"),
    cut = cms.string("abs(eta)<2.5 && pt>30."),
    filter = cms.bool(False)
    )

selectedGenJets = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("slimmedGenJets"),
    cut = cms.string("abs(eta)<2.5 && pt>30."),
    filter = cms.bool(False)
    )

# Sequences to produce all subsets
produceSubsets = cms.Sequence(
    preSelectedRecoJets *
    selectedRecoJets *
    selectedGenJets
    )
