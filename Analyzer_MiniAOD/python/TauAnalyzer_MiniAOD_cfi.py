import FWCore.ParameterSet.Config as cms

TauAnalyzer_MiniAOD = cms.EDAnalyzer("TauAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    TauCollection        	 = cms.InputTag("selectedTaus"),
    GenTauCollection         = cms.InputTag("selectedGenTaus"),
    GenElectronCollection    = cms.InputTag("selectedGenElectrons"),
    GenMuonCollection        = cms.InputTag("selectedGenMuons"),
    GenJetCollection         = cms.InputTag("selectedGenJets"),

    TauIDs	= cms.VPSet(
    	cms.PSet(
    		idStrings	= cms.vstring("decayModeFinding", "byLooseCombinedIsolationDeltaBetaCorr3Hits", "againstElectronLoose", "againstMuonTight3"), # decayModeFindingOldDMs = decayModeFinding ??
    		cutValues	= cms.vdouble(0.5,0.5,0.5,0.5),
    		idShortName = cms.string("decayModeFindingOld+byLooseCombinedIsolation")
    		),
    	cms.PSet(
    		idStrings	= cms.vstring("decayModeFinding", "byTightIsolationMVA3oldDMwLT", "againstElectronLoose", "againstMuonTight3"),
    		cutValues	= cms.vdouble(0.5,0.5,0.5,0.5),
    		idShortName = cms.string("decayModeFindingOld+byTightIsolation")
    		),
    	cms.PSet(
    		idStrings	= cms.vstring("decayModeFinding", "againstElectronMediumMVA5", "againstElectronLoose", "againstMuonTight3"),
    		cutValues	= cms.vdouble(0.5,0.5,0.5,0.5),
    		idShortName = cms.string("decayModeFindingOld+againstElectron")
    		),
    	cms.PSet(
    		idStrings	= cms.vstring("decayModeFindingNewDMs", "byLooseCombinedIsolationDeltaBetaCorr3Hits", "againstElectronLoose", "againstMuonTight3"),
    		cutValues	= cms.vdouble(0.5,0.5,0.5,0.5),
    		idShortName = cms.string("decayModeFindingNew+byLooseCombinedIsolation")
    		)
    	),

    #Debug
    Debug                    = cms.untracked.bool(False),

)


    #  Content of tauIDs()
    #  againstElectronLoose
    #  againstElectronLooseMVA5
    #  againstElectronMVA5category
    #  againstElectronMVA5raw
    #  againstElectronMedium
    #  againstElectronMediumMVA5
    #  againstElectronTight
    #  againstElectronTightMVA5
    #  againstElectronVLooseMVA5
    #  againstElectronVTightMVA5
    #  againstMuonLoose
    #  againstMuonLoose2
    #  againstMuonLoose3
    #  againstMuonLooseMVA
    #  againstMuonMVAraw
    #  againstMuonMedium
    #  againstMuonMedium2
    #  againstMuonMediumMVA
    #  againstMuonTight
    #  againstMuonTight2
    #  againstMuonTight3
    #  againstMuonTightMVA
    #  byCombinedIsolationDeltaBetaCorrRaw3Hits
    #  byIsolationMVA3newDMwLTraw
    #  byIsolationMVA3newDMwoLTraw
    #  byIsolationMVA3oldDMwLTraw
    #  byIsolationMVA3oldDMwoLTraw
    #  byLooseCombinedIsolationDeltaBetaCorr3Hits
    #  byLooseIsolationMVA3newDMwLT
    #  byLooseIsolationMVA3newDMwoLT
    #  byLooseIsolationMVA3oldDMwLT
    #  byLooseIsolationMVA3oldDMwoLT
    #  byMediumCombinedIsolationDeltaBetaCorr3Hits
    #  byMediumIsolationMVA3newDMwLT
    #  byMediumIsolationMVA3newDMwoLT
    #  byMediumIsolationMVA3oldDMwLT
    #  byMediumIsolationMVA3oldDMwoLT
    #  byTightCombinedIsolationDeltaBetaCorr3Hits
    #  byTightIsolationMVA3newDMwLT
    #  byTightIsolationMVA3newDMwoLT
    #  byTightIsolationMVA3oldDMwLT
    #  byTightIsolationMVA3oldDMwoLT
    #  byVLooseIsolationMVA3newDMwLT
    #  byVLooseIsolationMVA3newDMwoLT
    #  byVLooseIsolationMVA3oldDMwLT
    #  byVLooseIsolationMVA3oldDMwoLT
    #  byVTightIsolationMVA3newDMwLT
    #  byVTightIsolationMVA3newDMwoLT
    #  byVTightIsolationMVA3oldDMwLT
    #  byVTightIsolationMVA3oldDMwoLT
    #  byVVTightIsolationMVA3newDMwLT
    #  byVVTightIsolationMVA3newDMwoLT
    #  byVVTightIsolationMVA3oldDMwLT
    #  byVVTightIsolationMVA3oldDMwoLT
    #  chargedIsoPtSum
    #  decayModeFinding
    #  decayModeFindingNewDMs
    #  neutralIsoPtSum
    #  puCorrPtSum
    