#

target="splp_custom"

rm -rf "$target" || true

mkdir -p "$target"

nprobs=4
nvalues="100 200 300 400 500"
pvalues="4 8 12 16"

for pp in $pvalues; do
    for nn in $nvalues; do
        subtarg="$target"/"$pp"_"$nn"
        mkdir -p "$subtarg"
        for kk in $(seq $nprobs); do
            cost=$(python -c "from numpy import pi; print("$pp"**-2.5*pi**-0.5*"$nn")")
            python tools/problem_generator.py "$nn" "$cost" "$subtarg"/"$kk" "$subtarg"/"$kk".lp
        done
    done
done
