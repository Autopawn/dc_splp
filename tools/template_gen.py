import numpy as np

from sys import argv

"""
This program creates a SPLP problem with the given variables, giving random positions for instalations and clients.

n: number of potential facilities.
m: number of clients.
size: of the space where the points are located.
"""

def random_pos(fname,n,m,size):
    fcost="<<FCOST>>"
    pp="<<PP>>"
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

    # Create file with the positions:
    fp = open(fname+"_pos","w")
    fp.write("! fcost %s\n"%fcost)
    fp.write("! pp %s\n"%pp)
    fp.write("d %d %d\n"%(size,size))
    for i in range(n):
        fp.write("f %d %d\n"%(fxs[i],fys[i]))
    for j in range(m):
        fp.write("c %d %d\n"%(cxs[j],cys[j]))
    fp.close()

    # Create a linear SPLP and PMEDIAN programming problem
    fp_splp = open(fname+"_lp_splp","w")
    fp_pmpm = open(fname+"_lp_pm","w")
    #
    fp_splp.write("min:")
    fp_pmpm.write("min:")
    for i in range(n):
        fp_splp.write(" +%s X%d"%(fcost,i))
    for j in range(m):
        for i in range(n):
            fp_splp.write(" +%d Y%dc%d"%(int(round(dst[i][j])),j,i))
            fp_pmpm.write(" +%d Y%dc%d"%(int(round(dst[i][j])),j,i))
    fp_splp.write(";\n\n")
    fp_pmpm.write(";\n\n")

    for i in range(n):
        fp_pmpm.write("+X%d "%(i))
    fp_pmpm.write("<= %s"%(pp))
    fp_pmpm.write(";\n\n")

    #
    for j in range(m):
        for i in range(n):
            fp_splp.write("+Y%dc%d "%(j,i))
            fp_pmpm.write("+Y%dc%d "%(j,i))
        fp_splp.write("= 1;\n")
        fp_pmpm.write("= 1;\n")
    fp_splp.write("\n")
    fp_pmpm.write("\n")
    #
    for i in range(n):
        for j in range(m):
            fp_splp.write("Y%dc%d <= X%d;\n"%(j,i,i))
            fp_pmpm.write("Y%dc%d <= X%d;\n"%(j,i,i))
    fp_splp.write("\n")
    fp_pmpm.write("\n")
    #
    fp_splp.write("bin "+",".join(["X%d"%i for i in range(n)])+";\n")
    fp_pmpm.write("bin "+",".join(["X%d"%i for i in range(n)])+";\n")
    #
    fp_splp.close()
    fp_pmpm.close()

    # Create dsa problem:
    fp_splp = open(fname+"_dsa_splp","w")
    fp_pmpm = open(fname+"_dsa_pm","w")
    #
    fp_splp.write("%s\n"%fcost)
    fp_pmpm.write("-%s\n"%pp)

    fp_splp.write("%d\n"%1)
    fp_pmpm.write("%d\n"%1)
    fp_splp.write("%d\n"%n)
    fp_pmpm.write("%d\n"%n)
    fp_splp.write("%d\n"%m)
    fp_pmpm.write("%d\n"%m)
    fp_splp.write("\n")
    fp_pmpm.write("\n")
    #
    for i in range(n):
        fp_splp.write("%d %d\n"%(fxs[i],fys[i]))
        fp_pmpm.write("%d %d\n"%(fxs[i],fys[i]))
    fp_splp.write("\n")
    fp_pmpm.write("\n")
    for i in range(m):
        fp_splp.write("%d %d\n"%(cxs[i],cys[i]))
        fp_pmpm.write("%d %d\n"%(cxs[i],cys[i]))
    fp_splp.write("\n")
    fp_pmpm.write("\n")
    #
    for i in range(m):
        fp_splp.write("%d "%1)
        fp_pmpm.write("%d "%1)
    fp_splp.write("\n\n")
    fp_pmpm.write("\n\n")
    #
    fp_splp.close()
    fp_pmpm.close()


if __name__ == '__main__':
    if len(argv)!=5:
        print("Usage: python %s <n> <m> <size> <outfile>"%argv[0])
    else:
        random_pos(argv[-1],int(argv[1]),int(argv[2]),int(argv[3]))
