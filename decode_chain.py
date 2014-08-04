import os
from ROOT import gSystem
gSystem.Load("decode_C")
from ROOT import decode

data_dir = "/Volumes/TRANSCEND/data/runs_drs4/"
root_files = []
dat_files = []
for pad_file in os.listdir(data_dir):
    if pad_file.count(".root") == 1:
        root_files.append(pad_file.rstrip(".root"))
    if pad_file.count(".dat") == 1:
        dat_files.append(pad_file.rstrip(".dat"))
for pad_dat_file in dat_files:
    if root_files.count(pad_dat_file) == 0:
        print "decoding pad file " + pad_dat_file 
        decode(data_dir + pad_dat_file)
    else:
        print "the pad file " + pad_dat_file + " is already decode"


