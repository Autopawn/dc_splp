#!/bin/bash

quser=fcasas
maxjobs=200

if [ "$#" -ne 3 ]; then
    echo "usage: $0 <target_folder> <res_folder> <parameters>"
    echo "e.g.:  $0 \"splp\" \"res\" \"dc_dismsemin_0_50_100 dc_dismsesum_0_50_100\""
    echo "e.g.:  $0 \"pmedian\" \"res\" \"dc_dismsemin_0_50_100 dc_dismsesum_0_50_100\""
    echo "also, remember to 'make'!"
    exit 1
fi

target=$1
resfolder=$2
parameters=$3

fnames=$(find $target | \
    grep -v '\.opt' | grep -v '\.bub' | grep -v 'README' | \
    grep -v '\.c' | grep -v '\.lst' | grep -v '~' | \
    grep -v 'capinfo\.txt' | grep -v 'capmst1\.txt' | grep -v 'capmst2\.txt' | \
    grep -v '\.lp' \
    )
fnames="$fnames $target"

for params in $parameters; do
    problems=""
    for p in $fnames; do
        if [[ -d $p ]]; then
            if [ -n "$problems" ]; then
                gname="$(echo $group | tr / _ | cut -d'_' -f2-)"
                n2=$(echo $params | cut -d'_' -f2)
                n3=$(echo $params | cut -d'_' -f3)
                ng=$(echo $params | cut -d'_' -f4)
                name="$n2"_"$n3"_"$ng"_"$gname"
                while [ "$(qselect -u $quser | wc -l)" -gt "$maxjobs" ]; do
                    sleep 5
                done
                echo "calling $name"
                qsub -N $name splp_solve.sh \
                    -F "$params \"$group\" \"$problems\" \"$resfolder\"" || \
                bash splp_solve.sh $params "$group" "$problems" "$resfolder"
            fi
            problems=""
            group="$p"
        elif [[ -f $p ]]; then
            problems="$p $problems"
        else
            echo "$p is not valid"
            exit 1
        fi
    done
done
