#!/bin/bash
#PBS -l cput=8000:00:01
#PBS -l walltime=8000:00:01
#PBS mem=20gb

# usage: bash _solve_dsa.sh <jobid> <jobs> <p> (pm|splp) <PZ> [VR]

#parallelization using: https://stackoverflow.com/a/6594537/4386784

ppf=$(printf "%02d" $3)

CURRENT_JOB="$1"
MAX_JOBS="$2"

# Tunning for the HPC cluster:
if [ -n "${PBS_O_WORKDIR+1}" ]; then
    cd "$PBS_O_WORKDIR"
    export lp_solve="$HOME/lp_solve_5.5/lp_solve/bin/ux64/lp_solve"
else
    export lp_solve="lp_solve"
fi

if (( $# == 5 )); then
    dsaname=dsa"$5"
    vr=99999999
fi
if (( $# == 6 )); then
    dsaname=dsa"$5"vr"$6"
    vr="$6"
fi

pz="$5"

prob="$4"
ext=dsa_"$prob"

folder=results/"$prob"_"$dsaname"
hcfolder=results/"$prob"_"$dsaname"hc

mkdir -p "$folder"/nfacs_p
mkdir -p "$folder"/times_p
mkdir -p "$folder"/vals_p
mkdir -p "$folder"/fsols_p
mkdir -p "$hcfolder"/nfacs_p
mkdir -p "$hcfolder"/times_p
mkdir -p "$hcfolder"/vals_p
mkdir -p "$hcfolder"/fsols_p

todo_array=$(find problems/prob_*_p"$ppf"_"$ext" -type f)

function do_job {
    fname="$1"

    # Filenames
    bname=$(basename "$fname")
    bbname=${bname%_$ext}_"$prob"
    solname="$folder"/"$bname"_sol
    hcsolname="$hcfolder"/"$bname"_sol

    # Check if already exists:
    grep -q "$bbname" "$folder"/nfacs
    in_nfacs=$?
    grep -q "$bbname" "$folder"/times
    in_times=$?
    grep -q "$bbname" "$folder"/vals
    in_vals=$?
    grep -q "$bbname" "$folder"/fsols
    in_fsols=$?

    grep -q "$bbname" "$hcfolder"/nfacs
    in_nfacs_hc=$?
    grep -q "$bbname" "$hcfolder"/times
    in_times_hc=$?
    grep -q "$bbname" "$hcfolder"/vals
    in_vals_hc=$?
    grep -q "$bbname" "$hcfolder"/fsols
    in_fsols_hc=$?

    if [[ $in_nfacs -eq 0 && $in_times -eq 0 && $in_vals -eq 0 && $in_fsols -eq 0 && $in_nfacs_hc -eq 0 && $in_times_hc -eq 0 && $in_vals_hc -eq 0 && $in_fsols_hc -eq 0 ]]; then
        echo "SKIPPING: $folder $bbname already solved."
    else
        echo "SOLVING : $folder $bbname not found."

        # Clear other files:
        rm "$folder"/nfacs_p/"$bbname" || true
        rm "$folder"/times_p/"$bbname" || true
        rm "$folder"/vals_p/"$bbname"  || true
        rm "$folder"/fsols_p/"$bbname" || true
        rm "$hcfolder"/nfacs_p/"$bbname" || true
        rm "$hcfolder"/times_p/"$bbname" || true
        rm "$hcfolder"/vals_p/"$bbname"  || true
        rm "$hcfolder"/fsols_p/"$bbname" || true

        # Solve
        ./bin/dsa_ls "$pz" "$vr" 1 "$fname" "$solname" "$hcsolname"

        # Get number of facilities
        cat "$solname" | grep "Facilities:" | awk '{print $NF}' | \
            sed -e "s/^/$bbname /" > "$folder"/nfacs_p/"$bbname"

        # Get value of objective function
        cat "$solname" | grep "Value:" | awk '{print $NF}' | sed -e "s/-//" | \
            sed -e "s/^/$bbname /" > "$folder"/vals_p/"$bbname"

        # Get time
        cat "$solname" | grep "Time:" | awk '{print $NF}' | \
            sed -e "s/^/$bbname /" > "$folder"/times_p/"$bbname"

        # Get final solutions
        cat "$solname" | grep "Final_solutions:" | awk '{print $NF}' | \
            sed -e "s/^/$bbname /" > "$folder"/fsols_p/"$bbname"

        # Get number of facilities with HC
        cat "$hcsolname" | grep "Facilities:" | awk '{print $NF}' | \
            sed -e "s/^/$bbname /" > "$hcfolder"/nfacs_p/"$bbname"

        # Get value of objective function with HC
        cat "$hcsolname" | grep "Value:" | awk '{print $NF}' | sed -e "s/-//" | \
            sed -e "s/^/$bbname /" > "$hcfolder"/vals_p/"$bbname"

        # Get time with HC
        cat "$hcsolname" | grep "Time:" | awk '{print $NF}' | \
            sed -e "s/^/$bbname /" > "$hcfolder"/times_p/"$bbname"

        # Get final solutions with HC
        cat "$hcsolname" | grep "Final_solutions:" | awk '{print $NF}' | \
            sed -e "s/^/$bbname /" > "$hcfolder"/fsols_p/"$bbname"

        # Delete solution:
        rm "$solname" "$hcsolname"

    fi
}

index=0
for jobname in $todo_array; do
    if [ $((index % MAX_JOBS)) -eq $CURRENT_JOB ]; then
        do_job "$jobname"
    fi
    index=$((index+1))
done
