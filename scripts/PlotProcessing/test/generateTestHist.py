#!/usr/bin/env python

import ROOT as rt
rand = rt.TRandom3()

file = rt.TFile.Open("testHist.root","RECREATE")

file.mkdir("jets/looseId")
file.cd("jets/looseId")
genJetEta = rt.TH1D("genJetEta","genJetEta",100,-3,3)
genJetEta_matched = genJetEta.Clone("genJetEta_matched")
genJetEta.FillRandom("gaus",1000)
for b in range(0,genJetEta.GetNbinsX()):
    genJetEta_matched.SetBinContent(b,genJetEta.GetBinContent(b)*rand.Uniform(0.7,0.9))
genJetEta.Write()
genJetEta_matched.Write()

file.mkdir("jets/mediumId").cd()
file.cd("jets/mediumId")
genJetEta = rt.TH1D("genJetEta","genJetEta",100,-3,3)
genJetEta_matched = genJetEta.Clone("genJetEta_matched")
genJetEta.FillRandom("gaus",1000)
for b in range(0,genJetEta.GetNbinsX()):
    genJetEta_matched.SetBinContent(b,genJetEta.GetBinContent(b)*rand.Uniform(0.5,0.7))
genJetEta.Write()
genJetEta_matched.Write()

file.Close()
