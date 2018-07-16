#!/bin/bash -e
#PBS -l cput=8000:00:01
#PBS -l walltime=8000:00:01
#PBS mem=20gb

# usage: bash _solve_lpsolve.sh <jobid> <jobs> <p> (pm|splp)

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

prob="$4"
ext=lp_"$prob"
folder=results/"$prob"_lpsolve

mkdir -p "$folder"/nfacs_p
mkdir -p "$folder"/times_p
mkdir -p "$folder"/vals_p

todo_array=($(find problems/prob_*_p"$ppf"_"$ext" -type f)) # places output into an array

function do_job {
    fname="$1"

    # Filenames
    bname=$(basename "$fname")
    bbname=${bname%_$ext}_"$prob"
    solname="$folder"/"$bname"_sol
    timename="$folder"/"$bname"_time

    # Check if already exists:
    grep -q "$bbname" "$folder"/nfacs
    in_nfacs=$?
    grep -q "$bbname" "$folder"/times
    in_times=$?
    grep -q "$bbname" "$folder"/vals
    in_vals=$?

    if [[ $in_nfacs -eq 0 && $in_times -eq 0 && $in_vals -eq 0 ]]; then
        echo "SKIPPING: $folder $bbname already solved."
    else
        echo "SOLVING : $folder $bbname not found."

        # Clear other files:
        rm "$folder"/nfacs_p/"$bbname" || true
        rm "$folder"/times_p/"$bbname" || true
        rm "$folder"/vals_p/"$bbname"  || true

        sed -e s/$bbname//g -i "$folder"/nfacs
        sed -e s/$bbname//g -i "$folder"/times
        sed -e s/$bbname//g -i "$folder"/vals

        # Solve:
        { time -p $lp_solve "$fname" > "$solname"; } 2> "$timename"

        # Get number of facilities
        cat "$solname" | grep "X" | grep " 1" | wc -l | \
        sed -e "s/^/$bbname /" >> "$folder"/nfacs_p/"$bbname"

        # Get time
        cat "$timename" | grep "user" | awk '{print $NF}' | \
        sed -e "s/^/$bbname /" >> "$folder"/times_p/"$bbname"

        # Get value
        cat "$solname" | grep "objective function:" | awk '{print $NF}' | cut -d'.' -f1 | \
        sed -e "s/^/$bbname /" >> "$folder"/vals_p/"$bbname"

        # Delete solution:
        rm "$solname" "$timename"

    fi

}

index=0
for jobname in $todo_array; do
    if [ $((index % MAX_JOBS)) -eq $CURRENT_JOB ]; then
        do_job "$jobname"
    fi
    index=$((index+1))
done
