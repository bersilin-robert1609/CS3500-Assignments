# open file output and create table of data

import sys

# open file for reading

try:
    f = open("output.txt", 'r')
except:
    print("Cannot open file", "output.txt")
    sys.exit(1)

# read file and create table

table_rows = [1, 2, 3, 4, 5, 6]
table_cols = [500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500]

line = f.readline()

print("**Algo**", end="|")
for col in table_cols:
    print("**" + str(col) + "**", end="|")
print()

for row in table_rows:
    print("**" + str(row) + "**", end="|")
    for col in table_cols:
        line = f.readline()
        # remove newline character
        line = line[:-1]
        print(line, end="|")
    print()

f.close()