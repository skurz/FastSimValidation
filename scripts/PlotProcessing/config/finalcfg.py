#!/usr/bin/env python

from histprocessing import *

cfg = []

# use e.g. Muons = [["PFMuons",["Loose","Tight"]], ["TrackerMuons",["Loose","Tight"]], ...]


Jets = [["ak4PFJetsCHS", ["LooseID"]]]
bJets = [["ak4PFJetsCHS_bTag", ["CISVV2_Tight/matchedBjet", "CISVV2_Tight/matchedCjet", "CISVV2_Tight/matchedUDSGjet"]]]
Electrons = [["Electrons", ["vetoID", "looseID", "mediumID", "tightID"]]]
Muons = [["Muons", ["looseIDIso", "tightIDIso"]]]
Taus  = [["Taus", ["decayModeFindingOld+byLooseCombinedIsolation/matchedTau", "decayModeFindingOld+byTightIsolation/matchedTau", "decayModeFindingOld+againstElectron/matchedTau", "decayModeFindingNew+byLooseCombinedIsolation/matchedTau","decayModeFindingOld+byLooseCombinedIsolation/matchedEl", "decayModeFindingOld+byTightIsolation/matchedEl", "decayModeFindingOld+againstElectron/matchedEl", "decayModeFindingNew+byLooseCombinedIsolation/matchedEl","decayModeFindingOld+byLooseCombinedIsolation/matchedMu", "decayModeFindingOld+byTightIsolation/matchedMu", "decayModeFindingOld+againstElectron/matchedMu", "decayModeFindingNew+byLooseCombinedIsolation/matchedMu","decayModeFindingOld+byLooseCombinedIsolation/matchedJet", "decayModeFindingOld+byTightIsolation/matchedJet", "decayModeFindingOld+againstElectron/matchedJet", "decayModeFindingNew+byLooseCombinedIsolation/matchedJet"]]]
Photons = [["Photons", ["looseID", "tightID"]]]
MET = [["MET", ["vs_MetPt", "vs_MetPx", "vs_MetPy", "vs_MetProj", "vs_MetProjOrth"]]]


Collections = []
Collections.extend(Electrons)
Collections.extend(Muons)
Collections.extend(Taus)
Collections.extend(Photons)
Collections.extend(Jets)
#Collections.extend(MET)

for Col in Collections:
    for IDs in Col[1]:
        cfg.extend([
            Response(rawhist = "truePt_vs_pt", collection = Col[0], ID = IDs),
            Response(rawhist = "truePt_vs_eta", collection = Col[0], ID = IDs),
            Response(rawhist = "trueEta_vs_pt", collection = Col[0], ID = IDs),
            Response(rawhist = "trueEta_vs_eta", collection = Col[0], ID = IDs),
            ])

Collections = []
Collections.extend(Jets)

for Col in Collections:
    for IDs in Col[1]:
        cfg.extend([
            Response(rawhist = "truePt_vs_phi", collection = Col[0], ID = IDs),
            Response(rawhist = "trueEta_vs_phi", collection = Col[0], ID = IDs),
            ])

Collections = []
Collections.extend(Jets)
Collections.extend(bJets)
Collections.extend(Electrons)
Collections.extend(Muons)
Collections.extend(Taus)
Collections.extend(Photons)

for Col in Collections:
    for IDs in Col[1]:
        cfg.extend([
            Efficiency(numerator = "truePt_matched", denominator = "truePt_gen", collection = Col[0], ID = IDs),
            Efficiency(numerator = "trueEta_matched", denominator = "trueEta_gen", collection = Col[0], ID = IDs),
            ])