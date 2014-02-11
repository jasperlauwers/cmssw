import FWCore.ParameterSet.Config as cms

from HLTriggerOffline.Higgs.hltHiggsPostProcessor_cfi import *

# Build the standard strings to the DQM
def efficiency_string(objtype,plot_type,triggerpath):
    # --- IMPORTANT: Add here a elif if you are introduce a new collection
    #                (see EVTColContainer::getTypeString) 
    if objtype == "Mu" :
	objtypeLatex="#mu"
    elif objtype == "Photon": 
	objtypeLatex="#gamma"
    elif objtype == "Ele": 
	objtypeLatex="e"
    elif objtype == "MET" :
	objtypeLatex="MET"
    elif objtype == "PFTau": 
	objtypeLatex="#tau"
    elif objtype == "Jet": 
	objtypeLatex="jet"
    else:
	objtypeLatex=objtype

    numer_description = "# gen %s passed the %s" % (objtypeLatex,triggerpath)
    denom_description = "# gen %s " % (objtypeLatex)

    if plot_type == "TurnOn1":
        title = "pT Turn-On"
        xAxis = "p_{T} of Leading Generated %s (GeV/c)" % (objtype)
        input_type = "gen%sMaxPt1" % (objtype)
    elif plot_type == "TurnOn2":
        title = "Next-to-Leading pT Turn-On"
        xAxis = "p_{T} of Next-to-Leading Generated %s (GeV/c)" % (objtype)
        input_type = "gen%sMaxPt2" % (objtype)
    elif plot_type == "EffEta":
        title = "#eta Efficiency"
        xAxis = "#eta of Generated %s " % (objtype)
        input_type = "gen%sEta" % (objtype)
    elif plot_type == "EffPhi":
        title = "#phi Efficiency"
        xAxis = "#phi of Generated %s " % (objtype)
        input_type = "gen%sPhi" % (objtype)     
    elif "TurnOn" in plot_type:
        title = "%sth Leading pT Turn-On" % (plot_type[-1])
        xAxis = "p_{T} of %sth Leading Generated %s (GeV/c)" % (plot_type[-1], objtype)
        input_type = "gen%sMaxPt%s" % (objtype, plot_type[-1])
    elif plot_type == "EffdEtaqq":
        title = "#Delta #eta_{qq} Efficiency"
        xAxis = "#Delta #eta_{qq} of Generated %s " % (objtype)
        input_type = "gen%sdEtaqq" % (objtype)
    elif plot_type == "Effmqq":
        title = "m_{qq} Efficiency"
        xAxis = "m_{qq} of Generated %s " % (objtype)
        input_type = "gen%smqq" % (objtype)
    elif plot_type == "EffdPhibb":
	title = "#Delta #phi_{bb} Efficiency"
        xAxis = "#Delta #phi_{bb} of Generated %s " % (objtype)
        input_type = "gen%sdPhibb" % (objtype)

    yAxis = "%s / %s" % (numer_description, denom_description)
    all_titles = "%s for trigger %s; %s; %s" % (title, triggerpath,
                                        xAxis, yAxis)
    return "Eff_%s_%s '%s' %s_%s %s" % (input_type,triggerpath,
		    all_titles,input_type,triggerpath,input_type)

# Adding the reco objects
def get_reco_strings(strings):
    reco_strings = []
    for entry in strings:
        reco_strings.append(entry
                            .replace("Generated", "Reconstructed")
                            .replace("Gen", "Reco")
                            .replace("gen", "rec"))
    return reco_strings


plot_types = ["TurnOn1", "TurnOn2", "EffEta", "EffPhi"]

#--- IMPORTANT: Update this collection whenever you introduce a new object
#               in the code (from EVTColContainer::getTypeString)
obj_types  = ["Mu","Ele","Photon","MET","PFTau","Jet"]
#--- IMPORTANT: Trigger are extracted from the hltHiggsValidator_cfi.py module
triggers = [ ] 
efficiency_strings = []

# Extract the triggers used in the hltHiggsValidator 
from HLTriggerOffline.Higgs.hltHiggsValidator_cfi import hltHiggsValidator as _config
triggers = set([])
for an in _config.analysis:
	s = _config.__getattribute__(an)
	vstr = s.__getattribute__("hltPathsToCheck")
	map(lambda x: triggers.add(x.replace("_v","")),vstr)
triggers = list(triggers)

#------------------------------------------------------------

# Generating the list with all the efficiencies
for type in plot_types:
    for obj in obj_types:
	for trig in triggers:
	    efficiency_strings.append(efficiency_string(obj,type,trig))

efficiency_strings.extend(get_reco_strings(efficiency_strings))

hltHiggsPostHTauNu = hltHiggsPostProcessor.clone()
hltHiggsPostHTauNu.subDirs = ['HLT/Higgs/Htaunu']
hltHiggsPostHTauNu.efficiencyProfile = efficiency_strings

hltHiggsPostHWW = hltHiggsPostProcessor.clone()
hltHiggsPostHWW.subDirs = ['HLT/Higgs/HWW']
hltHiggsPostHWW.efficiencyProfile = efficiency_strings

hltHiggsPostHZZ = hltHiggsPostProcessor.clone()
hltHiggsPostHZZ.subDirs = ['HLT/Higgs/HZZ']
hltHiggsPostHZZ.efficiencyProfile = efficiency_strings

hltHiggsPostHgg = hltHiggsPostProcessor.clone()
hltHiggsPostHgg.subDirs = ['HLT/Higgs/Hgg']
hltHiggsPostHgg.efficiencyProfile = efficiency_strings

hltHiggsPostH2tau = hltHiggsPostProcessor.clone()
hltHiggsPostH2tau.subDirs = ['HLT/Higgs/H2tau']
hltHiggsPostH2tau.efficiencyProfile = efficiency_strings

hltHiggsPostHtaunu = hltHiggsPostProcessor.clone()
hltHiggsPostHtaunu.subDirs = ['HLT/Higgs/Htaunu']
hltHiggsPostHtaunu.efficiencyProfile = efficiency_strings

#Specific plots for VBFHbb
isVBFHBB = (_config.__getattribute__("Hbb")).__getattribute__("isVBFHBB")
if isVBFHBB: 
    plot_types = ["TurnOn1", "TurnOn2", "TurnOn3", "TurnOn4", "EffdEtaqq", "Effmqq", "EffdPhibb"]

efficiency_strings = []
efficiency_reco_strings = []
for obj in obj_types:
    for trig in triggers:
	for type in plot_types[0:4]:
	    efficiency_strings.append(efficiency_string(obj,type,trig))
	    efficiency_reco_strings.append(efficiency_string(obj,type,trig))
	for type in plot_types[4:]:
	    efficiency_reco_strings.append(efficiency_string(obj,type,trig))
	    
efficiency_strings.extend(get_reco_strings(efficiency_reco_strings))
    
hltHiggsPostHbb = hltHiggsPostProcessor.clone()
hltHiggsPostHbb.subDirs = ['HLT/Higgs/Hbb']
hltHiggsPostHbb.efficiencyProfile = efficiency_strings


hltHiggsPostProcessors = cms.Sequence(
		hltHiggsPostHWW+
		hltHiggsPostHZZ+
		hltHiggsPostHgg+
		hltHiggsPostHtaunu+
		hltHiggsPostH2tau+
		hltHiggsPostHbb
)
