import ROOT
from shiftAndFlip import shiftAndFlip

basepath = "./"

ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.ForceStyle()

li_runs = [ 4,5,6,7,8,9,10 ]

dic_run_volts = {
    4 : "500",
    5 : "-500",
    6 : "500",
    7 : "1000",
    8 : "-1000",
    9 : "-200",
    10 : "200",
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
for i_h in sorted(dic_hs.keys()):

    print i_h, h.Integral()
    
    h_tmp = shiftAndFlip( dic_hs[i_h] )
    li_hs.append( h_tmp )
    
    
    h_tmp.Scale( 1/h_tmp.Integral( h_tmp.FindBin(5), h_tmp.FindBin(600)))
    h_tmp.SetAxisRange( -10., 60., "x")
    
    h_tmp.SetLineWidth(2)
    h_tmp.SetLineColor(li_colors.pop(0))
    

    if i_h == 4:
        h_tmp.Draw()
    else:
        h_tmp.Draw("SAME")

    leg.AddEntry( h_tmp, "Run "+str(i_h)+ " (" + dic_run_volts[i_h]+" V)", "L" )

leg.Draw()
