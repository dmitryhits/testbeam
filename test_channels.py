#!/usr/bin/env python
"""
Script to check which channels are activated in binary file created by
DRS4 evaluation boards.
http://www.psi.ch/drs/evaluation-board

Gregor Kasieczka, ETHZ, 2014-01-15
"""

########################################
# Imports
########################################

import sys


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
# do_read
########################################

def do_read( f ):
    """ Helper function to read one byte from file (argument f).
    Increment global byte (inside word) and word counters.
    """

    global cnt_bytes
    global cnt_words

    byte = f.read(1)

    cnt_bytes += 1
    
    # If we have 4 bytes, go to the next word
    if cnt_bytes == 4:
        cnt_bytes = 0
        cnt_words += 1
        
    return byte
# end of do_read


########################################
# Process File
########################################

# Initialize Counters
cnt_bytes = -1                        
cnt_words = 0

# List to store which channels we found
li_channels = []

try:
    byte = do_read( f )
                        
    # We only need to look at the first couple of thousand words to
    # see which channels are present
    while byte != "" and cnt_words < 3000:

        # At these locations the channel info is stored.
        # The words can be C001, C002, C003, C004, EHDR
        # (EHDR means we reached the next event)
        # Source:
        # http://www.psi.ch/drs/DocumentationEN/manual_rev40.pdf
        # (On page 21)
        # This means looking at the last byte in the the words at the
        # special locations will tell us everything
        if (cnt_words == 1030) or (cnt_words == 1543) or (cnt_words == 2056) or (cnt_words == 2569):
            if cnt_bytes == 3:
                if byte == "R":
                    break
                else:
                    li_channels.append( byte )

        # And read the next one 
        byte = do_read( f )
    # End of loop over bytes        
finally:
    f.close()


print "Active channels: ", li_channels
