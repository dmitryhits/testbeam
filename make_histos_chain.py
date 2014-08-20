import os
from ROOT import gSystem
gSystem.Load("rec_C")
from ROOT import rec

#trigger_delay = 10
#runs = ["010","011","012","013","014","015","016","017","018","019","020","021","022","023","024","025"]
trigger_delay = 13
runs = ["075","076","077","078","079","080","081","082","083","084","085","086","087","088","089","090"]
#data_dir = "/Volumes/TRANSCEND/data/runs_drs4/" #Data mounted on mac
data_dir = "/home/ipp/PLT/padData/" # Data at ipp home share
root_files = []
dat_files = []
for pad_file in os.listdir(data_dir):
    for run in runs:
        if pad_file.count(run+".root") == 1:
            root_files.append(pad_file)
root_files.sort()
for root_file in root_files:
    #if root_files.count(pad_dat_file) == 0:
    print "Determining timing for the file " + root_file 
    t = rec(data_dir + root_file,trigger_delay)
    t.Loop()
    #else:
    #    print "the pad file " + pad_dat_file + " is already decode"


