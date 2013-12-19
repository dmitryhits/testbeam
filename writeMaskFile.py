#!/usr/bin/env python
"""
Write module.ini to mask some pixels
"""

########################################
# General Imports
########################################

import sys
import array
import ROOT
import pickle

########################################
# General configuration
########################################

outfile_name = "module.ini"

# Columns go from 0 to 51
# Rows go from 0 to 79
abs_max_col = 51
abs_max_row = 79


########################################
# checkInputs
########################################

def checkInputs( min_col, min_row, max_col, max_row ):

    # Make sure all cols/rows are larger than zero
    if any( x<0 for x in [min_col, min_row, max_col, max_row]) :
        print "One input col/row smaller zero. Exiting."
        sys.exit()

    # Make sure the columns numbers dont exceed the maximum
    if any( x>abs_max_col for x in [min_col, max_col] ):
        print "One col exceeds max. Exiting.."
        sys.exit()

    # Make sure the row numbers dont exceed the maximum
    if any( x>abs_max_row for x in [min_row, max_row] ):
        print "One col exceeds max. Exiting.."
        sys.exit()

    # Make sure the min col/rows are larger than max col/rows
    if (min_col > max_col) or (min_row > max_row):
        print "min col/row > max col/row. Exiting.."
        sys.exit()
# end of checkInput



########################################
# createMask
########################################

def createMask( min_col, min_row, max_col, max_row ):
    ''' 
    Create a pixel mask and write to output file.

    We will mask everything OUTSIDE of the given coordinates
    min_col will NOT be masked
    max_col will NOT be masked
    min_row will NOT be masked
    max_row will not be masked    
    '''

    # make sure that the inputs are sensible
    checkInputs( min_col, min_row, max_col, max_row )

    print "Input ok, will write mask to", outfile_name
    
    # prepare IO
    out_f = open( outfile_name, "w" )
    
    # Count how many pixels we masked
    cnt_mask = 0
    
    # And do the actual work
    # Loop over columns and rows
    for i_row in range( abs_max_row + 1 ):
        for i_col in range( abs_max_col + 1 ):
                        
            # check if we want to mask this pixel
            if not (( min_col <= i_col <= max_col ) and ( min_row <= i_row <= max_row)):
                cnt_mask += 1
                out_str = " ".join( ["pixd", str(i_col), str(i_row) ] ) + "\n"
                out_f.write( out_str)
            # end of maskin pixel
        # end of loop over columns
    # end of loop over rows

    out_f.close()
    print "Done, ", cnt_mask, "pixels masked"

# end of createMask
    

#########################################
# Allow use from the shell 
#########################################

# If callled from the command-line: execute createMask
if __name__ == "__main__":

    # Make sure we have exactly one argument
    if len(sys.argv)==5:
        createMask( int(sys.argv[1]), 
                    int(sys.argv[2]), 
                    int(sys.argv[3]), 
                    int(sys.argv[4]) )
    else:
        print "Wrong number of arguments. Usage: "
        print "python writeMaskFile min_col min_row max_col max_row"
        print "Everything OUTSIDE the given coordinates will be masked!"
    # end of checking input
# end of treating command_line call
