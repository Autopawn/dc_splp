#!/bin/bash

target="custom"
jobs=6

# Delete solutions without data
files=$(find $target | grep '\.opt') || true
for fil in $files; do
    len=$(wc -c < "$fil")
    if [ "$len" -lt "3" ]; then
        rm "$fil"
    fi
done

# Add jobs to solve lp problems
for i in $(seq $jobs); do
    echo $i
    qsub -N lpsolv_$i custom_lpsolve.sh -F "$i $jobs" || \
        bash custom_lpsolve.sh $i $jobs
    sleep 1
done
