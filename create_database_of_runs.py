import sqlite3
import csv

conn = sqlite3.connect('runs_july2014.db')

c = conn.cursor()
c.execute('''CREATE TABLE runs
             (run INT, diamond TEXT, run_type TEXT, rate TEXT, voltage TEXT, duration TEXT, comments TEXT)''')

csvfile = open('run_summary_for_july_testbeam.csv', 'rU')
csvreader = csv.reader(csvfile, delimiter=',')
row_number = 0
for row in csvreader:
    if row_number>0:
#        print row
        type = ""
        if row[2] == "yes":
            type = "SIGNAL" 
        elif row[3] == "yes":
            type = "PEDESTAL"
        else:
            print "ERROR: for run number " + row[0]+  " the data neither signal nor pedestal"
        new_row = [int(row[0]),row[1], type, row[4],row[5], row[6], row[7]]
        # Insert a row of data
        c.execute("INSERT INTO runs VALUES (?,?,?,?,?,?,?)",new_row)

    row_number+=1


# Save (commit) the changes
conn.commit()

# We can also close the connection if we are done with it.
# Just be sure any changes have been committed or they will be lost.
conn.close()
