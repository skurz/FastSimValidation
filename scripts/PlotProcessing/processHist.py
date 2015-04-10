#!/usr/bin/env python

import sys,imp
import ROOT as rt
from ROOT import gDirectory

sys.path.append('python')

ifile = sys.argv[1]
cfgfile = sys.argv[2]
ofile = sys.argv[3]

iFILE = rt.TFile(ifile)
oFILE = rt.TFile(ofile,"RECREATE")

cfg = imp.load_source('final',cfgfile)

for _cfg in cfg.cfg:
    hist = _cfg.getHist(iFILE)

    if not gDirectory.GetDirectory(_cfg.collection):
        gDirectory.mkdir(_cfg.collection+"/"+_cfg.ID)
        gDirectory.cd(_cfg.collection+"/"+_cfg.ID)
    else:
        gDirectory.cd(_cfg.collection)
        if not gDirectory.GetDirectory(_cfg.ID):
            gDirectory.mkdir(_cfg.ID)
        gDirectory.cd(_cfg.ID)

    for h in hist:
        h.Write()
        #CANVAS.Print(_cfg.collection + "_" + _cfg.ID + "_" + h.GetName() + ".pdf")
    gDirectory.cd("/")

oFILE.Close()
