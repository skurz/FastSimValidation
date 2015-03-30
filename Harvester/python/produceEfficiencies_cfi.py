import FWCore.ParameterSet.Config as cms

produceJetEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Jets"),
                                          efficiency = cms.vstring(
                                              "jetIdPtvsEff 'Jet ID Efficiency vs Pt' Helpers/truePt_recoJet Helpers/truePt_genJet",
                                              "jetIdEtavsEff 'Jet ID Efficiency vs Eta' Helpers/trueEta_recoJet Helpers/trueEta_genJet"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

produceMuonEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Muon"),
                                          efficiency = cms.vstring(
                                              "looseMuonIDIsoPtvsEff 'Muon IDIso Efficiency vs Pt for loose Muon ID/Iso' Helpers/looseIDIso_truePt_recoMuon Helpers/truePt_genMuon",
                                              "looseMuonIDIsoEtavsEff 'Muon IDIso Efficiency vs Eta for loose Muon ID/Iso' Helpers/looseIDIso_trueEta_recoMuon Helpers/trueEta_genMuon",
                                              "tightMuonIDIsoPtvsEff 'Muon IDIso Efficiency vs Pt for tight Muon ID/Iso' Helpers/tightIDIso_truePt_recoMuon Helpers/truePt_genMuon",
                                              "tightMuonIDIsoEtavsEff 'Muon IDIso Efficiency vs Eta for tight Muon ID/Iso' Helpers/tightIDIso_trueEta_recoMuon Helpers/trueEta_genMuon"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

produceElectronEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Electron"),
                                          efficiency = cms.vstring(
                                              "vetoElectronIDPtvsEff 'Electron ID Efficiency vs Pt for veto Electron ID' Helpers/vetoID_truePt_recoElectron Helpers/truePt_genElectron",
                                              "vetoElectronIDEtavsEff 'Electron ID Efficiency vs Eta for veto Electron ID' Helpers/vetoID_trueEta_recoElectron Helpers/trueEta_genElectron",
                                              "looseElectronIDPtvsEff 'Electron ID Efficiency vs Pt for loose Electron ID' Helpers/looseID_truePt_recoElectron Helpers/truePt_genElectron",
                                              "looseElectronIDEtavsEff 'Electron ID Efficiency vs Eta for loose Electron ID' Helpers/looseID_trueEta_recoElectron Helpers/trueEta_genElectron",
                                              "mediumElectronIDPtvsEff 'Electron ID Efficiency vs Pt for medium Electron ID' Helpers/mediumID_truePt_recoElectron Helpers/truePt_genElectron",
                                              "mediumElectronIDEtavsEff 'Electron ID Efficiency vs Eta for medium Electron ID' Helpers/mediumID_trueEta_recoElectron Helpers/trueEta_genElectron",
                                              "tightElectronIDPtvsEff 'Electron ID Efficiency vs Pt for tight Electron ID' Helpers/tightID_truePt_recoElectron Helpers/truePt_genElectron",
                                              "tightElectronIDEtavsEff 'Electron ID Efficiency vs Eta for tight Electron ID' Helpers/tightID_trueEta_recoElectron Helpers/trueEta_genElectron"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

producePhotonEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Photon"),
                                          efficiency = cms.vstring(
                                              "loosePhotonIDPtvsEff 'Photon ID Efficiency vs Pt for loose Photon ID' Helpers/looseID_truePt_recoPhoton Helpers/truePt_genPhoton",
                                              "loosePhotonIDEtavsEff 'Photon ID Efficiency vs Eta for loose Photon ID' Helpers/looseID_trueEta_recoPhoton Helpers/trueEta_genPhoton",
                                              "tightPhotonIDPtvsEff 'Photon ID Efficiency vs Pt for tight Photon ID' Helpers/tightID_truePt_recoPhoton Helpers/truePt_genPhoton",
                                              "tightPhotonIDEtavsEff 'Photon ID Efficiency vs Eta for tight Photon ID' Helpers/tightID_trueEta_recoPhoton Helpers/trueEta_genPhoton"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

producePhotonEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Tau"),
                                          efficiency = cms.vstring(
                                              "matchedTau_decayModeFindingOld+byLooseCombinedIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedTau for decayModeFindingOld+byLooseCombinedIsolation Tau ID' Helpers/matchedTau_decayModeFindingOld+byLooseCombinedIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedTau_decayModeFindingOld+byLooseCombinedIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedTau for decayModeFindingOld+byLooseCombinedIsolation Tau ID' Helpers/matchedTau_decayModeFindingOld+byLooseCombinedIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedTau_decayModeFindingOld+byTightIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedTau for decayModeFindingOld+byTightIsolation Tau ID' Helpers/matchedTau_decayModeFindingOld+byTightIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedTau_decayModeFindingOld+byTightIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedTau for decayModeFindingOld+byTightIsolation Tau ID' Helpers/matchedTau_decayModeFindingOld+byTightIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedTau_decayModeFindingOld+againstElectronTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedTau for decayModeFindingOld+againstElectron Tau ID' Helpers/matchedTau_decayModeFindingOld+againstElectronID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedTau_decayModeFindingOld+againstElectronTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedTau for decayModeFindingOld+againstElectron Tau ID' Helpers/matchedTau_decayModeFindingOld+againstElectronID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedTau_decayModeFindingNew+byLooseCombinedIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedTau for decayModeFindingNew+byLooseCombinedIsolation Tau ID' Helpers/matchedTau_decayModeFindingNew+byLooseCombinedIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedTau_decayModeFindingNew+byLooseCombinedIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedTau for decayModeFindingNew+byLooseCombinedIsolation Tau ID' Helpers/matchedTau_decayModeFindingNew+byLooseCombinedIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedEl_decayModeFindingOld+byLooseCombinedIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedEl for decayModeFindingOld+byLooseCombinedIsolation Tau ID' Helpers/matchedEl_decayModeFindingOld+byLooseCombinedIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedEl_decayModeFindingOld+byLooseCombinedIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedEl for decayModeFindingOld+byLooseCombinedIsolation Tau ID' Helpers/matchedEl_decayModeFindingOld+byLooseCombinedIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedEl_decayModeFindingOld+byTightIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedEl for decayModeFindingOld+byTightIsolation Tau ID' Helpers/matchedEl_decayModeFindingOld+byTightIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedEl_decayModeFindingOld+byTightIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedEl for decayModeFindingOld+byTightIsolation Tau ID' Helpers/matchedEl_decayModeFindingOld+byTightIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedEl_decayModeFindingOld+againstElectronTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedEl for decayModeFindingOld+againstElectron Tau ID' Helpers/matchedEl_decayModeFindingOld+againstElectronID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedEl_decayModeFindingOld+againstElectronTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedEl for decayModeFindingOld+againstElectron Tau ID' Helpers/matchedEl_decayModeFindingOld+againstElectronID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedEl_decayModeFindingNew+byLooseCombinedIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedEl for decayModeFindingNew+byLooseCombinedIsolation Tau ID' Helpers/matchedEl_decayModeFindingNew+byLooseCombinedIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedEl_decayModeFindingNew+byLooseCombinedIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedEl for decayModeFindingNew+byLooseCombinedIsolation Tau ID' Helpers/matchedEl_decayModeFindingNew+byLooseCombinedIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedMu_decayModeFindingOld+byLooseCombinedIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedMu for decayModeFindingOld+byLooseCombinedIsolation Tau ID' Helpers/matchedMu_decayModeFindingOld+byLooseCombinedIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedMu_decayModeFindingOld+byLooseCombinedIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedMu for decayModeFindingOld+byLooseCombinedIsolation Tau ID' Helpers/matchedMu_decayModeFindingOld+byLooseCombinedIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedMu_decayModeFindingOld+byTightIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedMu for decayModeFindingOld+byTightIsolation Tau ID' Helpers/matchedMu_decayModeFindingOld+byTightIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedMu_decayModeFindingOld+byTightIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedMu for decayModeFindingOld+byTightIsolation Tau ID' Helpers/matchedMu_decayModeFindingOld+byTightIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedMu_decayModeFindingOld+againstElectronTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedMu for decayModeFindingOld+againstElectron Tau ID' Helpers/matchedMu_decayModeFindingOld+againstElectronID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedMu_decayModeFindingOld+againstElectronTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedMu for decayModeFindingOld+againstElectron Tau ID' Helpers/matchedMu_decayModeFindingOld+againstElectronID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedMu_decayModeFindingNew+byLooseCombinedIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedMu for decayModeFindingNew+byLooseCombinedIsolation Tau ID' Helpers/matchedMu_decayModeFindingNew+byLooseCombinedIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedMu_decayModeFindingNew+byLooseCombinedIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedMu for decayModeFindingNew+byLooseCombinedIsolation Tau ID' Helpers/matchedMu_decayModeFindingNew+byLooseCombinedIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedJet_decayModeFindingOld+byLooseCombinedIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedJet for decayModeFindingOld+byLooseCombinedIsolation Tau ID' Helpers/matchedJet_decayModeFindingOld+byLooseCombinedIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedJet_decayModeFindingOld+byLooseCombinedIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedJet for decayModeFindingOld+byLooseCombinedIsolation Tau ID' Helpers/matchedJet_decayModeFindingOld+byLooseCombinedIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedJet_decayModeFindingOld+byTightIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedJet for decayModeFindingOld+byTightIsolation Tau ID' Helpers/matchedJet_decayModeFindingOld+byTightIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedJet_decayModeFindingOld+byTightIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedJet for decayModeFindingOld+byTightIsolation Tau ID' Helpers/matchedJet_decayModeFindingOld+byTightIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedJet_decayModeFindingOld+againstElectronTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedJet for decayModeFindingOld+againstElectron Tau ID' Helpers/matchedJet_decayModeFindingOld+againstElectronID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedJet_decayModeFindingOld+againstElectronTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedJet for decayModeFindingOld+againstElectron Tau ID' Helpers/matchedJet_decayModeFindingOld+againstElectronID_trueEta_recoTau Helpers/trueEta_genTau",
                                              "matchedJet_decayModeFindingNew+byLooseCombinedIsolationTauIDPtvsEff 'Tau ID Efficiency vs Pt for matchedJet for decayModeFindingNew+byLooseCombinedIsolation Tau ID' Helpers/matchedJet_decayModeFindingNew+byLooseCombinedIsolationID_truePt_recoTau Helpers/truePt_genTau",
                                              "matchedJet_decayModeFindingNew+byLooseCombinedIsolationTauIDEtavsEff 'Tau ID Efficiency vs Eta for matchedJet for decayModeFindingNew+byLooseCombinedIsolation Tau ID' Helpers/matchedJet_decayModeFindingNew+byLooseCombinedIsolationID_trueEta_recoTau Helpers/trueEta_genTau",
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )


# Sequences to produce all efficiency plots
produceEfficiencies = cms.Sequence(
    produceJetEff *
    produceMuonEff *
    produceElectronEff *
    producePhotonEff
    )