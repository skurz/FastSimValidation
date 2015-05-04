#!/usr/bin/env python

# USAGE:
# ./compare.py fastsim.root fullsim.root <OUTPUT_DIRECTORY>

import sys,os
import ROOT as rt
from math import sqrt

EXT = ".pdf"

files = [sys.argv[1],sys.argv[2]]
odir = sys.argv[3]
titles = ["FastSim","FullSim"]
colors = [rt.kRed,rt.kBlack]
tfiles = [rt.TFile.Open(file) for file in files]

# create output directory
if os.path.exists(odir):
    print "ERROR: output directory '{0}' already exists".format(odir)
    sys.exit()
os.mkdir(odir)

# create canvas
PADFRAC = 0.28
canvas = rt.TCanvas("canvas","canvas",800,800)
mainpad = rt.TPad("mainpad","mainpad",0.,PADFRAC,1.,1.)
mainpad.SetBottomMargin(0.12)
mainpad.SetLeftMargin(0.12)
ratiopad = rt.TPad("ratiopad","ratiopad",0.,0.,1.,PADFRAC)
ratiopad.SetBottomMargin(0.3)
ratiopad.SetTopMargin(0.12)
ratiopad.SetLeftMargin(0.12)
canvas.cd()
mainpad.Draw()
canvas.cd()
ratiopad.Draw()

# function to recursively process the content of a TDirectory
# the argument is a list of TDirectory objects (e.g. TFile objects)
# it is assumed that all TDirectory objects have the same structure
def processTDirectory(tdirs):
    for key in tdirs[0].GetListOfKeys():
        path = key.GetMotherDir().GetPath().split(":")[1]
        path += "/" + key.GetName()
        path = path.lstrip("/")
        objects = [tfile.Get(path) for tfile in tfiles]
        if isinstance(objects[0],rt.TDirectory):
            os.system("mkdir -p {0}/{1}".format(odir,path))
            processTDirectory(objects)
        elif isinstance(objects[0],rt.TH1):
            processTH1(objects,odir + "/" + path + EXT)
        elif isinstance(objects[0],rt.TGraphAsymmErrors):
            processTGraph(objects,odir + "/" + path + EXT)

# get y-range of list of histograms
# using hist.GetBinContent(hist.GetMinimumBin()) rather than hist.GetMinimum
# avoids interference with internal ROOT-usages and setting of mimima and maxima
def getYRange(hists):
    _min = min([hist.GetBinContent(hist.GetMinimumBin()) for hist in hists])*1.1
    _max = max([hist.GetBinContent(hist.GetMaximumBin()) for hist in hists])*1.3
    return [_min,_max]

def getYRangeGraph(hists):
    xVal = rt.Double(0.)
    yVal = rt.Double(0.)
    yList = []

    for h in range(len(hists)): 
        hist = hists[h]

        for nBin in range(hist.GetN()-1):  
                hist.GetPoint(nBin, xVal, yVal)
                yList.append(float(yVal))

    _min = min(yList)*1.1
    _max = max(yList)*1.3
    return [_min,_max]

# function to print a list of histograms
# the argument is a list of histograms
# it is assumed that the histograms have the same binning
def processTH1(hists,ofile):

    # set histogram style and draw
    [_min,_max] = getYRange(hists)
    if _min > 0:
        _min = 0
    mainpad.cd()
    for h in range(len(hists)):
        hist = hists[h]        
        hist.SetStats(False)
        setHistStyle(hist, h, _min, _max)
        hist.Rebin(2)

        if h == 0:
            hist.Draw("E")
        else:
            hist.Draw("E,SAME")

    # calculate ratios
    ratios = []
    ratiopad.cd()
    for h in range(len(hists)-1):
        ratios.append(hists[0].Clone())
        ratio = ratios[h]
        ratio.Divide(hists[-1])

    # set histogram style and draw
    _min,_max = getYRange(ratios)
    for h in range(len(ratios)): 
        ratio = ratios[h]

        if h == 0:
            ratioplot = rt.TF1("zL","1", ratio.GetXaxis().GetXmin(), ratio.GetXaxis().GetXmax())            
            setRatioStyle(ratioplot)
            ratioplot.Draw()
        
        ratio.SetLineColor(colors[h])
        ratio.Draw("E,SAME")

    # Legend
    mainpad.cd()
    leg = rt.TLegend(0.7,0.75,0.9,0.9);
    #leg->SetHeader("");
    for h in range(len(hists)):
        if h == 0:
            leg.AddEntry(hists[h],"FastSim","l")
        else:
            leg.AddEntry(hists[h],"FullSim","l")
    leg.Draw();

    # finally, print the canvas
    canvas.Print(ofile)


