#!/usr/bin/env python

import ROOT as rt

class Efficiency:
    def __init__(self,denominator=None,numerator=None,collection=None,ID=None,xLabel="",yLabel=""):
        self.denominator = denominator
        self.numerator = numerator
        self.collection = collection
        self.ID = ID
        self.xLabel = xLabel
        self.yLabel = yLabel


    def getHist(self,file):

        # Get Histograms
        if self.ID.count("/")==0:
            h_denominator = file.Get("DQMData/Run 1/" + self.collection + "/Run summary/" + "Gen/" + self.denominator).Clone()
        else:
            h_denominator = file.Get("DQMData/Run 1/" + self.collection + "/Run summary/" + "Gen/" + self.ID.split("/", 1)[1] + "/" + self.denominator).Clone()    
        h_numerator = file.Get("DQMData/Run 1/" + self.collection + "/Run summary/" + self.ID + "/" + self.numerator).Clone()

        # Calculate Fraction Using Binomial Errors
        # Asym. Errors:
        g_efficiency = rt.TGraphAsymmErrors()
        g_efficiency.Divide(h_numerator,h_denominator,"cl=0.683 b(1,1) mode")

        # In case TH1 are needed
        #g_efficiency = h_numerator.Clone()
        #g_efficiency.Divide(h_numerator, h_denominator, 1, 1, "B")

        # Set Axis Labels, Name,..
        g_efficiency.GetXaxis().SetTitle(self.xLabel);
        g_efficiency.GetYaxis().SetTitle(self.yLabel);

        g_efficiency.GetXaxis().SetTitleOffset(1.3);
        g_efficiency.GetYaxis().SetTitleOffset(1.4);


        g_efficiency.SetName("eff_" + h_numerator.GetName())
        return [g_efficiency]


class Response:
    def __init__(self,rawhist=None,collection=None,ID=None,xLabel="",yLabel=""):
        self.rawhist = rawhist
        self.collection = collection
        self.ID = ID
        self.xLabel = xLabel
        self.yLabel = yLabel

    def getHist(self,file):

        # Get Histogram
        h_rawhist = file.Get("DQMData/Run 1/" + self.collection + "/Run summary/" + self.ID + "/" + self.rawhist).Clone()

        # Calculate Response
        NbinsX = h_rawhist.GetNbinsX()
        h_mean = rt.TH1D("scale_" + h_rawhist.GetName(), "", NbinsX, h_rawhist.GetXaxis().GetXmin(), h_rawhist.GetXaxis().GetXmax())
        h_RMS = rt.TH1D("res_"    + h_rawhist.GetName(), "", NbinsX, h_rawhist.GetXaxis().GetXmin(), h_rawhist.GetXaxis().GetXmax())

        for xBin in range(1, NbinsX):
            h_yProj = h_rawhist.ProjectionY(h_rawhist.GetName() + "_" + str(xBin), xBin, xBin, "e")
            h_mean.SetBinContent(xBin, h_yProj.GetMean())
            h_mean.SetBinError(xBin, h_yProj.GetMeanError())            
            h_RMS.SetBinContent(xBin, h_yProj.GetRMS())
            h_RMS.SetBinError(xBin, h_yProj.GetRMSError())

        # Set Axis Labels, Name,..
        h_mean.GetXaxis().SetTitle(self.xLabel);
        h_RMS.GetXaxis().SetTitle(self.xLabel);

        if self.yLabel.count("eta")==0 and self.yLabel.count("phi")==0:
            h_mean.GetYaxis().SetTitle("<"+self.yLabel+"^{reco} / "+self.yLabel+"^{true}>");        
            h_RMS.GetYaxis().SetTitle("#sigma("+self.yLabel+"^{reco} / "+self.yLabel+"^{true})");
        else:
            h_mean.GetYaxis().SetTitle("<"+self.yLabel+"^{reco} - "+self.yLabel+"^{true}>");        
            h_RMS.GetYaxis().SetTitle("#sigma("+self.yLabel+"^{reco} - "+self.yLabel+"^{true})");

        h_mean.GetXaxis().SetTitleOffset(1.3);
        h_mean.GetYaxis().SetTitleOffset(1.4);
        h_RMS.GetXaxis().SetTitleOffset(1.3);
        h_RMS.GetYaxis().SetTitleOffset(1.4);

        return [h_mean,h_RMS]
    

    
