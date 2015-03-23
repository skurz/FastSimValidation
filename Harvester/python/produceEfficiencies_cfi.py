import FWCore.ParameterSet.Config as cms

produceJetEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Jets"),
                                          efficiency = cms.vstring(
                                              "jetIdPtvsEff 'Jet ID Efficiency vs Pt' Helpers/PtvsrecoJet Helpers/PtvsgenJet",
                                              "jetIdEtavsEff 'Jet ID Efficiency vs Eta' Helpers/EtavsrecoJet Helpers/EtavsgenJet"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

produceMuonEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Muon"),
                                          efficiency = cms.vstring(
                                              "looseMuonIDIsoPtvsEff 'Muon IDIso Efficiency vs Pt for loose Muon ID/Iso' Helpers/LooseIDIso_PtvsrecoMuon Helpers/PtvsgenMuon",
                                              "looseMuonIDIsoEtavsEff 'Muon IDIso Efficiency vs Eta for loose Muon ID/Iso' Helpers/LooseIDIso_EtavsrecoMuon Helpers/EtavsgenMuon",
                                              "tightMuonIDIsoPtvsEff 'Muon IDIso Efficiency vs Pt for tight Muon ID/Iso' Helpers/TightIDIso_PtvsrecoMuon Helpers/PtvsgenMuon",
                                              "tightMuonIDIsoEtavsEff 'Muon IDIso Efficiency vs Eta for tight Muon ID/Iso' Helpers/TightIDIso_EtavsrecoMuon Helpers/EtavsgenMuon"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

produceElectronEff = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Electron"),
                                          efficiency = cms.vstring(
                                              "vetoElectronIDPtvsEff 'Electron ID Efficiency vs Pt for veto Electron ID/Iso' Helpers/VetoID_PtvsrecoElectron Helpers/PtvsgenElectron",
                                              "vetoElectronIDEtavsEff 'Electron ID Efficiency vs Eta for veto Electron ID/Iso' Helpers/VetoID_EtavsrecoElectron Helpers/EtavsgenElectron",
                                              "looseElectronIDPtvsEff 'Electron ID Efficiency vs Pt for loose Electron ID/Iso' Helpers/LooseID_PtvsrecoElectron Helpers/PtvsgenElectron",
                                              "looseElectronIDEtavsEff 'Electron ID Efficiency vs Eta for loose Electron ID/Iso' Helpers/LooseID_EtavsrecoElectron Helpers/EtavsgenElectron",
                                              "mediumElectronIDPtvsEff 'Electron ID Efficiency vs Pt for medium Electron ID/Iso' Helpers/MediumID_PtvsrecoElectron Helpers/PtvsgenElectron",
                                              "mediumElectronIDEtavsEff 'Electron ID Efficiency vs Eta for medium Electron ID/Iso' Helpers/MediumID_EtavsrecoElectron Helpers/EtavsgenElectron",
                                              "tightElectronIDPtvsEff 'Electron ID Efficiency vs Pt for tight Electron ID/Iso' Helpers/TightID_PtvsrecoElectron Helpers/PtvsgenElectron",
                                              "tightElectronIDEtavsEff 'Electron ID Efficiency vs Eta for tight Electron ID/Iso' Helpers/TightID_EtavsrecoElectron Helpers/EtavsgenElectron"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )


# Sequences to produce all efficiency plots
produceEfficiencies = cms.Sequence(
    produceJetEff *
    produceMuonEff *
    produceElectronEff
    )