def processTGraph(hists,ofile):

    # set histogram style and draw
    [_min,_max] = getYRangeGraph(hists)
    if _max > 0.3:
        _max = 1.0
    mainpad.cd()
    for h in range(len(hists)):
        hist = hists[h]
        setHistStyle(hist, h, 0., _max*1.3)

        if h == 0:
            hist.Draw("AL")
        else:
            hist.Draw("L")

    # ratios
    ratios = []
    ratiopad.cd()
    for h in range(len(hists)-1):
        nom = hists[0]
        denom = hists[-1]
        ratio = rt.TGraphErrors(nom.GetN())
        ratios.append(ratio)

        for nNom in range(nom.GetN()-1):
            xNom = rt.Double(0.)
            yNom = rt.Double(0.)
            nom.GetPoint(nNom, xNom, yNom)
            yNomHigh = nom.GetErrorYhigh(nNom)
            yNomLow = nom.GetErrorYlow(nNom)
            yNomErr = max(yNomHigh, yNomLow)

            for nDenom in range(denom.GetN()-1):
                xDenom = rt.Double(0.)
                yDenom = rt.Double(0.)
                denom.GetPoint(nDenom, xDenom, yDenom)
                yDenomHigh = denom.GetErrorYhigh(nDenom)
                yDenomLow = denom.GetErrorYlow(nDenom)
                yDenomErr = max(yDenomHigh, yDenomLow)

                if xNom == xDenom:
                    if not yDenom == 0:
                        ratio.SetPoint(nNom, xNom, yNom/yDenom)
                        yError = sqrt(1/(yDenom*yDenom)*yNomErr*yNomErr+(yNom*yNom)/(yDenom*yDenom*yDenom*yDenom)*yDenomErr*yDenomErr)
                        ratio.SetPointError(nNom, 0., yError)

    for h in range(len(ratios)):
        ratio = ratios[h]

        if h == 0:
            ratioplot = rt.TF1("zL","1", min(nom.GetXaxis().GetXmin(),denom.GetXaxis().GetXmin()), max(nom.GetXaxis().GetXmax(),denom.GetXaxis().GetXmax()))            
            setRatioStyle(ratioplot)
            ratioplot.Draw()

        ratio.SetLineColor(colors[h])
        ratio.SetMarkerColor(colors[h])
        ratio.SetMarkerStyle(2)
        ratio.Draw("P0")

    # Legend
    mainpad.cd()
    leg = rt.TLegend(0.7,0.75,0.9,0.9);
    #leg->SetHeader("");
    for h in range(len(hists)):
        if h == 0:
            leg.AddEntry(hists[h],"FastSim","l")
        else:
            leg.AddEntry(hists[h],"FullSim","l")
    leg.Draw();

    # finally, print the canvas
    canvas.Print(ofile)

def setHistStyle(hist, ind, ymin, ymax):
    hist.SetTitle("")
    hist.SetMinimum(ymin)
    hist.SetMaximum(ymax)    
    hist.GetXaxis().CenterTitle()
    hist.GetYaxis().CenterTitle()
    hist.GetYaxis().SetTitleOffset(0.5*2.7)
    hist.GetXaxis().SetTitleSize(0.045)
    hist.GetYaxis().SetTitleSize(0.045)
    hist.SetLineColor(colors[ind])

def setRatioStyle(ratio):
    ratio.SetMinimum(0.5)
    ratio.SetMaximum(2.)
    ratio.SetLineColor(1)
    ratio.SetLineWidth(1)
    ratio.SetTitle(";;FastSim/FullSim")
    ratio.GetYaxis().CenterTitle()
    ratio.GetYaxis().SetTitleSize(0.045*2.6)
    ratio.GetYaxis().SetTitleOffset(0.5)
    ratio.GetXaxis().SetLabelSize(0.09)
    ratio.GetYaxis().SetLabelSize(0.09)
    ratio.GetYaxis().SetNdivisions(-503)
                  
processTDirectory(tfiles)



