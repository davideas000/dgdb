import re
import sys
import os

infilename = sys.argv[1]
infile = open(infilename)
outfilename = sys.argv[1] + "tmp"
outfile = open(outfilename, "w")

lregex = re.compile("====$")
bbregex = re.compile("testb")
beregex = re.compile("teste")
line = infile.readline()
lineno = 1

while line:
    match = re.search(lregex, line)
    if match:
        line = infile.readline()
        lineno += 1
        continue

    match = re.search(bbregex, line)
    if match:
        match = re.search(beregex, line)
        while line and not match:
            match = re.search(beregex, line)
            line = infile.readline()
            lineno += 1

    outfile.write(line)
    line = infile.readline()
    lineno += 1


infile.close()
outfile.close()

os.rename(infilename, infilename + "bak")
os.rename(outfilename, infilename)
