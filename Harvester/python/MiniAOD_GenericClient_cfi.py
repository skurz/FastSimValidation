import FWCore.ParameterSet.Config as cms

MiniAOD_GenericClient = cms.EDAnalyzer("DQMGenericClient",
                                          subDirs = cms.untracked.vstring("Jets"),
                                          efficiency = cms.vstring(
                                              "jetIdPt 'Jet ID Efficiency vs Pt' Helpers/PtvsgenJet Helpers/PtvsrecoJet",
                                              "jetIdEta 'Jet ID Efficiency vs Eta' Helpers/EtavsgenJet Helpers/EtavsrecoJet"
                                              ),
                                          resolution = cms.vstring(""),
                                          verbose = cms.untracked.uint32(0),
    )
