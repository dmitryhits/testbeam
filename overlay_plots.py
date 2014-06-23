import ROOT 
from ROOT import gROOT, TCanvas, TH1, TF1, TGraph
from shiftAndFlip import shiftAndFlip
from shiftAndFlip2 import shiftAndFlip2
from shiftFlipandScale import shiftFlipAndScale
import os
import numpy as n

basepath = "/scratch/psi_testbeam_data/may2014/DRS4_data/"
histo_name = "Integral_long"
gROOT.Reset()
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.ForceStyle()

list_samples = [ "2A87-E", "2A87-H","s129"]
list_run_types = [ "Pedestal", "Normal" ]
list_rates = ["65_75","80_80", "100_100", "130_150","200_300","500_500"]
list_rates_Hz = [ 3e3 , 3e4 , 3e5 , 3e6 , 3e7 ]
dic_rates = dict(zip(list_rates,list_rates_Hz))
list_signs = ["minus","plus"]
list_voltages = ["n500V", "p500V", "n1000V", "p1000V"]
dic_voltages = {"n500V":"-500 V","p500V":"+500 V","n1000V":"-1000 V","p1000V":"+1000 V"}
li_hs_colors = [ROOT.kBlack, ROOT.kRed, ROOT.kBlue, 8, 28, ROOT.kOrange, ROOT.kGreen, ROOT.kMagenta]*10
li_gr_colors = [ROOT.kBlack, ROOT.kRed, ROOT.kBlue, 8, 28, ROOT.kOrange, ROOT.kGreen, ROOT.kMagenta]*10
################# setup legend ###################################
legend_origin_x  = 0.15
legend_origin_y  = 0.72
legend_size_x    = 0.05
legend_size_y    = 0.22

gr_leg = ROOT.TLegend( legend_origin_x,
                            legend_origin_y,
                            legend_origin_x + legend_size_x,
                            legend_origin_y + legend_size_y )
gr_leg.SetFillColor(0)
gr_leg.SetLineColor(0)
gr_leg.SetTextSize(0.05)
gr_leg.SetShadowColor(0)

legend_origin_x  = 0.57
legend_origin_y  = 0.72
legend_size_x    = 0.10
legend_size_y    = 0.18
dic_sample_sign_leg = {}

for sample in list_samples:
    dic_sign_leg = {}
    for sign in list_signs:
        dic_sign_leg[sign] = ROOT.TLegend( legend_origin_x,
                            legend_origin_y,
                            legend_origin_x + legend_size_x,
                            legend_origin_y + legend_size_y )
        dic_sign_leg[sign].SetFillColor(0)
        dic_sign_leg[sign].SetLineColor(0)
        dic_sign_leg[sign].SetTextSize(0.05)
        dic_sign_leg[sign].SetShadowColor(0)
    dic_sample_sign_leg[sample]=dic_sign_leg
print dic_sample_sign_leg
######################################################################
#outfile = ROOT.TFile( basepath+"Run" +str(li_runs) + "_overlayed.root","OVERWRITE")
################## scan thru samples ###########################
dic_samples_files = {}
dic_samples_opened_files = {}
dic_samples_files_hs = {}
for sample in list_samples:
    list_files = os.listdir(basepath+sample+"/histos/")
    dic_samples_files[sample] = list_files
################### scan thru samples #############################
for sample in list_samples:
    opened_files=[]
    dic_hs = {}
    for i_file in dic_samples_files[sample]:
        infile = ROOT.TFile( basepath+sample+"/histos/" +i_file)    
        print infile
        if i_file.count("Pedestal") == 1:
        #if i_file.count("Pedestal") == 0:
            h =  infile.Get( histo_name ).Clone( str(i_file))
            #h =  infile.Get( "h_Mean_medium" ).Clone( str(i_file))
            h.SetDirectory(0)
            dic_hs[i_file] = h
            print i_file, h.Integral()

            opened_files.append(infile)
    dic_samples_opened_files[sample]=opened_files
    dic_samples_files_hs[sample] = dic_hs
######################## setup canvases #######################
c1 = TCanvas( 'c1', 'Overlayed Runs', 1000, 10, 1000, 1500 )
c1.Divide(2,3)
c2 = TCanvas( 'c2', 'graphs', 200, 10, 700, 500 )
c1.SetGridx()
c1.SetGridy()
c2.SetLogx()
##########################################################
li_hs = []
dic_sample_sign_rate = {}
dic_sample_sign_pedestal = {}
dic_sample_sign_pedestal_error = {}
dic_sample_sign_count = {}

####################### scan thru samples ################################
for sample in list_samples:
    rate_plus=[]
    rate_minus=[]
    ped_plus =[]
    ped_minus = []                                                                              
    ped_plus_error = []
    ped_minus_error = []
    dic_sign_rate = {"plus":rate_plus,"minus":rate_minus}
    dic_sign_pedestal = {"plus":ped_plus,"minus":ped_minus}
    dic_sign_pedestal_error = {"plus":ped_plus_error,"minus":ped_minus_error}
    dic_sign_count = {"plus":0,"minus":1}
