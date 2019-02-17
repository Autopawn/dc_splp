#!/bin/bash -e
#PBS -l cput=8000:00:01
#PBS -l walltime=8000:00:01
#PBS -l mem=30gb

if [ "$#" -ne 4 ]; then
    echo "4 arguments are required!"
    exit 1
fi

if [ -n "${PBS_O_WORKDIR+1}" ]; then
    cd "$PBS_O_WORKDIR"
fi

exec="$(echo $1 | cut -d'_' -f1-3 -)"
pz="$(echo $1 | cut -d'_' -f4 -)"
vr="$(echo $1 | cut -d'_' -f5 -)"
if [ "$vr" -eq "0" ]; then
   vr=99999999
fi
group="$2"
files="$3"

resfolder=$4

mkdir -p "$resfolder/$1/$group"
echo group "$group" > "$resfolder/$1/$group/problem_list"
echo '$1' "$1" >> "$resfolder/$1/$group/problem_list"
echo "$files" | tr " " "\n" >> "$resfolder/$1/$group/problem_list"
for prob in $files; do
    if [ ! -f "$resfolder/$1/""$prob""_ls" ]; then
        ./bin/"$exec" "$pz" "$vr" 10 "$prob" "$resfolder/$1/$prob" "$resfolder/$1/""$prob""_ls"
    fi
done
