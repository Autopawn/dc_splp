#!/bin/bash -e
#PBS -l cput=8000:00:01
#PBS -l walltime=8000:00:01
#PBS mem=30gb

# usage: bash _solve_randomhc.sh <p> <jobs> (pm|splp) <MULT> <MAXSIZE>

#parallelization using: https://stackoverflow.com/a/6594537/4386784

ppf=$(printf "%02d" $1)

MAX_JOBS="$2"

index=0

# Tunning for the HPC cluster:
if [ -n "${PBS_O_WORKDIR+1}" ]; then
    cd "$PBS_O_WORKDIR"
    export lp_solve="$HOME/lp_solve_5.5/lp_solve/bin/ux64/lp_solve"
else
    export lp_solve="lp_solve"
fi

if (( $# != 5 )); then
    echo "Wrong number of arguments!"
    exit 1
fi

randomhcname=randomhc"$4"x"$5"
mult="$4"
maxsize="$5"

prob="$3"
ext=dsa_"$prob"

folder=results/"$prob"_"$randomhcname"

mkdir -p "$folder"

todo_array=($(find problems/prob_*_p"$ppf"_"$ext" -type f)) # places output into an array

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

    # Solve
    ./bin/randomhc "$mult" "$maxsize" 1 "$fname" "$solname"

    # Get number of facilities
    cat "$solname" | grep "Facilities:" | awk '{print $NF}' | \
        sed -e "s/^/$bbname /" >> "$folder"/nfacs

    # Get value of objective function
    cat "$solname" | grep "Value:" | awk '{print $NF}' | sed -e "s/-//" | \
        sed -e "s/^/$bbname /" >> "$folder"/vals

    # Get time
    cat "$solname" | grep "Time:" | awk '{print $NF}' | \
        sed -e "s/^/$bbname /" >> "$folder"/times

    # Get final solutions
    cat "$solname" | grep "Final_solutions:" | awk '{print $NF}' | \
        sed -e "s/^/$bbname /" >> "$folder"/fsols

    # Delete solution:
    rm "$solname" "$hcsolname"
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
