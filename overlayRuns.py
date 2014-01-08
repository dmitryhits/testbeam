import ROOT 
from ROOT import gROOT, TCanvas, TH1
from shiftAndFlip2 import shiftAndFlip2

basepath = "/shome/hits/testbeam_data/"
gROOT.Reset()
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.ForceStyle()

li_runs = [ 30,39,40,41,42 ]

dic_run_volts = {
    30 : "500",
    39 : "500",
    40 : "500",
    41 : "500",
    42 : "500",
}

dic_run_rates = {
    30 : "SC,1500",
    39 : "1000",
    40 : "9000",
    41 : "120000",
    42 : "9000",
}

dic_hs = {}


files=[]

li_colors = [ROOT.kBlack, ROOT.kRed, ROOT.kBlue, 8, 28, ROOT.kOrange, ROOT.kGreen, ROOT.kMagenta]*10

for i_run in li_runs:
    infile = ROOT.TFile( basepath+"Run" +str(i_run) + "_histos.root")    
    print infile
    h =  infile.Get( "histo1" ).Clone( str(i_run))
    h.SetDirectory(0)
    dic_hs[i_run] = h
    print i_run, h.Integral()

    files.append(infile)


legend_origin_x  = 0.7
legend_origin_y  = 0.6
legend_size_x    = 0.25
legend_size_y    = 0.3

leg = ROOT.TLegend( legend_origin_x,
                    legend_origin_y,
                    legend_origin_x + legend_size_x,
                    legend_origin_y + legend_size_y )
leg.SetFillColor(0)
leg.SetLineColor(0)
leg.SetTextSize(0.05)
leg.SetShadowColor(0)

li_hs = []
c1 = TCanvas( 'c1', 'Overlayed Runs', 200, 10, 700, 500 )
c1.SetGridx()
c1.SetGridy()

for i_h in sorted(dic_hs.keys()):

    print i_h, h.Integral()
    
    h_tmp = shiftAndFlip2( dic_hs[i_h] )
    li_hs.append( h_tmp )
    
    print  "Scaling before", h_tmp.FindBin(-10),  h_tmp.FindBin(100),  1/h_tmp.Integral( h_tmp.FindBin(-10), h_tmp.FindBin(100))    
    h_tmp.Scale( 1/h_tmp.Integral( h_tmp.FindBin(-10), h_tmp.FindBin(100)))
    print  "Scaling after", h_tmp.FindBin(-10),  h_tmp.FindBin(100),  1/h_tmp.Integral( h_tmp.FindBin(-10), h_tmp.FindBin(100))
    h_tmp.SetAxisRange( -10., 100., "x")
    
    h_tmp.SetLineWidth(2)
    h_tmp.SetLineColor(li_colors.pop(0))
    

    if i_h == 30: # Run that sets the scale
        h_tmp.Draw()
    else:
        h_tmp.Draw("SAME")

    leg.AddEntry( h_tmp, "Run "+str(i_h)+ " (" + dic_run_rates[i_h]+" Hz)", "L" )

leg.Draw()
