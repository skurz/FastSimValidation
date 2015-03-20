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

# Create subset of GenJets
selectedGenJets = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("slimmedGenJets"),
    cut = cms.string("abs(eta)<2.5 && pt>30."),
    filter = cms.bool(False)
    )

# Create subset of Muons
# TightID and Isolation Cuts are performed within the Analyzer
selectedMuons = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string("abs(eta)<2.5 && pt>10. && isLooseMuon()"), 
    filter = cms.bool(False)
    )

genMuons = cms.EDFilter("PdgIdAndStatusCandViewSelector",
    src = cms.InputTag("packedGenParticles"), 
    pdgId = cms.vint32(-13, +13),
    status = cms.vint32(1),
    filter = cms.bool(False)
    )

selectedGenMuons = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("genMuons"),
    cut = cms.string("abs(eta)<2.5 && pt>10."), 
    filter = cms.bool(False)
    )

# Sequences to produce all subsets
produceSubsets = cms.Sequence(
    preSelectedRecoJets *
    selectedRecoJets *
    selectedGenJets *
    selectedMuons *
    genMuons *
    selectedGenMuons
    )
