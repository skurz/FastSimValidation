import FWCore.ParameterSet.Config as cms

MiniAODHarvester = cms.EDAnalyzer("MiniAODHarvester",
                                  #numMonitorName = cms.string("Physics/TopTest/ElePt_leading_HLT_matched"),
                                  #denMonitorName = cms.string("Physics/TopTest/ElePt_leading")
                                  numMonitorName = cms.string("Physics/TopTest/EleEta_leading_HLT_matched"),
                                  denMonitorName = cms.string("Physics/TopTest/EleEta_leading")
                                  )
