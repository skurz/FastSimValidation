#!/usr/bin/env python

import sys,imp
import ROOT as rt

sys.path.append('python')

ifile = sys.argv[1]
cfgfile = sys.argv[2]
ofile = sys.argv[3]

iFILE = rt.TFile(ifile)
oFILE = rt.TFile(ofile,"RECREATE")

cfg = imp.load_source('dummy',cfgfile)

for _cfg in cfg.cfg:
    hist = _cfg.getHist(iFILE)
    oFILE.mkdir(_cfg.directory)
    oFILE.cd(_cfg.directory)
    hist.Write()
    oFILE.cd()

oFILE.Close()
