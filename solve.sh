#!/bin/bash
#PBS -l cput=8000:00:01
#PBS -l walltime=8000:00:01
#PBS -l mem=10gb

if [ -n "${PBS_O_WORKDIR+1}" ]; then
    cd "$PBS_O_WORKDIR"
fi

mode="$(echo $1 | cut -d'_' -f1 -)"
pz="$(echo $1 | cut -d'_' -f2 -)"
vr="$(echo $1 | cut -d'_' -f3 -)"
if [ "$vr" -eq "0" ]; then
   vr=99999999
fi
group="$2"
files="$3"

mkdir -p "res/$1/$group"
for prob in $files; do
    if [ "$mode" == "h" ]; then
        ./bin/dsa_hausdorff_ls "$pz" "$vr" 10 "$prob" "res/$1/$prob" "res/$1/""$prob""_ls"
    fi
    if [ "$mode" == "n" ]; then
        ./bin/dsa_ls "$pz" "$vr" 10 "$prob" "res/$1/$prob" "res/$1/""$prob""_ls"
    fi
done
