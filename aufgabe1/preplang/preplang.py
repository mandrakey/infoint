#!/usr/bin/env python3

import os
import re

_COUNTRIES = {};

def loadCountries(path):
    if path is None or path == "":
        raise "Need path."
    
    global _COUNTRIES
    f = open(path)
    for line in f:
        tmp = line.split("\t")
        code = tmp[0].strip()
        name = tmp[1].strip().lower()
        
        if name not in _COUNTRIES:
            _COUNTRIES[name] = code

def setCsvData(path):
    if path is None or path == "":
        raise "Need path."
    
    global _COUNTRIES
    d = os.listdir(path)
    nameRx = r".+\.csv$"
    
    for f in d:
        m = re.match(nameRx, f)
        if m is None:
            continue
        
        print("\nWorking on {0}:".format(f.replace(".csv", "")))
        outf = open(f + ".out", "w")
        inpf = open(f)
        
        for inline in inpf:
            tmp = inline.split(";")
            if len(tmp) < 2:
                continue
            country = tmp[1].strip().lower()
            
            if len(country) > 2:
                if country in _COUNTRIES:
                    country = _COUNTRIES[country]
                else:
                    code = raw_input("Give code for '{0}': ".format(country))
                    _COUNTRIES[country] = code
                    country = code
            else:
                country = country.upper()
            
            outf.write("{0};{1}\n".format(tmp[0], country));
        
        outf.close()
        inpf.close()

def main():
    global _COUNTRIES
    
    loadCountries("countries.txt")
    for c in _COUNTRIES:
        print(c)
    
    setCsvData(".")

if __name__ == "__main__":
    main()
