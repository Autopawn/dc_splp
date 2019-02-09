#!/bin/bash -xe

rm -rf "pmedian" || true

mkdir -p "pmedian"

gcc tools/pmed_translator.c -o bin/pmed_translator

cd pmedian

# Get optima
wget http://people.brunel.ac.uk/~mastjjb/jeb/orlib/files/pmedopt.txt

for i in $(seq 40); do
    target=pmed"$i".txt
    wget people.brunel.ac.uk/~mastjjb/jeb/orlib/files/pmed"$i".txt
    mv pmed"$i".txt pmed"$i"_original.txt
    ../bin/pmed_translator pmed"$i"_original.txt "$target"
    rm pmed"$i"_original.txt
    #
    nn=$(cat "$target" | sed -n 2p | cut -d' ' -f1)
    val=$(cat pmedopt.txt | grep pmed"$i"' ' | rev | cut -d' ' -f1 | rev)
    touch "$target".opt
    for i in $(seq $nn); do
        echo -n " -1" >> "$target".opt
    done
    echo " "$val >> "$target".opt
done

rm pmedopt.txt
