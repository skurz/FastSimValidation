#!/usr/bin/env python

import ROOT as rt

class Efficiency:
    def __init__(self,denominator=None,numerator=None,directory=None,):
        self.denominator = denominator
        self.numerator = numerator
        self.directory = directory
    def getHist(self,file):
        h_denominator = file.Get(self.directory + "/" + self.denominator).Clone()
        h_numerator = file.Get(self.directory + "/" + self.numerator).Clone()
        h_numerator.Divide(h_denominator)
        return h_numerator

class Response:
    def __init__(self,rawhist=None,directory=None):
        self.rawhist = rawhist
        self.directory = directory
    #def getHist(file):
        # translate rawhist into TProfile  with the mean
        # h_mean = ...
        # create the TProfile with the RMS
        # h_RMS = 
        # return [h_mean,h_RMS]
    

    
