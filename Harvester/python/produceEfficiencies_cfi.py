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
                                              "looseMuonIDIsoEtavsEff 'Jet ID Efficiency vs Eta for loose Muon ID/Iso' Helpers/TightIDIso_EtavsrecoMuon Helpers/EtavsgenMuon",
                                              "tightMuonIDIsoPtvsEff 'Muon IDIso Efficiency vs Pt for tight Muon ID/Iso' Helpers/LooseIDIso_PtvsrecoMuon Helpers/PtvsgenMuon",
                                              "tightMuonIDIsoEtavsEff 'Jet ID Efficiency vs Eta for tight Muon ID/Iso' Helpers/TightIDIso_EtavsrecoMuon Helpers/EtavsgenMuon"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )

# Sequences to produce all efficiency plots
produceEfficiencies = cms.Sequence(
    produceJetEff *
    produceMuonEff
    )