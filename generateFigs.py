import sys
import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cmx
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.pyplot import imshow

def getMatrixFromFile(filename, u):
    try:
        file_ = open(filename, 'r');
        x = 0;
        y = 0;
        for content in file_.readlines():
            print('content:'+str(content))
            content = content.rstrip()
            content = content.split(' ')
            print('content:'+str(content))
            for v in content:
                print('v:' + str(v))
                u[x][y] = float(v)
                y += 1;
            x += 1
        file_.close();
        return u
    except (OSError, IOError) as exc:
        print("Your file could not be open to, your exception details are: {}".format(exc))
        return None

def initializeGraphParameters():
    global scalarMap, cm;
    cm = plt.get_cmap('binary')
    cNorm  = colors.Normalize(vmin=0, vmax=1)
    scalarMap = cmx.ScalarMappable(norm=cNorm, cmap=cm)

def createFigure(title, x, u, maxDensity, minDensity):
    global cm
    fig = plt.figure(figsize=(10,7));
    ax = fig.gca();
    plt.title(title, fontsize=17);
    plt.xlabel('x',fontsize=17);
    plt.ylabel('y',fontsize=17);
    plt.axis([min(x), max(x), min(x), max(x)])
    pcm = plt.pcolormesh(x, x, u[t], cmap=cm)
    plt.colorbar(pcm)
    return [fig,ax];

def generateGraphs(fig, filename):
    #pp = PdfPages(filename + '.pdf')
    #pp.savefig(fig,bbox_inches='tight')
    #pp.close()
    fig.savefig(filename + '.svg', format='svg', dpi=1200,bbox_inches='tight')
    fig.savefig(filename + '.jpg', format='jpg', bbox_inches='tight')


#Main
T = 1
SIZEY = 30
ac = [[0. for y in range(0,T)] for x in range(0,30)]
initializeGraphParameters()
color_index = 0
colorVal = scalarMap.to_rgba(color_index)
ACSIZE = 30
acfilename = "acconf.txt"
getMatrixFromFile(acfilename,ac)
[fig1,ax1] = createFigure('1d Cellular automata',ACSIZE,ac);
ax1.legend(loc='upper right')
generateGraphs(fig1,acfilename);
