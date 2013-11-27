#ifndef HLTRIGGEROFFLINE_HIGGS_MATCHSTRUCT_CC
#define HLTRIGGEROFFLINE_HIGGS_MATCHSTRUCT_CC

/** \class MatchStruct
 *  Generate histograms for trigger efficiencies Higgs related
 *  Documentation available on the CMS TWiki:
 *  https://twiki.cern.ch/twiki/bin/view/CMS/HiggsWGHLTValidate
 *
 *  $Date: 2012/03/23 11:50:56 $
 *  $Revision: 1.7 $
 *  \author  J. Duarte Campderros
 *
 */

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "TLorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"

#include<vector>

// Matching structure: helper structure to match gen/reco candidates with
// hlt trigger objects
struct MatchStruct 
{
	unsigned int objType;
	float pt;
	float eta;
	float phi;
	float bTag;
	math::XYZTLorentzVector lorentzVector;
	const void * thepointer;
	MatchStruct():
		objType(0),
		pt(0),
		eta(0),
		phi(0),
		bTag(0),
 		lorentzVector(0,0,0,0),
		thepointer(0)
	{
	}
	MatchStruct(const reco::Candidate * cand, const unsigned int & obj) :
		objType(obj),
		pt(cand->pt()),
		eta(cand->eta()),
		phi(cand->phi()),
		thepointer(cand)

	{
	}
	MatchStruct(const reco::Candidate * cand, const unsigned int & obj, const float & bTagVal) :
		objType(obj),
		pt(cand->pt()),
		eta(cand->eta()),
		phi(cand->phi()),
		bTag(bTagVal),
		lorentzVector(cand->p4()),
		thepointer(cand)

	{
	}
	// FIXME: If finally the track is disappeared, then recover the last code...
	MatchStruct(const reco::Track * cand, const unsigned int & obj) :
		objType(obj),
		pt(cand->pt()),
		eta(cand->eta()),
		phi(cand->phi()),
		thepointer(cand)
	{
	}
	bool operator<(MatchStruct match) 
	{      
		return this->pt < match.pt;
	}
	bool operator>(MatchStruct match) 
	{
		return this->pt > match.pt;		    	
	}
};

//! Helper structure to order MatchStruct
struct matchesByDescendingPt 
{
	bool operator() (MatchStruct a, MatchStruct b) 
	{     
		return a.pt > b.pt;
	}
};
struct matchesByDescendingEta 
{
	bool operator() (MatchStruct a, MatchStruct b) 
	{     
		return a.eta > b.eta;
	}
};
struct matchesByDescendingBtag
{
	bool operator() (MatchStruct a, MatchStruct b) 
	{     
		return a.bTag > b.bTag;
	}
};
#endif