###################################################################
 
######################## scan thru files for one sample #########################
    for i_h in sorted(dic_samples_files_hs[sample].keys()):
        h_tmp = dic_samples_files_hs[sample][i_h]
        print i_h, h_tmp.Integral()
        rate = "none"
        voltage = "none"
################# determine the rate ###########################
        for i_rate in list_rates:
            if i_h.count(i_rate) == 1:
                rate = i_rate
##################### determine the voltage ###########################
        for i_voltage in list_voltages:
            if i_h.count(i_voltage) == 1:
                voltage = i_voltage
            
        print "Mean",i_h, h_tmp.GetMean()
######################## select voltage and rate to plot ################################
        if  voltage.count("500")==1 and rate.count("200_300")==0 and rate.count("500_500")==0:
########################## set color and line width for each histo which will be plotted and add legend entry for it ########################
            h_tmp.SetLineWidth(2)
            h_tmp.SetLineColor(li_hs_colors.pop(0))
            #leg.AddEntry( h_tmp, " "+ voltage + " : " + str(dic_rates[rate]/1000) + " kHz/cm2", "L" )
############################################# find peak and fit a gaussian to each histo #####################
            low = h_tmp.GetBinCenter(h_tmp.GetMaximumBin())- h_tmp.GetRMS()
            high = h_tmp.GetBinCenter(h_tmp.GetMaximumBin())+ h_tmp.GetRMS()
            h_tmp.Fit("gaus","M0","",low,high)
######################### scan thru both voltage signs ###################################
            for sign in list_signs:
                dic_sample_sign_leg[sample][sign].AddEntry( h_tmp, dic_voltages[voltage] + str(dic_rates[rate]/1000) + " kHz/cm2", "L" )
                dic_sign_pedestal[sign].append(h_tmp.GetFunction("gaus").GetParameter(1))
                dic_sign_pedestal_error[sign].append(h_tmp.GetFunction("gaus").GetParError(1))
                dic_sign_rate[sign].append(dic_rates[rate])
                print "INDEX : "+ str(1+list_signs.index(sign)+2*list_samples.index(sample)) +" SAMPLE :" + sample
                print "SIGN : " + sign + " : VOLTAGE " + voltage
                c1.cd(1+list_signs.index(sign)+2*list_samples.index(sample))
                h_tmp.Scale(100/h_tmp.Integral(h_tmp.FindBin(-100),h_tmp.FindBin(100)) )
                if dic_sign_count[sign] == 0: # Run that sets the scale
                    h_tmp.Draw()
                    h_tmp.SetAxisRange( -50., 50., "x")
                    h_tmp.SetAxisRange( 0, 10., "y")
                    h_tmp.SetTitle(sample)
                else:
                    h_tmp.Draw("SAME")
                    dic_sample_sign_leg[sample][sign].Draw()
                print
                dic_sign_count[sign]+=1
    
    
    #leg.AddEntry( h_tmp, "Run "+str(i_h) +": Gain = "+ dic_gain[i_h] , "L" )
    print "plus counts for " + sample + " is " + str(dic_sign_count["plus"])
    print "minus counts for " + sample + " is " + str(dic_sign_count["minus"])
    dic_sample_sign_rate[sample]=dic_sign_rate 
    dic_sample_sign_pedestal[sample]=dic_sign_pedestal
    dic_sample_sign_pedestal_error[sample]= dic_sign_pedestal_error 
    dic_sample_sign_count[sample] = dic_sign_count

c2.cd()

#c2.GetPad(1).SetLogx()
dic_samples_gr = {}
for sample in list_samples:
    for sign in ["plus","minus"]:
        i_gr = sample+sign
        print i_gr
        
        dic_samples_gr[i_gr] = ROOT.TGraphErrors(dic_sample_sign_count[sample][sign],n.array(dic_sample_sign_rate[sample][sign]),n.array(dic_sample_sign_pedestal[sample][sign]),n.array(dic_sample_sign_pedestal_error[sample][sign]))
        dic_samples_gr[i_gr].SetMarkerStyle(4)
        dic_samples_gr[i_gr].SetMarkerSize(2)
        dic_samples_gr[i_gr].SetMarkerColor(li_gr_colors.pop(0))
        gr_leg.AddEntry(dic_samples_gr[i_gr], " " +sign +" " +sample, "P" )
        if list_samples.index(sample)==0 and sign == "plus":
            dic_samples_gr[i_gr].Draw("AP")
            dic_samples_gr[i_gr].GetYaxis().SetRangeUser(-25.,20.)
            dic_samples_gr[i_gr].GetXaxis().SetTitle("rate, Hz")
            dic_samples_gr[i_gr].SetTitle(histo_name)
        else:
            dic_samples_gr[i_gr].Draw("P")
            gr_leg.Draw()
c2.SetGridx()
c2.SetGridy()

c1.SaveAs(os.getcwd() + "/plots/"+ histo_name + "_pedestal_all_samples.pdf")
c2.SaveAs(os.getcwd() + "/plots/" + histo_name + "_peak_pedestal_graph_all_samples.pdf")
