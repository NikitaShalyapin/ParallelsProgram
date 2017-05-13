# -*- coding: cp1251 -*-

import sys
import subprocess
from math import sqrt

# arguments
args = list(sys.argv )

programm = "C:\Users\Admin\Documents\Visual Studio 2013\Projects\ParallelsProgramProject\DeterminantRecursionPthreadMPI\Debug\DeterminantRecursionPthreadMPI.exe"
numRepeats = 100

if len(args) >= 2:    
    programm = args [1]
    numRepeats = int ( args [2])
    


# program
PIPE = subprocess.PIPE

for threads in [1, 2, 4, 8]:#[4]:#
    timeList = []
    for num in range(numRepeats):
        p = subprocess.Popen([programm, inputFile, str(threads)], stdout=PIPE)
        for line in p.stdout:
            if 'runtime without reading = ' in line :
                timeList.append(int(line.split()[-1]))        

    m=sum(timeList)/numRepeats
    disp = 0.00
    for val in timeList :
        disp = disp + (val - m) ** 2
        
    if numRepeats == 1:
        disp = disp / numRepeats
    else :
        disp = disp / ( numRepeats - 1)

    sigma = sqrt(disp)

    t=1.984
    interHigh = m + t*(sigma/(sqrt(numRepeats)))
    interLow = m - t*(sigma/(sqrt(numRepeats)))
    print("{} threads : average = {}, dispersion = {}, interval = [{}, {}]".format(threads , m , disp, interHigh, interLow))


