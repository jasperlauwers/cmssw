
/** \file HLTHiggsPlotter.cc
 *  $Date: 2013/06/05 13:50:24 $
 *  $Revision: 1.8 $
 */


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include "HLTriggerOffline/Higgs/interface/HLTHiggsPlotter.h"
#include "HLTriggerOffline/Higgs/interface/HLTHiggsSubAnalysis.h"
#include "HLTriggerOffline/Higgs/src/EVTColContainer.cc"

#include "TPRegexp.h"


#include<set>
#include<cctype>

HLTHiggsPlotter::HLTHiggsPlotter(const edm::ParameterSet & pset,
                                 const std::string & hltPath,
                                 const std::vector<unsigned int> & objectsType, 
				  const unsigned int & minCandidates,
				  const bool & isVBFHBB,
                                 DQMStore * dbe) :
    _hltPath(hltPath),
    _hltProcessName(pset.getParameter<std::string>("hltProcessName")),
    _objectsType(std::set<unsigned int>(objectsType.begin(),objectsType.end())),
    _nObjects(objectsType.size()),
    _parametersEta(pset.getParameter<std::vector<double> >("parametersEta")),
    _parametersPhi(pset.getParameter<std::vector<double> >("parametersPhi")),
    _parametersTurnOn(pset.getParameter<std::vector<double> >("parametersTurnOn")),
    _minCandidates(minCandidates),
    _isVBFHBB(isVBFHBB),
    _dbe(dbe)
{
  for(std::set<unsigned int>::iterator it = _objectsType.begin();
      it != _objectsType.end(); ++it)
  {
    // Some parameters extracted from the .py
    std::string objStr = EVTColContainer::getTypeString( *it );
    _cutMinPt[*it] = pset.getParameter<double>( std::string(objStr+"_cutMinPt").c_str() );
    _cutMaxEta[*it] = pset.getParameter<double>( std::string(objStr+"_cutMaxEta").c_str() );
  }
}

HLTHiggsPlotter::~HLTHiggsPlotter()
{
}


void HLTHiggsPlotter::beginJob() 
{
}



void HLTHiggsPlotter::beginRun(const edm::Run & iRun,
                               const edm::EventSetup & iSetup)
{
  for (std::set<unsigned int>::iterator it = _objectsType.begin(); 
      it != _objectsType.end(); ++it)
  {
    std::vector<std::string> sources(2);
    sources[0] = "gen";
    sources[1] = "rec";
    TString maxPt;

    const std::string objTypeStr = EVTColContainer::getTypeString(*it);
	  
    for (size_t i = 0; i < sources.size(); i++) 
    {
      std::string source = sources[i];
      bookHist(source, objTypeStr, "Eta");
      bookHist(source, objTypeStr, "Phi");
      for( unsigned int i=0; i < _minCandidates; i++ )
      {
	maxPt = "MaxPt";
	maxPt += i+1;
	bookHist(source, objTypeStr, maxPt.Data());
      }
      if( _isVBFHBB ) {
	bookHist(source, objTypeStr, "dEtaqq");
	bookHist(source, objTypeStr, "mqq");
	bookHist(source, objTypeStr, "dPhibb");
      }
    }
  }
}

void HLTHiggsPlotter::analyze(const bool & isPassTrigger,
                              const std::string & source,
                              const std::vector<MatchStruct> & matches)
{
  if ( !isPassTrigger )
  {
    return;
  }
  std::map<unsigned int,int> countobjects;
  // Initializing the count of the used object
  for(std::set<unsigned int>::iterator co = _objectsType.begin();
      co != _objectsType.end(); ++co)
  {
    countobjects[*co] = 0;
  }
	
  int counttotal = 0;
  const int totalobjectssize2 = _minCandidates*countobjects.size();
  // Fill the histos if pass the trigger (just the two with higher pt)
  for (size_t j = 0; j < matches.size(); ++j)
  {
    // Is this object owned by this trigger? If not we are not interested...
    if ( _objectsType.find( matches[j].objType) == _objectsType.end() )
    {
      continue;
    }

    const unsigned int objType = matches[j].objType;
    const std::string objTypeStr = EVTColContainer::getTypeString(matches[j].objType);
		
    float pt  = matches[j].pt;
    float eta = matches[j].eta;
    float phi = matches[j].phi;
    this->fillHist(isPassTrigger,source,objTypeStr,"Eta",eta);
    this->fillHist(isPassTrigger,source,objTypeStr,"Phi",phi);
    
    TString maxPt;
    for( unsigned int i=0; i < _minCandidates; i++ )
    {
      if( (unsigned)countobjects[objType] == i )
      {
	maxPt = "MaxPt";
	maxPt += i+1;
	this->fillHist(isPassTrigger,source,objTypeStr,maxPt.Data(),pt);
	// Filled the high pt ...
	++(countobjects[objType]);
	++counttotal;
	break;
      }
    }
   if ( counttotal == totalobjectssize2 ) 
    {
      break;
    }				
  }
}

