import numpy as np

from sys import argv

"""
This program creates a SPLP problem with the given variables, giving random positions for instalations and clients.

n: number of potential facilities.
m: number of clients.
size: of the space where the points are located.

fcost: facility cost.
"""

def random_pos(fname,n,m,size,fcost):
    # Create positions x and y and write them to pos_fname
    xs = np.random.randint(0,size,m+n)
    ys = np.random.randint(0,size,m+n)
    fxs = xs[:n]
    fys = ys[:n]
    cxs = xs[n:]
    cys = ys[n:]
    dx = np.array([fxs]).T - np.array([cxs])
    dy = np.array([fys]).T - np.array([cys])
    dst = np.sqrt(dx**2 + dy**2)
    fdx = np.array([fxs]).T - np.array([fxs])
    fdy = np.array([fys]).T - np.array([fys])
    fdst = np.sqrt(fdx**2 + fdy**2)

    # Create file with the positions:
    fp = open(fname+"_pos","w")
    fp.write("! fcost %d\n"%fcost)
    fp.write("d %d %d\n"%(size,size))
    for i in range(n):
        fp.write("f %d %d\n"%(fxs[i],fys[i]))
    for j in range(m):
        fp.write("c %d %d\n"%(cxs[j],cys[j]))
    fp.close()

    # Create a linear programming problem:
    fp = open(fname+"_lp","w")
    fp.write("min:")
    for i in range(n):
        fp.write(" +%d X%d"%(fcost,i))
    for j in range(m):
        for i in range(n):
            fp.write(" +%d Y%dc%d"%(dst[i][j],j,i))
    fp.write(";\n\n")
    #
    for j in range(m):
        for i in range(n):
            fp.write("+Y%dc%d "%(j,i))
        fp.write("= 1;\n")
    fp.write("\n")
    #
    for i in range(n):
        for j in range(m):
            fp.write("Y%dc%d <= X%d;\n"%(j,i,i))
    fp.write("\n")
    #
    fp.write("bin "+",".join(["X%d"%i for i in range(n)])+";\n")
    #
    fp.close()

    # Create dsa problem:
    fp = open(fname+"_dsa","w")
    #
    fp.write("%d\n"%fcost)
    fp.write("%d\n"%1)
    fp.write("%d\n"%n)
    fp.write("%d\n"%m)
    fp.write("\n")
    #
    for i in range(n):
        for j in range(n):
            fp.write("%d "%fdst[i][j])
        fp.write("\n")
    fp.write("\n")
    #
    for i in range(m):
        fp.write("%d "%1)
    fp.write("\n")
    fp.write("\n")
    #
    for i in range(n):
        for j in range(m):
            fp.write("%d "%dst[i][j])
        fp.write("\n")
    #
    fp.close()


if __name__ == '__main__':
    if len(argv)!=6:
        print("Usage: python %s <n> <m> <size> <fcost> <outfile>"%argv[0])
    else:
        random_pos(argv[-1],int(argv[1]),int(argv[2]),int(argv[3]),int(argv[4]))
