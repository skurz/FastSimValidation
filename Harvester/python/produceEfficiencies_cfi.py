import FWCore.ParameterSet.Config as cms

produceJetEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Jets"),
                                          efficiency = cms.vstring(
                                              "jetIdPtvsEff 'Jet ID Efficiency vs Pt' Helpers/Pt_recoJet Helpers/Pt_genJet",
                                              "jetIdEtavsEff 'Jet ID Efficiency vs Eta' Helpers/Eta_recoJet Helpers/Eta_genJet"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

produceMuonEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Muon"),
                                          efficiency = cms.vstring(
                                              "looseMuonIDIsoPtvsEff 'Muon IDIso Efficiency vs Pt for loose Muon ID/Iso' Helpers/LooseIDIso_Pt_recoMuon Helpers/Pt_genMuon",
                                              "looseMuonIDIsoEtavsEff 'Muon IDIso Efficiency vs Eta for loose Muon ID/Iso' Helpers/LooseIDIso_Eta_recoMuon Helpers/Eta_genMuon",
                                              "tightMuonIDIsoPtvsEff 'Muon IDIso Efficiency vs Pt for tight Muon ID/Iso' Helpers/TightIDIso_Pt_recoMuon Helpers/Pt_genMuon",
                                              "tightMuonIDIsoEtavsEff 'Muon IDIso Efficiency vs Eta for tight Muon ID/Iso' Helpers/TightIDIso_Eta_recoMuon Helpers/Eta_genMuon"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

produceElectronEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Electron"),
                                          efficiency = cms.vstring(
                                              "vetoElectronIDPtvsEff 'Electron ID Efficiency vs Pt for veto Electron ID/Iso' Helpers/VetoID_Pt_recoElectron Helpers/Pt_genElectron",
                                              "vetoElectronIDEtavsEff 'Electron ID Efficiency vs Eta for veto Electron ID/Iso' Helpers/VetoID_Eta_recoElectron Helpers/Eta_genElectron",
                                              "looseElectronIDPtvsEff 'Electron ID Efficiency vs Pt for loose Electron ID/Iso' Helpers/LooseID_Pt_recoElectron Helpers/Pt_genElectron",
                                              "looseElectronIDEtavsEff 'Electron ID Efficiency vs Eta for loose Electron ID/Iso' Helpers/LooseID_Eta_recoElectron Helpers/Eta_genElectron",
                                              "mediumElectronIDPtvsEff 'Electron ID Efficiency vs Pt for medium Electron ID/Iso' Helpers/MediumID_Pt_recoElectron Helpers/Pt_genElectron",
                                              "mediumElectronIDEtavsEff 'Electron ID Efficiency vs Eta for medium Electron ID/Iso' Helpers/MediumID_Eta_recoElectron Helpers/Eta_genElectron",
                                              "tightElectronIDPtvsEff 'Electron ID Efficiency vs Pt for tight Electron ID/Iso' Helpers/TightID_Pt_recoElectron Helpers/Pt_genElectron",
                                              "tightElectronIDEtavsEff 'Electron ID Efficiency vs Eta for tight Electron ID/Iso' Helpers/TightID_Eta_recoElectron Helpers/Eta_genElectron"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

producePhotonEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Photon"),
                                          efficiency = cms.vstring(
                                              "loosePhotonIDPtvsEff 'Photon ID Efficiency vs Pt for loose Photon ID/Iso' Helpers/LooseID_Pt_recoPhoton Helpers/Pt_genPhoton",
                                              "loosePhotonIDEtavsEff 'Photon ID Efficiency vs Eta for loose Photon ID/Iso' Helpers/LooseID_Eta_recoPhoton Helpers/Eta_genPhoton",
                                              "tightPhotonIDPtvsEff 'Photon ID Efficiency vs Pt for tight Photon ID/Iso' Helpers/TightID_Pt_recoPhoton Helpers/Pt_genPhoton",
                                              "tightPhotonIDEtavsEff 'Photon ID Efficiency vs Eta for tight Photon ID/Iso' Helpers/TightID_Eta_recoPhoton Helpers/Eta_genPhoton"
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