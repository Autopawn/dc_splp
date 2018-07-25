import os

import numpy as np

import matplotlib.pyplot as plt

measures={
    "fsols": "Number of final solutions",
    "nfacs": "Instalations in the best solution",
    "vals": "Value of best solution found",
    "times": "Required computation time",
}

problems={
    "splp":"SPLP, $p'$=",
    "pm":"$p$-median, $p$=",
}

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

RES_FOLDER = raw_input("Results folder: ")
PLOTSTUFF = int(raw_input("Plots? [0/1] "))
REQUIRED_VALS = 100

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
                                proportions[ii] = values[prob][pp][strat][nn][ii]/values[prob][pp][lpsolve_name][nn][ii]
                                if values[prob][pp][strat][nn][ii]-0.24 <= values[prob][pp][lpsolve_name][nn][ii]:
                                    completes += 1
                                    # assert(values[prob][pp][strat][nn][ii] >= values[prob][pp][lpsolve_name][nn][ii])
                    mean = np.mean(values[prob][pp][strat][nn].values())
                    std = np.std(values[prob][pp][strat][nn].values())
                    prop_mean = np.mean(proportions.values())
                    prop_std = np.std(proportions.values())
                    computed = len(values[prob][pp][strat][nn])
                    # print("%-4s %4d %-18s %3d (%14.6f+-%14.6f) (%8.6f+-%8.6f) (>=:%3d/%3d) (c:%3d)"%(prob,pp,strat,nn,mean,std,prop_mean,prop_std,completes,total,computed))
                    print("%-4s %4d %-18s %3d (%14.6f+-%14.6f) (%8.6f+-%8.6f) (>=:%3d/%3d) (c:%3d)"%(prob,pp,strat,nn,mean,std,prop_mean-1.0,prop_std,completes,total,computed))

    if PLOTSTUFF:
        # Plot means of values:
        for prob in values:
            for pp in values[prob]:
                plt.figure(figsize=(4,3))
                maxy = -np.inf
                miny = np.inf
                strats = sorted(values[prob][pp].keys())
                lpsolve_name = prob+"_lpsolve"
                lpsolve_vals = {}
                some_plot = False
                if lpsolve_name in strats:
                    strats.remove(lpsolve_name)
                    strats.insert(0,lpsolve_name)
                for strat in strats:
                    xs = sorted(list(values[prob][pp][strat].keys()))
                    xs_removed = []
                    ys = []
                    for x in xs:
                        vals = values[prob][pp][strat][x].values()
                        if len(vals)!=REQUIRED_VALS:
                            print("Warning: %s %s %s %s %s has %d/%d values."%(measure,prob,pp,strat,x,len(vals),REQUIRED_VALS))
                        if len(vals)<REQUIRED_VALS/2:
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
                    if (strat+"hc") not in strats:
                        plt.plot(xs,ys,'o-',label=stratname,color=STRAT_COLORS[stratname])
                        some_plot = True
                        maxy = np.maximum(maxy,np.max(ys))
                        miny = np.minimum(miny,np.max(ys))
                #
                probname = problems[prob] if prob in problems else prob
                plt.title("%s for %s%d"%(measures[measure],probname,pp))
                if some_plot:
                    plt.ylim((0 if measure!="vals" else miny,maxy))
                plt.legend()
                plt.show()

    measure_vals[measure] = values

tablehead = """
\\begin{table}
Results for %s with $p=%d$:

\\scriptsize
\\begin{tabular}{ | l r r r r | }
    \\hline & $|\\hat{P}|$ & $|R|$ & Relative cost & Opt. sols."""

transf={"pm":"$p$-median","splp":"SPLP"}

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
                    fsols = measure_vals["fsols"][prob][pp][strat][nn].values()
                    fsolshc = measure_vals["fsols"][prob][pp][strathc][nn].values()

                    optimals = 0
                    radii = []
                    for ii in measure_vals["vals"][prob][pp][strathc][nn]:
                        radii.append(measure_vals["vals"][prob][pp][strathc][nn][ii]/measure_vals["vals"][prob][pp][lpsolve_name][nn][ii])
                        optimals += measure_vals["vals"][prob][pp][strathc][nn][ii] == measure_vals["vals"][prob][pp][lpsolve_name][nn][ii]

                    sep = "\\hline" if first else ""
                    print("    \\\\ %6s %15s & $%7.2f$ & $%7.2f$ & $%8.6f \\pm %8.6f$ & $%3d$ "%(sep,stratname,np.mean(fsols),np.mean(fsolshc),np.mean(radii),np.std(radii),optimals) )
                    first = False
        #
        print("\\\\ \\hline")
        print("\\end{tabular}")
        print("\\end{table}")
