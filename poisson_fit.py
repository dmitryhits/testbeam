import ROOT
import math

ROOT.gStyle.SetOptFit(11111)

n0 = 134989
n1 = 4776
n2 = 206
n3 = 24
n4 = 6
n5 = 0

n = n0 + n1 + n2 + n3 +n4

h = ROOT.TH1F("h","h",6,0,6)
h.SetBinContent(1,n0)
h.SetBinError  (1,math.sqrt(n0))
h.SetBinContent(2,n1)
h.SetBinError  (2,math.sqrt(n1))
h.SetBinContent(3,n2)
h.SetBinError  (3,math.sqrt(n2))
h.SetBinContent(4,n3)
h.SetBinError  (4,math.sqrt(n3))
h.SetBinContent(5,n4)
h.SetBinError  (5,math.sqrt(n4))
h.SetBinContent(6,n5)
h.SetBinError  (6,math.sqrt(n5))


c = ROOT.TCanvas("c","c",600,600)
c.SetLogy()
  
h.SetTitle("")
h.GetYaxis().SetRangeUser(1,1.e6)
h.Draw()

f = ROOT.TF1("f","[0]*TMath::PoissonI(x,[1])",0,6)
f.SetParameter(0,1)
f.SetParameter(1,0.01)

h.Fit("f","","",1,6)
