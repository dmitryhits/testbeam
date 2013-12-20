import ROOT

if not ROOT.gROOT.ProcessLine(".L rec.C+")==0:
   print "Error loading/compiling rec_C.so Exiting."
   sys.exit()

myRec = ROOT.rec("/shome/gregor/testbeam_data/Run7.root")
myRec.Loop()

