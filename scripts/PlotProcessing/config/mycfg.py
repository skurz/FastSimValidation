#!/usr/bin/env python

from histprocessing import *

cfg = []

dirs = ["jets/looseId","jets/mediumId"]

for dir in dirs:
    cfg.extend([
            #Efficiency(numerator = "genJetPt_matched", denominator = "genJetPt", directory = dir),
            Efficiency(numerator = "genJetEta_matched", denominator = "genJetEta", directory = dir),
            #Response(rawhist = "jetPtResponse_vs_genJetPt",directory = dir),
            #Response(rawhits = "jetPtResponse_vs_genJetEta",directory = dir),
            #Response(rawhist = "jetEtaResponse_vs_genJetPt",directory = dir),
            #Response(rawhits = "jetEtaResponse_vs_genJetEta",directory = dir)
            ])

