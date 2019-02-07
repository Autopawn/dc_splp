#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "usage: $0 <target_folder> <res_folder>"
    echo "e.g.:  $0 \"splp\" \"res\""
    echo "e.g.:  $0 \"custom\" \"custom_res\""
    echo "also, remember to 'make'!"
    exit 1
fi

target=$1
resfolder=$2

fnames=$(find $target | grep -v 'kmedian' | \
    grep -v '\.opt' | grep -v '\.bub' | grep -v 'README' | \
    grep -v '\.c' | grep -v '\.lst' | grep -v '~' | \
    grep -v 'capinfo\.txt' | grep -v 'capmst1\.txt' | grep -v 'capmst2\.txt' | \
    grep -v 'ORLIB-uncap/a-c' )
fnames="$fnames $target"

parameters="\
    dc_norm_m_200_400 dc_norm_s_200_400 \
    dc_haus_m_200_400 dc_haus_s_200_400 \
    dc_norm_m_200_-1 dc_norm_s_200_-1 \
    dc_best_m_200_400 dc_best_s_200_400 "

# Delelte problem_list files
mkdir -p "$resfolder"
for probfile in $(find "$resfolder" | grep problem_list); do
    rm $probfile
done

# make
# rm -rf res || true

for params in $parameters; do
    problems=""
    for p in $fnames; do
        if [[ -d $p ]]; then
            if [ -n "$problems" ]; then
                gname="$(echo $group | tr / _ | cut -d'_' -f2-)"
                n2=$(echo $params | cut -d'_' -f2)
                n3=$(echo $params | cut -d'_' -f3)
                ng=$(echo $params | cut -d'_' -f4-5)
                name=dc_"${n2:0:1}${n3:0:1}"_"$ng"_"$gname"
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
