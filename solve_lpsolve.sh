#!/bin/bash -e
#PBS -l cput=8000:00:01
#PBS -l walltime=8000:00:01
#PBS mem=30gb

# usage: bash solve_lpsolve.sh (pm|splp)

#parallelization using: https://stackoverflow.com/a/6594537/4386784

MAX_JOBS=5

index=0

# Tunning for the HPC cluster:
if [ -n "${PBS_O_WORKDIR+1}" ]; then
    cd "$PBS_O_WORKDIR"
    rm result.err result.out || true
    export lp_solve="$HOME/lp_solve_5.5/lp_solve/bin/ux64/lp_solve"
else
    export lp_solve="lp_solve"
fi

prob="$1"
ext=lp_"$prob"
folder=results/"$prob"_lpsolve

rm -rf "$folder" || true
mkdir -p "$folder"

todo_array=($(find problems/prob_*_"$ext" -type f)) # places output into an array

function add_next_job {
    # if still jobs to do then add one
    if [[ $index -lt ${#todo_array[*]} ]]
    then
        echo Adding job: ${todo_array[$index]}
        do_job ${todo_array[$index]} &
        index=$(($index+1))
    fi
}

function do_job {
    fname="$1"

    # Filenames
    bname=$(basename "$fname")
    bbname=${bname%_$ext}_"$prob"
    solname="$folder"/"$bname"_sol
    timename="$folder"/"$bname"_time

    # Solve
    { time -p $lp_solve "$fname" > "$solname"; } 2> "$timename"

    # Get number of facilities
    cat "$solname" | grep "X" | grep " 1" | wc -l | \
        sed -e "s/^/$bbname /" >> "$folder"/nfacs

    # Get time
    cat "$timename" | grep "user" | awk '{print $NF}' | \
        sed -e "s/^/$bbname /" >> "$folder"/times

    # # Get value
    cat "$solname" | grep "objective function:" | awk '{print $NF}' | cut -d'.' -f1 | \
        sed -e "s/^/$bbname /" >> "$folder"/vals

    # Delete solution:
    rm "$solname" "$timename"
}

set -o monitor
# run background processes in a separate processes
trap add_next_job CHLD
# execute add_next_job when we receive a child complete signal

# add initial set of jobs
while [[ $index -lt $MAX_JOBS ]]
do
    add_next_job
done

# wait for all jobs to complete
wait
echo "done!"
