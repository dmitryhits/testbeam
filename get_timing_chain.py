import os
from ROOT import gSystem
gSystem.Load("get_trigger_delay_C")
from ROOT import get_trigger_delay

runs = ["10", "11", "27", "31", "32", "33", "36", "47", "50", "55", "67", "71", "75", "87", "91", "103", "108", "110"]
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
    t = get_trigger_delay(data_dir + root_file)
    t.Loop()
    #else:
    #    print "the pad file " + pad_dat_file + " is already decode"


