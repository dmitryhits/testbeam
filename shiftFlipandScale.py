import ROOT 
from ROOT import gROOT, TCanvas, TH1

n_count  =0

def shiftFlipAndScale( h ):
    ''' find the position of the pedestal and pion peak
    pedestal is the highest bin
    pion is the highest bin of the second highest bin
    shift the histogram so the pedestal is at zero
    if the pion peak is left of the pedestel, invert around the pedestal
    
    Arguments: h ROOT TH1
    returns a new histogram
    '''

    peak1=0.
    peak2=0.
    iPeak1=-999
    iPeak2=-999

    global n_count

    DEBUG = False

    #Find First Peak (pedestal)
    iPeak1=h.GetMaximumBin()
    peak1=h.GetMaximum()
    #Find second largest peak
    for iBin in range(2,h.GetNbinsX()):
        if iBin > 4+iPeak1 or iBin<iPeak1-4:
            if h.GetBinContent(iBin) > peak2 and h.GetBinContent(iBin)>h.GetBinContent(iBin+1) and h.GetBinContent(iBin)>h.GetBinContent(iBin-1):
                peak2= h.GetBinContent(iBin)
                iPeak2=iBin
                
    print "Highest peak before shift " + str(h.GetBinCenter(iPeak1)) + " with height " + str(peak1)
    print "Lowest peak before shift " + str(h.GetBinCenter(iPeak2)) + " with height " + str(peak2)

    h2 = h.Clone("cloned"+str(n_count))
    h2.SetDirectory(0)
    n_count += 1
    #Shift
    for iBin in range(1,h.GetNbinsX()+1):

        newBin = iBin - iPeak1 + h.FindBin(0)

        if newBin <= 0:
            continue

        if newBin >= h.GetNbinsX()+1:
            continue
        h2.SetBinContent( newBin, h.GetBinContent(iBin))
    # end of loop over bins


    #Find second peak
    for iBin in range(2,h2.GetNbinsX()):
        if iBin > 4+h2.FindBin(0) or iBin<h2.FindBin(0)-4:
            if h2.GetBinContent(iBin) > peak2 and h2.GetBinContent(iBin)>h2.GetBinContent(iBin+1) and h2.GetBinContent(iBin)>h2.GetBinContent(iBin-1):
                peak2= h2.GetBinContent(iBin)
                iPeak2=iBin
                
    # end of loop over bins

    print "Peaks: ", h2.GetBinCenter(iPeak1), h2.GetBinCenter( iPeak2)

    # Check if we need a flip
    if h2.GetBinCenter( iPeak2)<0: ##Flip
        print "FLIPPING"
        for iBin in range(1,h2.GetNbinsX()+1):
            if h2.GetBinCenter(iBin)>0:break
            iBinOpp=h2.FindBin( -h2.GetBinCenter(iBin) )
            minus=h2.GetBinContent(iBin)
            plus=h2.GetBinContent(iBinOpp)
            h2.SetBinContent(iBin,plus)
            h2.SetBinContent(iBinOpp,minus)
        # end of loop over bins
    # end of flipping
    #Horizontal Scaling
    print "SCALING"
    print "Histogram has " + str(h.GetNbinsX()) + " and ranges from min : " + str(h.GetXaxis().GetXmin()) + "; max :   " + str(h.GetXaxis().GetXmax())
    h_scaled = ROOT.TH1F("h_scaled", "h_scaled", h.GetNbinsX(), 0.658*h.GetXaxis().GetXmin(), 0.658*h.GetXaxis().GetXmax())
    for iBin in range(1,h_scaled.GetNbinsX()+1):
        h_scaled.SetBinContent(iBin,h2.GetBinContent(iBin))
    print "DONE SCALING"
    if DEBUG:
        print h.Integral(), h2.Integral()

    print h_scaled
    return h_scaled
# end of shiftAndFlip
