import os

import numpy as np

import matplotlib.pyplot as plt

measures={
    "fsols": "Number of final solutions",
    "nfacs": "Instalations in the best solution",
    "vals": "Value of best solution found",
    "times": "Required computation time [s]",
}

problems={
    "splp":"SPLP, $p'$=",
    "pm":"$p$-median, $p$=",
}

transf={
    "pm":"$p$-median",
    "splp":"SPLP"}

STRAT_COLORS={
    "lpsolve":     (0.0, 0.0, 0.0),
    "dsa50":        (0.6, 0.0, 0.0),
    "dsa50hc":      (1.0, 0.0, 0.0),
    "dsa200vr400":  (0.0, 0.6, 0.0),
    "dsa200vr400hc":(0.0, 1.0, 0.0),
    "dsa400vr800":  (0.0, 0.0, 0.6),
    "dsa400vr800hc":(0.0, 0.0, 1.0),
    "randomhc400":  (1.0, 1.0, 0.0),
}

RES_FOLDER = input("Results folder: ")
PLOTSTUFF = int(input("Plots? [0/1] "))
REQUIRED_VALS = int(input("No. experiments? "))

measure_vals = {}

for measure in measures:
    print("MEASURE: %s"%measure)
    values = {}

    strat_keys = {}
    pp_keys = set()
    nn_keys = set()
    # Iterate over the strategies:
    for strat in os.listdir(RES_FOLDER):

        # Read files:
        fname = os.path.join(RES_FOLDER,strat,measure)
        if os.path.isfile(fname):
            fil = open(fname)
            for lin in fil:
                try:
                    label,val = lin.strip().split(" ")
                except:
                    print("Error reading line: \"%s\" on %s"%(lin.strip(),fname))
                v1,v2,v3,v4 = label.split("_")[1:]
                nn = int(v1[1:])
                ii = int(v2[1:])
                pp = int(v3[1:])
                prob = v4
                val = float(val)
                #
                pp_keys.add(pp)
                nn_keys.add(nn)
                if prob not in strat_keys: strat_keys[prob] = set()
                strat_keys[prob].add(strat)
                #
                if prob not in values:
                    values[prob] = {}
                if pp not in values[prob]:
                    values[prob][pp] = {}
                if strat not in values[prob][pp]:
                    values[prob][pp][strat] = {}
                if nn not in values[prob][pp][strat]:
                    values[prob][pp][strat][nn] = {}
                if measure=="vals" and "lpsolve" in strat:
                    # NOTE: I had to add 0.0001 because some lp_solve results may end in .99999999...
                    values[prob][pp][strat][nn][ii] = val+0.0001
                else:
                    values[prob][pp][strat][nn][ii] = val
            fil.close()

    # Tabulate results:
    for k in strat_keys:
        strat_keys[k] = sorted(list(strat_keys[k]))
    pp_keys = sorted(list(pp_keys))
    nn_keys = sorted(list(nn_keys))


    for prob in values:
        lpsolve_name = prob+"_lpsolve"
        for pp in pp_keys:
            for nn in nn_keys:
                for strat in strat_keys[prob]:
                    completes = 0
                    total = 0
                    proportions = {}
                    if lpsolve_name in strat_keys[prob]:
                        total = len(values[prob][pp][lpsolve_name][nn])
                        for ii in values[prob][pp][lpsolve_name][nn].keys():
                            if ii in values[prob][pp][strat][nn]:
                                proportions[ii] = values[prob][pp][strat][nn][ii]/max(1e-3,values[prob][pp][lpsolve_name][nn][ii])
                                if proportions[ii]<1.0: proportions[ii]=1.0
                                if values[prob][pp][strat][nn][ii] <= values[prob][pp][lpsolve_name][nn][ii]:
                                    completes += 1
                                    # assert(values[prob][pp][strat][nn][ii] >= values[prob][pp][lpsolve_name][nn][ii])
                    mean = np.mean(list(values[prob][pp][strat][nn].values()))
                    std = np.std(list(values[prob][pp][strat][nn].values()))
                    prop_mean = np.mean(list(proportions.values()))
                    prop_std = np.std(list(proportions.values()))
                    computed = len(values[prob][pp][strat][nn])
                    # print("%-4s %4d %-18s %3d (%14.6f+-%14.6f) (%8.6f+-%8.6f) (>=:%3d/%3d) (c:%3d)"%(prob,pp,strat,nn,mean,std,prop_mean,prop_std,completes,total,computed))
                    print("%-4s %4d %-18s %3d (%14.6f+-%14.6f) (%8.6f+-%8.6f) (>=:%3d/%3d) (c:%3d)"%(prob,pp,strat,nn,mean,std,prop_mean-1.0,prop_std,completes,total,computed))
    if PLOTSTUFF:
        # Plot means of values:
        for prob in values:
            plot_labels = []
            fig, axs = plt.subplots(nrows=1,ncols=len(values[prob].keys()),sharex=True,sharey=True,figsize=(14,5))
            fig.suptitle("%s"%(measures[measure]))
            i = 0
            for pp in sorted(values[prob].keys()):
                ax = axs[i]
                i += 1
                # plt.figure(figsize=(4,3))
                maxy = -np.inf
                miny = np.inf
                strats = sorted(values[prob][pp].keys())
                lpsolve_name = prob+"_lpsolve"
                lpsolve_vals = {}
                some_plot = False
                strat_k = [x for x in strat_keys[prob] if '5' in x] + [x for x in strat_keys[prob] if '5' not in x]
                if lpsolve_name in strat_k:
                    strat_k.remove(lpsolve_name)
                    strat_k.insert(0,lpsolve_name)
                for strat in strat_k:
                    xs = sorted(list(values[prob][pp][strat].keys()))
                    xs_removed = []
                    ys = []
                    for x in xs:
                        vals = list(values[prob][pp][strat][x].values())
                        if len(vals)!=REQUIRED_VALS:
                            print("Warning: %s %s %s %s %s has %d/%d values."%(measure,prob,pp,strat,x,len(vals),REQUIRED_VALS))
                        if len(vals)<REQUIRED_VALS//2:
                            print("Ignoring!")
                            xs_removed.append(x)
                            continue
                        yval = np.mean(vals)
                        if measure=="vals":
                            if strat==lpsolve_name:
                                lpsolve_vals[x] = yval
                            if x not in lpsolve_vals:
                                xs_removed.append(x)
                                continue
                            yval /= lpsolve_vals[x]
                        ys.append(yval)
                    for x in xs_removed:
                        xs.remove(x)
                    if len(ys)==0: continue
                    stratname = '_'.join(strat.split('_')[1:])
                    # if (strat+"hc") not in strats:
                    if True or (strat+"hc") not in strats:
                        dispname = "dc"+stratname[3:] if stratname[:3]=="dsa" else stratname
                        if dispname not in plot_labels:
                            ax.plot(xs,ys,'o-',label=dispname,color=STRAT_COLORS[stratname])
                            plot_labels.append(dispname)
                        else:
                            ax.plot(xs,ys,'o-',color=STRAT_COLORS[stratname])
                        some_plot = True
                        maxy = np.maximum(maxy,np.max(ys))
                        miny = np.minimum(miny,np.max(ys))
                #
                probname = problems[prob] if prob in problems else prob
                ax.set_title("%s%d"%(probname,pp))
                if some_plot:
                    ax.set_ylim((0 if measure!="vals" else miny,maxy))
            fig.legend(loc='upper center', bbox_to_anchor=(0.5, 0.85), ncol=4, fancybox=True, shadow=True)
            fig.show()

    measure_vals[measure] = values

