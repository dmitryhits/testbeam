import ROOT 
from ROOT import gROOT, TCanvas, TH1
from shiftAndFlip import shiftAndFlip
from shiftAndFlip2 import shiftAndFlip2
from shiftFlipandScale import shiftFlipAndScale

basepath = "/shome/hits/testbeam_data/"
gROOT.Reset()
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.ForceStyle()


#li_runs = [  24 ]
li_runs = [  24,25,26,27,28,29 ]
#li_runs = [ 15,16,17, 18 ]
#li_runs = [ 11,12,13,15,16,17, 18 ]
#li_runs = [ 14, 30 ]

#outfile = ROOT.TFile( basepath+"Run" +str(li_runs) + "_overlayed.root","OVERWRITE")

dic_run_volts = {
    11 : "+500 ",
    12 : "-500 ",
    13 : "+1000",
    15 : "-1000",
    16 : "-1000",
    17 : "-1000",
    18 : "-1000",
    24 : "-500 ",
    25 : "-500 ",
    26 : "-500 ",
    27 : "-500 ",
    28 : "-500 ",
    29 : "-500 ",
    31 : "+500 ",
    }

dic_gain = {
    14: "10",
    30: " 6",
    }
dic_run_rates = {
    11 : "200  ",
    12 : "210  ",
    13 : "200  ",
    15 : "200  ",
    16 : "4000 ",
    17 : "8000 ",
    18 : "12500",
    24 : "1500  ",
    25 : "21000 ",
    26 : "43000 ",
    27 : "83000 ",
    28 : "160000",
    29 : "1500  ",
    31 : "1100 ",
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
    
    if(i_h > 10 and i_h < 17):
        h_tmp = shiftFlipAndScale( dic_hs[i_h] )
    if(i_h <30 and i_h >16):
        h_tmp = shiftAndFlip( dic_hs[i_h] )
        # at and after run30 a pixel map was making background much smaller introduced
        # the signal peak became bigger than background ,
        # thus requireing a change in shiftAndFlip algorithm
    if(i_h >= 30):
        h_tmp = shiftAndFlip2( dic_hs[i_h] )
    li_hs.append( h_tmp )
    
    print  "Scaling before", h_tmp.FindBin(-20),  h_tmp.FindBin(100),  1/h_tmp.Integral( h_tmp.FindBin(-20), h_tmp.FindBin(100))    
    if(i_h > 10 and i_h < 17):
        h_tmp.Scale( 1/(0.658*h_tmp.Integral( h_tmp.FindBin(-20), h_tmp.FindBin(100))))
        print  "Scaling after", h_tmp.FindBin(-20),  h_tmp.FindBin(100),  1/h_tmp.Integral( h_tmp.FindBin(-20), h_tmp.FindBin(100))
    else:
        h_tmp.Scale( 1/h_tmp.Integral( h_tmp.FindBin(-20), h_tmp.FindBin(100)))
        print  "Scaling after", h_tmp.FindBin(-20),  h_tmp.FindBin(100),  1/h_tmp.Integral( h_tmp.FindBin(-20), h_tmp.FindBin(100))

    h_tmp.SetAxisRange( -20., 100., "x")
    print "Mean",i_h, h_tmp.GetMean()
    h_tmp.SetLineWidth(2)
    h_tmp.SetLineColor(li_colors.pop(0))
    

    if i_h == 14: # Run that sets the scale
        h_tmp.Draw()
    else:
        h_tmp.Draw("SAME")

    leg.AddEntry( h_tmp, "Run "+str(i_h)+ " (" + dic_run_rates[i_h]+" Hz)" +" : " +  dic_run_volts[i_h]+ " V" , "L" )
    #leg.AddEntry( h_tmp, "Run "+str(i_h) +": Gain = "+ dic_gain[i_h] , "L" )

leg.Draw()


c1.SaveAs("overlayer.root")
