#!/usr/bin/env python
"""
Script to convert binary format to root
for DRS4 evaluation boards.
http://www.psi.ch/drs/evaluation-board

Gregor Kasieczka, ETHZ, 2014-01-15
based on decode.C by Dmitry Hits
"""

########################################
# Imports
########################################

import sys
import ROOT
from struct import unpack
import array



########################################
# Prepare Input
########################################

if not len(sys.argv) == 2:
    print "Wrong number of arguments!"
    print "Usage: python test_channels.py filename.dat"
    print "Exiting..."
    sys.exit()

input_filename = sys.argv[1]

f = open( input_filename, "rb")


outfile = ROOT.TFile( 'foo.root', 'recreate')

outtree = ROOT.TTree( 'tree', 'tree' )

var_n = array.array('i',[0])
chn1 = ROOT.std.vector( float )()
chn2 = ROOT.std.vector( float )()
chn3 = ROOT.std.vector( float )()
chn4 = ROOT.std.vector( float )()


outtree.Branch("n", var_n   ,"n/I");  


setattr( outtree, "chn1", chn1 )
setattr( outtree, "chn2", chn2 )
setattr( outtree, "chn3", chn3 )
setattr( outtree, "chn4", chn4 )

outtree.Branch("chn1", chn1)
outtree.Branch("chn2", chn2)
outtree.Branch("chn3", chn3)
outtree.Branch("chn4", chn4)

                      


########################################
# Process File
########################################

dic_channels = { "1" : chn1,
                 "2" : chn2,
                 "3" : chn3,
                 "4" : chn4 }

try:        

    event_header = f.read(4)
    is_new_event = True

    while True:

        if is_new_event:
            is_new_event = False

            chn1.resize( 0 )
            chn2.resize( 0 )
            chn3.resize( 0 )
            chn4.resize( 0 )

            fluff = f.read(4*1029)
            var_n[0] += 1


            print var_n[0]
        
        header = f.read(4)

        if header == "EHDR":
            outtree.Fill()            
            is_new_event = True            
            continue
        else:
            channel = dic_channels[header[-1]]

        for voltage_bin in range(1024):
            voltage_int    =  unpack('H', f.read(2))[0]
            voltage_double = ((voltage_int / 65535.) - 0.5) * 1000
            channel.push_back( voltage_double )


finally:
    f.close()
    outtree.Write()
    outfile.Close()