input("Press key.")

tablehead = """
\\begin{table}
Results for %s with $p=%d$:

\\scriptsize
\\begin{tabular}{ | l r r r r | }
    \\hline & $|\\hat{P}|$ & $|R|$ & Relative cost & Opt. sols."""

for prob in values:
    lpsolve_name = prob+"_lpsolve"

    for pp in pp_keys:
        print(tablehead%(transf[prob],pp))
        for nn in nn_keys:
            print("    \\\\ \\hline $N=%d$ & & & & "%nn)
            strat_k = [x for x in strat_keys[prob] if '5' in x] + [x for x in strat_keys[prob] if '5' not in x]
            first = True
            for strat in strat_k:
                if strat[-2:] != "hc" and strat!=lpsolve_name:
                    strathc = strat+"hc"
                    stratname = "_".join(strat.split("_")[1:])
                    fsols = list(measure_vals["fsols"][prob][pp][strat][nn].values())
                    fsolshc = list(measure_vals["fsols"][prob][pp][strathc][nn].values())

                    optimals = 0
                    radii = []
                    for ii in measure_vals["vals"][prob][pp][strathc][nn]:
                        radii.append(measure_vals["vals"][prob][pp][strathc][nn][ii]/measure_vals["vals"][prob][pp][lpsolve_name][nn][ii])
                        if radii[-1]<1.0: radii[-1] = 1.0
                        optimals += measure_vals["vals"][prob][pp][strathc][nn][ii] <= measure_vals["vals"][prob][pp][lpsolve_name][nn][ii]

                    dispname = "dc"+stratname[3:] if stratname[:3]=="dsa" else stratname
                    sep = "\\hline" if first else ""
                    print("    \\\\ %6s %15s & $%7.2f$ & $%7.2f$ & $%8.6f \\pm %8.6f$ & $%3d$ "%(sep,dispname,np.mean(fsols),np.mean(fsolshc),np.mean(radii),np.std(radii),optimals) )
                    first = False
        #
        print("\\\\ \\hline")
        print("\\end{tabular}")
        print("\\end{table}")
