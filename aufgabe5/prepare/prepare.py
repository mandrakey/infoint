#!/usr/bin/env python

import random

def createList(amount, lower, upper):
    l = []
    for i in range(0, amount):
        num = random.randrange(lower, upper)
        while l.count(num) != 0:
            num = random.randrange(lower, upper)
        
        l.append(num)
    
    return l

def splitLine(line, separator):
    literal = False
    tokens = []
    token = ""
    
    for c in line:
        if literal:
            token = token + c
            if c == "\"":
                literal = False
        else:
            if c == separator:
                tokens.append(prepToken(token))
                token = ""
            elif c == "\"":
                token = token + c
                literal = True
            else:
                token = token + c
    
    if token != "":
        tokens.append(prepToken(token))
    
    return tokens

def prepToken(token):
    if token.count(";") > 0:
        token = "\"" + token + "\""
    return token

# ==============================================================================
# Processing

infile = None
outfile = None

try:
    infile = open("5Engines.csv", "r")
    outfile = open("horst.csv", "w+")
except OSError as e:
    print("Error opening a file: {0}\n".format(e))

# Calculate ID lists
tuplesCount = random.randrange(70, 150)
attribCount = random.randrange(5, 7)
tuplesList = createList(tuplesCount, 1, 483)
#~ tuplesList = [ 25 ]
attribList = createList(attribCount, 1, 23)
#~ attribList = [ 1, 4, 6, 7, 9, 23 ]

# Process file
i = 0
for line in infile:
    if tuplesList.count(i) != 0:
        
        tmp = splitLine(line, ",")
        #~ print(tmp[22])
        newline = []
        for j in range(1,len(tmp) + 1):
            if attribList.count(j) != 0:
                newline.append(tmp[j - 1])
        
        # Write to target
        for j in range(0,len(newline)):
            outfile.write(newline[j])
            if (j < len(newline) - 1):
                outfile.write(";")
        outfile.write("\n")
    
    i = i + 1

infile.close()
outfile.close()

print("Wrote {0} tuples.\n".format(tuplesCount))
                
