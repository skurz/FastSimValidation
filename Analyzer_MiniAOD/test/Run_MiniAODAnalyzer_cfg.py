# Dataset
    #'/store/relval/CMSSW_7_4_0_pre1/RelValTTbar_13_AVE20/MINIAODSIM/PU_MCRUN2_73_V5_FastSim-v1/00000/32C7CF0F-BC7A-E411-B391-002618943913.root'
    #'root://xrootd-cms.infn.it//store/relval/CMSSW_7_4_0_pre1/RelValTTbar_13/MINIAODSIM/PU50ns_MCRUN2_73_V4-v1/00000/AA55FDCF-727A-E411-AFCA-002618943983.root'


import FWCore.ParameterSet.Config as cms

process = cms.Process('MINIAODDQM')

from FastSimValidation.Analyzer_MiniAOD.MiniAODAnalyzer_cfg import MiniAODAnalyzer
MiniAODAnalyzer(process,
                testfileName='root://xrootd-cms.infn.it//store/relval/CMSSW_7_4_0_pre1/RelValTTbar_13/MINIAODSIM/PU50ns_MCRUN2_73_V4-v1/00000/AA55FDCF-727A-E411-AFCA-002618943983.root',
                isFullSim=True,
                numProcessedEvt=-1)