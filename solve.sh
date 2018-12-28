#!/bin/bash
#PBS -l cput=8000:00:01
#PBS -l walltime=8000:00:01
#PBS mem=20gb

if [ -n "${PBS_O_WORKDIR+1}" ]; then
    cd "$PBS_O_WORKDIR"
fi

pz="$(echo $1 | cut -d'_' -f1 -)"
vr="$(echo $1 | cut -d'_' -f2 -)"
if [ "$vr" -eq "0" ]; then
   vr=99999999
fi
group="$2"
files="$3"

mkdir -p "res/$group"
for prob in $files; do
    ./bin/dsa_ls "$pz" "$vr" 10 "$prob" "res/$prob" "res/""$prob""_ls"
done
