_SAMPLES = ""
_COUNT = 16000000
_NUMFILES = 5
_DATA = "3111.416667,115.216667,3113.866667,113.133333"

for i in range(1,_NUMFILES):
    print("Write file S{0} ...".format(i))
    f = open("S{0}.csv".format(i), "w+")
    for j in range(1,_COUNT):
        f.write("{0},{1}\n".format(j, _DATA))
    
    f.close()
