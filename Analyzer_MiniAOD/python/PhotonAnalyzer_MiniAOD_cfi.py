import FWCore.ParameterSet.Config as cms

PhotonAnalyzer_MiniAOD = cms.EDAnalyzer("PhotonAnalyzer_MiniAOD",
    #fullSim                  = cms.untracked.bool(True),  #Choose 'False' for FastSim

    PhotonCollection        = cms.InputTag("selectedPhotons"),
    GenPhotonCollection     = cms.InputTag("selectedGenPhotons"),

    PhotonIDs	= cms.VPSet(
    	cms.PSet(
    		idString	= cms.string("PhotonCutBasedIDLoose"),
    		idShortName = cms.string("loose"),
    		),
    	cms.PSet(
    		idString	= cms.string("PhotonCutBasedIDTight"),
    		idShortName = cms.string("tight"),
    		),
    	),

    CollectionName = cms.string("Photons"),
    
    #Debug
    Debug                     = cms.untracked.bool(False),

)


    #  Content of photonIDs()
    #  No cutValues nedded!
    #  PhotonCutBasedIDLoose -> (true,false)
    #  PhotonCutBasedIDTight -> (true,false)

    #  MediumID not stored (at least in MiniAOD) -> define selection cuts manually?
      
