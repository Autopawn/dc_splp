#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "usage: $0 <target_folder> <res_folder>"
    echo "e.g.:  $0 \"splp\" \"res\""
    echo "e.g.:  $0 \"custom\" \"res\""
    echo "also, remember to 'make'!"
    exit 1
fi

target=$1
resfolder=$2

fnames=$(find $target | \
    grep -v '\.opt' | grep -v '\.bub' | grep -v 'README' | \
    grep -v '\.c' | grep -v '\.lst' | grep -v '~' | \
    grep -v 'capinfo\.txt' | grep -v 'capmst1\.txt' | grep -v 'capmst2\.txt' | \
    grep -v '\.lp' \
    )
fnames="$fnames $target"

# NOTE: for splp
parameters="\
    dc_dismsemin_200_400 dc_dismsesum_200_400 \
    dc_dishaumin_200_400 dc_dishausum_200_400 \
    dc_discli_200_400 \
    dc_bes_200_0 \
    dc_ran_200_0 \
    dc_dismsemin_400_600 dc_dismsesum_400_600 \
    dc_dishaumin_400_600 dc_dishausum_400_600 \
    dc_discli_400_600 \
    dc_bes_400_0 \
    dc_ran_400_0 \
    dc_bes_1000_0 \
    dc_ran_1000_0 \
    "

# NOTE: for custom
parameters="\
    dc_dismsemin_200_400 \
    dc_bes_200_0 \
    dc_ran_200_0 \
    dc_dismsemin_400_600 \
    dc_bes_400_0 \
    dc_ran_400_0 \
    dc_bes_1000_0 \
    dc_ran_1000_0 \
    "

# NOTE: for pmedian_large
parameters="\
    dc_dismsemin_50_100 dc_dismsesum_50_100 \
    dc_dishaumin_50_100 dc_dishausum_50_100 \
    dc_discli_50_100 \
    dc_bes_200_0 \
    dc_ran_200_0 \
    dc_bes_400_0 \
    dc_ran_400_0 \
    dc_bes_1000_0 \
    dc_ran_1000_0 \
    "

# # NOTE: for splp_kmedian
# parameters="\
#     dc_dismsemin_50_100 dc_dismsesum_50_100 \
#     dc_dishaumin_50_100 dc_dishausum_50_100 \
#     dc_discli_50_100 \
#     dc_bes_50_0 \
#     dc_ran_50_0 \
#     "

# Delete problem_list files
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
                ng=$(echo $params | cut -d'_' -f3)
                name="$n2"_"$ng"_"$gname"
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
