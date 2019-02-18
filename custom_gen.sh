#

target="custom"

rm -rf "$target" || true

mkdir -p "$target"

nprobs=100
nvalues="100 200 300 400 500"
nvalues2="100 200 300 400 500 600 700 800 900 1000"
pvalues="4 8 12 16"
bvalues="0.0340 0.0094 0.0048 0.0032"

for i in $(seq $(echo "$pvalues" | wc -w)); do
    pp=$(echo "$pvalues" | cut -d' ' -f"$i")
    bb=$(echo "$bvalues" | cut -d' ' -f"$i")
    for nn in $nvalues; do
        subtarg="$target"/splp_"$pp"_"$nn"
        mkdir -p "$subtarg"
        for kk in $(seq $nprobs); do
            cost=$(python -c "print("$bb"*"$nn")")
            python tools/problem_generator.py "$nn" "$cost" "$subtarg"/"$kk" "$subtarg"/"$kk".lp
        done
    done
done

for pp in $pvalues; do
    for nn in $nvalues; do
        subtarg="$target"/pmedian_"$pp"_"$nn"
        mkdir -p "$subtarg"
        for kk in $(seq $nprobs); do
            python tools/problem_generator.py "$nn" p_"$pp" "$subtarg"/"$kk" "$subtarg"/"$kk".lp
        done
    done
done

# won't be generated.
# for nn in $nvalues2; do
#     subtarg="$target"/splpl_"$nn"
#     mkdir -p "$subtarg"
#     for kk in $(seq $nprobs); do
#         python tools/problem_generator.py "$nn" l "$subtarg"/"$kk" "$subtarg"/"$kk".lp
#     done
# done
