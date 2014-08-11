import ROOT
import sys

if len(sys.argv)<2:
   print "wrong number of input arguments. Usage:"
   print "python rec.py RunX"
   print "(per default runs are read from /shome/gregor/testbeam_data/)"
   

if not ROOT.gROOT.ProcessLine(".L rec.C+")==0:
   print "Error loading/compiling rec_C.so Exiting."
   sys.exit()

basepath = "/shome/gregor/testbeam_data/"
run      = sys.argv[1]


myRec = ROOT.rec( basepath + run + ".root")
myRec.Loop()

li_hs = [ROOT.histo,
         ROOT.histo1,
         ROOT.histo2,
         ROOT.histo3]

outfile = ROOT.TFile( run + "_histos.root", "RECREATE" )
outfile.cd()

for h in li_hs:
    h.Write()

outfile.Close()
