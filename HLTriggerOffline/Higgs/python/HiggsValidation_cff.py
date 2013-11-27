import FWCore.ParameterSet.Config as cms

#from HLTriggerOffline.Higgs.HLTBtagPFJets_cfi import *

from HLTriggerOffline.Higgs.hltHiggsValidator_cfi import *

HiggsValidationSequence = cms.Sequence(
    #prebTagSequence+
    hltHiggsValidator
    )

#HLTHiggsVal_FastSim = cms.Sequence(
#    recoHiggsValidationHLTFastSim_seq +
#    hltHiggsValidator
#    )
