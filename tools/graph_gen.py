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
REQUIRED_VALS = 100

for measure in measures:
    print("MEASURE: %s"%measure)
    values = {}
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
                        xs_removed.append(x)
                        print("Warning: %s %s %s %s %s has %d/%d values."%(measure,prob,pp,strat,x,len(vals),REQUIRED_VALS))
                    else:
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
