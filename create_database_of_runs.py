import sqlite3
conn = sqlite3.connect('runs_july2014.db')

c = conn.cursor()
c.execute('''CREATE TABLE runs
             (diamond text, run_type text, rate text, voltage text, duration text, comments text)''')

# Insert a row of data
c.execute("INSERT INTO runs VALUES ('2006-01-05','BUY','RHAT',100,35.14)")

# Save (commit) the changes
conn.commit()

# We can also close the connection if we are done with it.
# Just be sure any changes have been committed or they will be lost.
conn.close()
