import FWCore.ParameterSet.Config as cms

process = cms.Process("HLTHiggsOfflineAnalysis")

process.load("HLTriggerOffline.Higgs.HiggsValidation_cff")
process.load("DQMServices.Components.MEtoEDMConverter_cfi")


##############################################################################
##### Templates to change parameters in hltMuonValidator #####################
# process.hltMuonValidator.hltPathsToCheck = ["HLT_IsoMu3"]
# process.hltMuonValidator.genMuonCut = "abs(mother.pdgId) == 24"
# process.hltMuonValidator.recMuonCut = "isGlobalMuon && eta < 1.2"
##############################################################################

hltProcessName = "HLT"
process.hltHiggsValidator.hltProcessName = hltProcessName
#process.hltHiggsValidator.HWW.hltPathsToCheck = cms.vstring(
#		"HLT_Photon26",
		#		"HLT_Mu30_eta2p1_v",
		#		"HLT_IsoMu24_eta2p1_v",
		#"HLT_Ele27_WP80_v",
#		)

process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = cms.string(autoCond['startup'])

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	    #'file:/afs/cern.ch/user/d/duarte/scratch0/step2_RAW2DIGI_RECO.root',
	    #'/store/relval/CMSSW_5_2_0/RelValZMM/GEN-SIM-RECO/START52_V4A-v1/0248/BE2AD2B0-1569-E111-B555-003048678FF4.root',
	    #'/store/relval/CMSSW_6_2_0/RelValMinBias/GEN-SIM-RECO/PRE_ST62_V8-v3/00000/E25D61DF-53EC-E211-833B-002481E736D2.root',
	    '/store/relval/CMSSW_6_2_0_pre8/RelValSingleMuPt100/GEN-SIM-RECO/PRE_ST62_V8-v1/00000/A2495409-00E1-E211-B3E9-00304896B904.root'
    ),
    secondaryFileNames = cms.untracked.vstring(
	    #'file:/afs/cern.ch/user/d/duarte/scratch0/H130GGgluonfusion_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT.root',
       	    #'/store/relval/CMSSW_6_2_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_ST62_V8-v3/00000/F0BD3DB3-3AEC-E211-A563-5404A63886A5.root',
       	    #'/store/relval/CMSSW_6_2_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_ST62_V8-v3/00000/EE31C8C3-46EC-E211-BD25-02163E008FF0.root',
       	    '/store/relval/CMSSW_6_2_0_pre8/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_ST62_V8-v1/00000/1232ED18-E6E0-E211-8BB9-003048FE9B78.root'
    )
)

process.DQMStore = cms.Service("DQMStore")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000
process.MessageLogger.destinations += ['HiggsValidationMessages']
process.MessageLogger.categories   += ['HiggsValidation']
process.MessageLogger.debugModules += ['*']#HLTHiggsValidator','HLTHiggsSubAnalysis','HLTHiggsPlotter']
process.MessageLogger.HiggsValidationMessages = cms.untracked.PSet(
    threshold       = cms.untracked.string('DEBUG'),
    default         = cms.untracked.PSet(limit = cms.untracked.int32(0)),
    HiggsValidation = cms.untracked.PSet(limit = cms.untracked.int32(1000))
    )

process.out = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
        'drop *', 
        'keep *_MEtoEDMConverter_*_HLTHiggsOfflineAnalysis'),
    fileName = cms.untracked.string('hltHiggsValidator.root')
)


process.analyzerpath = cms.Path(
    #process.prebTagSequence *
    process.hltHiggsValidator *
    process.MEtoEDMConverter
)


process.outpath = cms.EndPath(process.out)
