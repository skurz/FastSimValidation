#!/usr/bin/env python

import ROOT as rt

class Efficiency:
    def __init__(self,denominator=None,numerator=None,collection=None,ID=None):
        self.denominator = denominator
        self.numerator = numerator
        self.collection = collection
        self.ID = ID

    def getHist(self,file):
        if self.ID.count("/")==0:
            h_denominator = file.Get("DQMData/Run 1/" + self.collection + "/Run summary/" + "Gen/" + self.denominator).Clone()
        else:
            h_denominator = file.Get("DQMData/Run 1/" + self.collection + "/Run summary/" + "Gen/" + self.ID.split("/", 1)[1] + "/" + self.denominator).Clone()    
        h_numerator = file.Get("DQMData/Run 1/" + self.collection + "/Run summary/" + self.ID + "/" + self.numerator).Clone()

        g_efficiency = rt.TGraphAsymmErrors()
        g_efficiency.Divide(h_numerator,h_denominator,"cl=0.683 b(1,1) mode")

        g_efficiency.SetName("eff_" + h_numerator.GetName())
        g_efficiency.SetTitle("Efficiency of " + h_numerator.GetTitle().split(" ", 5)[5])
        return [g_efficiency]

        # In case TH1 are needed
        #h_numerator.Divide(h_numerator, h_denominator, 1, 1, "B")
        #h_numerator.SetName("eff_" + h_numerator.GetName())
        #h_numerator.SetTitle("Efficiency of " + h_numerator.GetTitle().split(" ", 5)[5])
        #return [h_numerator]


class Response:
    def __init__(self,rawhist=None,collection=None,ID=None):
        self.rawhist = rawhist
        self.collection = collection
        self.ID = ID

    def getHist(self,file):
        h_rawhist = file.Get("DQMData/Run 1/" + self.collection + "/Run summary/" + self.ID + "/" + self.rawhist).Clone()

        NbinsX = h_rawhist.GetNbinsX()
        h_mean = rt.TH1D("scale_" + h_rawhist.GetName(), "scale: " + h_rawhist.GetTitle(), NbinsX, h_rawhist.GetXaxis().GetXmin(), h_rawhist.GetXaxis().GetXmax())
        h_RMS = rt.TH1D("res_" + h_rawhist.GetName(), "res: " + h_rawhist.GetTitle(), NbinsX, h_rawhist.GetXaxis().GetXmin(), h_rawhist.GetXaxis().GetXmax())

        for xBin in range(1, NbinsX):
            h_yProj = h_rawhist.ProjectionY(h_rawhist.GetName() + "_" + str(xBin), xBin, xBin, "e")
            h_mean.SetBinContent(xBin, h_yProj.GetMean())
            h_mean.SetBinError(xBin, h_yProj.GetMeanError())            
            h_RMS.SetBinContent(xBin, h_yProj.GetRMS())
            h_RMS.SetBinError(xBin, h_yProj.GetRMSError())

        return [h_mean,h_RMS]
    

    
