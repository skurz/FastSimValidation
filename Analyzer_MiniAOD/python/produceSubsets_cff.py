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

# Create subset of prunedGenParticles for bJet identification
selectedPrunedGenParticles = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("prunedGenParticles"),
    cut = cms.string("abs(eta)<2.5 && pt>30."),
    filter = cms.bool(False)
    )

produceJets = cms.Sequence(
    preSelectedRecoJets *
    selectedRecoJets *
    selectedGenJets
    )


# Create subset of Muons
# TightID and Isolation Cuts are performed within the Analyzer
selectedMuons = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string("abs(eta)<2.5 && pt>15."),  #&& isLooseMuon()
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
    cut = cms.string("abs(eta)<2.5 && pt>15."), 
    filter = cms.bool(False)
    )

produceMuons = cms.Sequence(
    selectedMuons *
    genMuons *
    selectedGenMuons
    )


# Create subset of Electrons
# veto/loose/medium/tight working points are selected within the analyzer
selectedElectrons = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("slimmedElectrons"),
    cut = cms.string("abs(eta)<2.5 && pt>15."), 
    filter = cms.bool(False)
    )

genElectrons = cms.EDFilter("PdgIdAndStatusCandViewSelector",
    src = cms.InputTag("packedGenParticles"), 
    pdgId = cms.vint32(-11, +11),
    status = cms.vint32(1),
    filter = cms.bool(False)
    )

selectedGenElectrons = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("genElectrons"),
    cut = cms.string("abs(eta)<2.5 && pt>15."), 
    filter = cms.bool(False)
    )

produceElectrons = cms.Sequence(
    selectedElectrons *
    genElectrons *
    selectedGenElectrons
    )

# Create subset of Photons
# loose/medium/tight working points are selected within the analyzer
selectedPhotons = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("slimmedPhotons"),
    cut = cms.string("abs(eta)<2.5 && pt>20."), 
    filter = cms.bool(False)
    )

genPhotons = cms.EDFilter("PdgIdAndStatusCandViewSelector",
    src = cms.InputTag("packedGenParticles"), 
    pdgId = cms.vint32(22),
    status = cms.vint32(1),
    filter = cms.bool(False)
    )

selectedGenPhotons = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("genPhotons"),
    cut = cms.string("abs(eta)<2.5 && pt>20."), 
    filter = cms.bool(False)
    )

producePhotons = cms.Sequence(
    selectedPhotons *
    genPhotons *
    selectedGenPhotons
    )

# Create subset of Taus
# loose/medium/tight working points are selected within the analyzer
selectedTaus = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("slimmedTaus"),
    cut = cms.string("abs(eta)<2.5 && pt>20."), 
    filter = cms.bool(False)
    )

genTaus = cms.EDFilter("PdgIdAndStatusCandViewSelector",
    src = cms.InputTag("prunedGenParticles"), 
    pdgId = cms.vint32(-15, 15),
    status = cms.vint32(3),
    filter = cms.bool(False)
    )

selectedGenTaus = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("genTaus"),
    cut = cms.string("abs(eta)<2.5 && pt>20."), 
    filter = cms.bool(False)
    )

produceTaus = cms.Sequence(
    selectedTaus *
    genTaus *
    selectedGenTaus
    )


# Sequences to produce all subsets
produceSubsets = cms.Sequence(
    produceJets *
    produceMuons *
    produceElectrons *
    producePhotons *
    produceTaus
    )
