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


########################################
# Prepare Output
########################################

# File and Tree
outfile = ROOT.TFile( input_filename.replace(".dat",".root"), 'recreate')
outtree = ROOT.TTree( 'tree', 'tree' )

# Create variables ...
chn1 = ROOT.std.vector( float )()
chn2 = ROOT.std.vector( float )()
chn3 = ROOT.std.vector( float )()
chn4 = ROOT.std.vector( float )()
var_n = array.array('i',[0])

# .. And add to tree
setattr( outtree, "chn1", chn1 )
setattr( outtree, "chn2", chn2 )
setattr( outtree, "chn3", chn3 )
setattr( outtree, "chn4", chn4 )

outtree.Branch("chn1", chn1)
outtree.Branch("chn2", chn2)
outtree.Branch("chn3", chn3)
outtree.Branch("chn4", chn4)

outtree.Branch("n", var_n   ,"n/I");  

# Dictionary to look up the channels by name 
dic_channels = { 
    "1" : chn1,
    "2" : chn2,
    "3" : chn3,
    "4" : chn4 }
                      

########################################
# Actual Wotk
########################################

# Get the EHDR at the top of the file out of the way
event_header = f.read(4)

# This is the main loop
# One iteration corresponds to reading one channel
# every few channels (1-4) a new event can start 
# We know that this if the case if we see "EHDR" instead of "C00x" (x=1..4)
# If we have a new event: Fill the tree, reset the branches, increment event counter
# The binary format is described in:
# http://www.psi.ch/drs/DocumentationEN/manual_rev40.pdf
# (page 21)

is_new_event = True

while True:

    # Handle new events
    if is_new_event:
        
        is_new_event = False

        # Reset the vector branches
        chn1.resize( 0 )
        chn2.resize( 0 )
        chn3.resize( 0 )
        chn4.resize( 0 )

        # Read the serial number and timing info 
        # (do nothing with it for now)
        fluff = f.read(4*1029)

        # Increment event counter
        var_n[0] += 1
        
        # Let the (impatient) user know what's going on
        if (var_n[0] % 100)==0:
            print var_n[0]
    # end of is_new_event

    # Read the header, this is either 
    #  EHDR -> begin new event
    #  C00x -> read the data
    #  ""   -> end of file
    header = f.read(4)
    
    # End of File
    if header == "":
        outtree.Fill()            
        break

    # End of Event
    elif header == "EHDR":
        outtree.Fill()            
        is_new_event = True            
        continue

    # Read and store data
    else:            
        # the voltage info is 1024 floats with 2-byte precision
        channel = dic_channels[header[-1]]
        li_voltage_int    =  unpack('H'*1024, f.read(2048))
        for x in li_voltage_int:
            channel.push_back( ((x / 65535.) - 0.5) * 1000  )

# End of main loop

# Clean up
f.close()
outtree.Write()
outfile.Close()
