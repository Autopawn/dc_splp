import os
import sys
import numpy as np

# -- parse input --
if len(sys.argv)!=3:
    print("usage: python %s <prob_dir> <sols_dir>"%(sys.argv[0]))
    print("e.g.:  python %s %s %s"%(sys.argv[0],'splp','res/dc_norm_s_200_400/splp'))
    sys.exit(1)

prob_dir = sys.argv[1]
sols_dir = sys.argv[2]

# -- --

def get_dirs(dir,ext=None):
    dirs = []
    for subdir in os.listdir(dir):
        subdir_path = os.path.join(dir,subdir)
        if os.path.isdir(subdir_path):
            contents = get_dirs(subdir_path,ext)
            dirs += [[subdir]+x for x in contents]
        else:
            if ext is None or (len(subdir)>=len(ext) and subdir[-len(ext):]==ext):
                dirs += [[subdir]]
    return dirs

def read_optimum(fname):
    fi = open(fname)
    time = None
    for lin in fi:
        lin = lin.strip()
        if len(lin)==0: continue
        if "#" in lin:
            if "#runtime" in lin:
                time = int(lin.replace("#runtime",""))
        else:
            k = lin.split()
    fi.close()
    assigns = [int(x) for x in k[:-1]]
    value = float(k[-1])
    return assigns,value,time

def read_problem(fname):
    fi = open(fname)
    mode = 0
    for lin in fi:
        lin=lin.strip()
        if "FILE" in lin:
            mode = 1
            continue
        if mode==0:
            n,m = [int(x) for x in lin.split(' ')]
            p = 0
        else:
            n,m,p = [int(x) for x in lin.split(' ')]
        break
    fi.close()
    return n,m,p

def read_solution(fname):
    if not os.path.isfile(fname):
        return None,None
    # ---
    fi = open(fname)
    assigns = None
    value = None
    for lin in fi:
        if (assigns is None) and ("Assigns:" in lin):
            assigns = [int(x) for x in lin.split()[1:]]
        if (value is None) and ("Value:" in lin):
            value = -float(lin.split()[1])
        if (assigns is not None) and (value is not None):
            break
    fi.close()
    assert(assigns is not None)
    assert(value is not None)
    #
    return assigns,value

def is_optimum(sol,opt):
    opt_assi,opt_val,_ = opt
    sol_assi,sol_val = sol
    if sol_assi is None:
        assert(sol_val is None)
        return False
    sol_facts = set(sol_assi)
    opt_facts = set(opt_assi)
    if sol_facts == opt_facts: return True
    if sol_val <= opt_val: return True
    return False

problems = {}
problems['opt'] = get_dirs(prob_dir,".opt")
problems['bub'] = get_dirs(prob_dir,".bub")

summary = {}
for kind in ('opt','bub'):
    summary[kind] = []
    print("="*80)
    print("> PROBLEMS: "+kind.upper())
    print("="*80)
    prob_names = sorted(list(problems[kind]))
    # Identify the problem groups (last folders):
    group_names = sorted(list(set([tuple(x[:-1]) for x in prob_names])))

    for group in group_names:

        # Find the specific problems for this group:
        group_prob_names = [x for x in prob_names if tuple(x[:-1])==group]

        strings = []
        optis = 0
        nones = 0
        perces = []
        nfacs = []
        nclis = []
        opt_nfacs = []
        ps = []
        opt_times = []

        for prob in group_prob_names:
            joined = os.path.join(*prob)
            fname = os.path.join(prob_dir,joined)
            prob_fname = fname.replace('.'+kind,'')
            if not os.path.isfile(prob_fname):
                print("ERROR: %s does not exists."%prob_fname)
                sys.exit(1)
            n,m,p = read_problem(prob_fname)
            nfacs.append(n)
            nclis.append(m)
            ps.append(p)
            opt_data = read_optimum(fname)
            # --- Maximums
            n_opt_facilities = len(set(opt_data[0]))
            n_clients = len(opt_data[0])
            opt_nfacs.append(n_opt_facilities)
            # --- Get the solution
            sol_fname = os.path.join(sols_dir,joined)
            sol_fname = sol_fname[:-4]+"_ls"
            sol_data = read_solution(sol_fname)
            # --- Check for optimality
            perce = 0
            show = False
            if sol_data[0] is None:
                nones += 1
            else:
                if opt_data[2] is not None:
                    opt_times.append(opt_data[2])
                opt = is_optimum(sol_data,opt_data)
                if opt:
                    optis += 1
                else:
                    show = True
                perce = 0 if opt_data[1] is None else sol_data[1]/opt_data[1]
            if show==1:
                strings.append("%-30s %5d %5d %12.3f %12.3f %8.4f"%(
                    joined,n_clients,n_opt_facilities,sol_data[1] or 0,opt_data[1],perce))
                perces.append(perce)
        # Print problem and opt solutions description
        group_name = '/'.join(group)
        print("-"*20)
        min_p = np.min(ps)
        max_p = np.max(ps)
        min_nfacs = np.min(nfacs)
        max_nfacs = np.max(nfacs)
        min_nclis = np.min(nclis)
        max_nclis = np.max(nclis)
        min_opt_nfacs = np.min(opt_nfacs)
        max_opt_nfacs = np.max(opt_nfacs)
        opt_time_mean = float('inf') if len(opt_times)==0 else np.mean(opt_times)
        opt_time_std = float('inf') if len(opt_times)==0 else np.std(opt_times)
        print("%-30s  (%d,probs)  n:%d-%d  m:%d-%d  p:%d-%d  on:%d-%d  otime:%f+-%f"%(group_name,
            len(group_prob_names),
            min_nfacs,max_nfacs,
            min_nclis,max_nclis,
            min_p,max_p,
            min_opt_nfacs,max_opt_nfacs,
            opt_time_mean,opt_time_std))
        # Print solutions description
        if nones==0:
            red = ''
            noc = ''
        else:
            red = '\033[0;31m'
            noc = '\033[0m'
        perce = "  --  " if len(perces)==0 else "%9.6f"%(np.mean(perces))
        print("%-30s opt:%3d/%-3d  %snons:%3d/%-3d%s perce:%s"%("",
            optis,len(group_prob_names),
            red,nones,len(group_prob_names),noc,
            perce))
        for stri in strings:
            print(stri)
