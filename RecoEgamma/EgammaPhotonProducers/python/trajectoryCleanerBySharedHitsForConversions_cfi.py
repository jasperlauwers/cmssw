import FWCore.ParameterSet.Config as cms

from TrackingTools.TrajectoryCleaning.TrajectoryCleanerBySharedHits_cfi import *
import  TrackingTools.TrajectoryCleaning.TrajectoryCleanerBySharedHits_cfi
TrajectoryCleanerBySharedHitsForConversions = TrackingTools.TrajectoryCleaning.TrajectoryCleanerBySharedHits_cfi.trajectoryCleanerBySharedHits.clone()
TrajectoryCleanerBySharedHitsForConversions.ComponentName = 'TrajectoryCleanerBySharedHitsForConversions'
TrajectoryCleanerBySharedHitsForConversions.fractionShared = cms.double('0.5')

