import ROOT

if not ROOT.gROOT.ProcessLine(".L rec.C+")==0:
   print "Error loading/compiling rec_C.so Exiting."
   sys.exit()

basepath = "/shome/gregor/testbeam_data/"
run      = "Run7"


myRec = ROOT.rec( basepath + run + ".root")
myRec.Loop()

li_hs = [ROOT.histo,
         ROOT.histo1,
         ROOT.histo2,
         ROOT.histo3]

outfile = ROOT.TFile( basepath+run + "_histos.root", "RECREATE" )
outfile.cd()

for h in li_hs:
    h.Write()

outfile.Close()
