#!/bin/bash

target="custom"
jobs=8

# Delete solutions without data
files=$(find $target | grep '\.opt')
for fi in $files; do
    len=$(wc -c < "$fi")
    if [ "$len" -eq "0" ]; then
        rm "$fi"
    fi
done

# Add jobs to solve lp problems
for i in $(seq $jobs); do
    qsub -N lpsolv_$i custom_lpsolve.sh || \
        bash custom_lpsolve.sh
    sleep 1
done