void HLTHiggsPlotter::analyze(const bool & isPassTrigger, const std::string & source, const std::vector<MatchStruct> & matches,
			       const std::map<std::string,bool> & nMinOne, const float & dEtaqq, const float & mqq, const float & dPhibb)
{
  if ( !isPassTrigger )
  {
    return;
  }
  std::map<unsigned int,int> countobjects;
  // Initializing the count of the used object
  for(std::set<unsigned int>::iterator co = _objectsType.begin();
      co != _objectsType.end(); ++co)
  {
    countobjects[*co] = 0;
  }
	
  int counttotal = 0;
  const int totalobjectssize2 = _minCandidates*countobjects.size();
  // Fill the histos if pass the trigger (just the two with higher pt)
  for (size_t j = 0; j < matches.size(); ++j)
  {
    // Is this object owned by this trigger? If not we are not interested...
    if ( _objectsType.find( matches[j].objType) == _objectsType.end() )
    {
      continue;
    }

    const unsigned int objType = matches[j].objType;
    const std::string objTypeStr = EVTColContainer::getTypeString(matches[j].objType);
		
    float pt  = matches[j].pt;
    float eta = matches[j].eta;
    float phi = matches[j].phi;
    
    if( objType == EVTColContainer::PFJET )
    {
	if( nMinOne.at("MaxPt1") && nMinOne.at("MaxPt2") ) {
	    this->fillHist(isPassTrigger,source,objTypeStr,"Eta",eta);
	    this->fillHist(isPassTrigger,source,objTypeStr,"Phi",phi);
	}
    }
    else 
    {
	this->fillHist(isPassTrigger,source,objTypeStr,"Eta",eta);
	this->fillHist(isPassTrigger,source,objTypeStr,"Phi",phi);
    }
    
    TString maxPt;
    for( unsigned int i=0; i < _minCandidates; i++ )
    {
      if( (unsigned)countobjects[objType] == i )
      {
	maxPt = "MaxPt";
	maxPt += i+1;
	if( objType == EVTColContainer::PFJET) {
	    if( nMinOne.at(maxPt.Data()) ) {
		this->fillHist(isPassTrigger,source,objTypeStr,maxPt.Data(),pt);
		// Filled the high pt ...
		++(countobjects[objType]);
		++counttotal;
	    }
	}
	else {
	    this->fillHist(isPassTrigger,source,objTypeStr,maxPt.Data(),pt);
	    // Filled the high pt ...
	    ++(countobjects[objType]);
	    ++counttotal;
	}
	
	break;
      }
    }
   if ( counttotal == totalobjectssize2 ) 
    {
      break;
    }				
  }
  
  if( nMinOne.at("dEtaqq") ) {
    this->fillHist(isPassTrigger,source,EVTColContainer::getTypeString(EVTColContainer::PFJET),"dEtaqq",dEtaqq);
  }
  if( nMinOne.at("mqq") ) {
    this->fillHist(isPassTrigger,source,EVTColContainer::getTypeString(EVTColContainer::PFJET),"mqq",mqq);
  }
  if( nMinOne.at("dPhibb") ) {
    this->fillHist(isPassTrigger,source,EVTColContainer::getTypeString(EVTColContainer::PFJET),"dPhibb",dPhibb);
  }  
}

void HLTHiggsPlotter::bookHist(const std::string & source, 
                               const std::string & objType,
                               const std::string & variable)
{
  std::string sourceUpper = source; 
  sourceUpper[0] = std::toupper(sourceUpper[0]);
  std::string name = source + objType + variable + "_" + _hltPath;
  TH1F * h = 0;

  if (variable.find("MaxPt") != std::string::npos) 
  {
    std::string desc;
    if (variable == "MaxPt1") desc = "Leading";
    else if (variable == "MaxPt2") desc = "Next-to-Leading";
    else desc = variable.substr(5,6) + "th Leading";
    std::string title = "pT of " + desc + " " + sourceUpper + " " + objType + " "
        "where event pass the "+ _hltPath;
    const size_t nBins = _parametersTurnOn.size() - 1;
    float * edges = new float[nBins + 1];
    for(size_t i = 0; i < nBins + 1; i++)
    {
      edges[i] = _parametersTurnOn[i];
    }
    h = new TH1F(name.c_str(), title.c_str(), nBins, edges);
    delete [] edges;
  }
  else 
    {
    if( variable == "dEtaqq" ){
	std::string title  = "#Delta #eta_{qq} of " + sourceUpper + " " + objType;
	int    nBins = 20;
	double min   = 0;
	double max   = 4.8;
	h = new TH1F(name.c_str(), title.c_str(), nBins, min, max);
	}
	else if ( variable == "mqq" ){
		std::string title  = "m_{qq} of " + sourceUpper + " " + objType;
		int    nBins = 20;
	    	double min   = 0;
	    	double max   = 800;
	    	h = new TH1F(name.c_str(), title.c_str(), nBins, min, max);
	} 
	else if ( variable == "dPhibb" ){
		std::string title  = "#Delta #phi_{bb} of " + sourceUpper + " " + objType;
		int    nBins = 20;
	    	double min   = 0;
	    	double max   = 6.284;
	    	h = new TH1F(name.c_str(), title.c_str(), nBins, min, max);
	}
	else
	{
	    std::string symbol = (variable == "Eta") ? "#eta" : "#phi";
	    std::string title  = symbol + " of " + sourceUpper + " " + objType + " "+
		"where event pass the "+ _hltPath;
	    std::vector<double> params = (variable == "Eta") ? _parametersEta : _parametersPhi;

	    int    nBins = (int)params[0];
	    double min   = params[1];
	    double max   = params[2];
	    h = new TH1F(name.c_str(), title.c_str(), nBins, min, max);
	}
    }
  h->Sumw2();
  _elements[name] = _dbe->book1D(name, h);
  delete h;
}

void HLTHiggsPlotter::fillHist(const bool & passTrigger,
                               const std::string & source, 
                               const std::string & objType,
                               const std::string & variable,
                               const float & value )
{
  std::string sourceUpper = source; 
  sourceUpper[0] = toupper(sourceUpper[0]);
  std::string name = source + objType + variable + "_" + _hltPath;

  _elements[name]->Fill(value);
}


