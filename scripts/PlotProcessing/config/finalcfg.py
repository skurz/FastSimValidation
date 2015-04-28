#!/usr/bin/env python

from histprocessing import *

cfg = []

# use e.g. Muons = [["PFMuons",["Loose","Tight"]], ["TrackerMuons",["Loose","Tight"]], ...]


Jets = [["ak4PFJetsCHS", ["LooseID"]]]
bJets = [["ak4PFJetsCHS_bTag", ["CSVL/matchedBjet", "CSVL/matchedCjet", "CSVL/matchedUDSGjet", "CSVM/matchedBjet", "CSVM/matchedCjet", "CSVM/matchedUDSGjet", "CSVT/matchedBjet", "CSVT/matchedCjet", "CSVT/matchedUDSGjet"]]]
Electrons = [["Electrons", ["vetoID", "looseID", "mediumID", "tightID"]]]
Muons = [["Muons", ["looseIDIso", "tightIDIso"]]]
Taus  = [["Taus", ["decayModeFindingOld+byLooseCombinedIsolation/matchedTau", "decayModeFindingOld+byTightIsolation/matchedTau", "decayModeFindingOld+againstElectron/matchedTau", "decayModeFindingNew+byLooseCombinedIsolation/matchedTau","decayModeFindingOld+byLooseCombinedIsolation/matchedEl", "decayModeFindingOld+byTightIsolation/matchedEl", "decayModeFindingOld+againstElectron/matchedEl", "decayModeFindingNew+byLooseCombinedIsolation/matchedEl","decayModeFindingOld+byLooseCombinedIsolation/matchedMu", "decayModeFindingOld+byTightIsolation/matchedMu", "decayModeFindingOld+againstElectron/matchedMu", "decayModeFindingNew+byLooseCombinedIsolation/matchedMu","decayModeFindingOld+byLooseCombinedIsolation/matchedJet", "decayModeFindingOld+byTightIsolation/matchedJet", "decayModeFindingOld+againstElectron/matchedJet", "decayModeFindingNew+byLooseCombinedIsolation/matchedJet"]]]
Photons = [["Photons", ["looseID", "tightID"]]]
MET = [["MET", ["MetPt", "MetPx", "MetPy", "MetProjThrust", "MetProjThrustOrth"]]]

#######################
# Scale / Response
#######################

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
            Response(rawhist = "relPt_vs_truePt", collection = Col[0], ID = IDs, xLabel = "p_{T} [GeV]", yLabel = "p_{T}"),
            Response(rawhist = "etaDiff_vs_truePt", collection = Col[0], ID = IDs, xLabel = "p_{T} [GeV]", yLabel = "#eta"),
            Response(rawhist = "relPt_vs_trueEta", collection = Col[0], ID = IDs, xLabel = "#eta [ ]", yLabel = "p_{T}"),
            Response(rawhist = "etaDiff_vs_trueEta", collection = Col[0], ID = IDs, xLabel = "#eta [ ]", yLabel = "#eta"),
            ])


Collections = []
Collections.extend(Jets)

for Col in Collections:
    for IDs in Col[1]:
        cfg.extend([
            Response(rawhist = "phiDiff_vs_truePt", collection = Col[0], ID = IDs, xLabel = "p_{T} [GeV]", yLabel = "#phi"),
            Response(rawhist = "phiDiff_vs_trueEta", collection = Col[0], ID = IDs, xLabel = "#eta [ ]", yLabel = "#phi"),
            ])


Collections = []
Collections.extend(MET)

for Col in Collections:
    for IDs in Col[1]:
        cfg.extend([
            Response(rawhist = "rel"+IDs+"_vs_HT", collection = Col[0], ID = IDs, xLabel = "HT [GeV]", yLabel = IDs),
            Response(rawhist = "rel"+IDs+"_vs_NJets", collection = Col[0], ID = IDs, xLabel = "NJets [ ]", yLabel = IDs),
            Response(rawhist = "rel"+IDs+"_vs_NVertex", collection = Col[0], ID = IDs, xLabel = "NVtx [ ]", yLabel = IDs),
            Response(rawhist = "rel"+IDs+"_vs_SumET", collection = Col[0], ID = IDs, xLabel = "SumET [GeV]", yLabel = IDs),
            Response(rawhist = "rel"+IDs+"_vs_Thrust", collection = Col[0], ID = IDs, xLabel = "Thrust [ ]", yLabel = IDs),
            Response(rawhist = "rel"+IDs+"_vs_genMET", collection = Col[0], ID = IDs, xLabel = "genMET [GeV]", yLabel = IDs),
            ])


#######################
# Efficiencies
#######################

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
            Efficiency(numerator = "truePt_matched", denominator = "truePt_gen", collection = Col[0], ID = IDs, xLabel = "p_{T} [GeV]", yLabel = "Efficiency"),
            Efficiency(numerator = "trueEta_matched", denominator = "trueEta_gen", collection = Col[0], ID = IDs, xLabel = "#eta [ ]", yLabel = "Efficiency"),
            ])