
if (( $# != 3 )); then
    echo "usage: bash generate_problems.sh <p_values> <n_values> <n_probs>"
    exit 1
fi

NPROBS="$3"
NN="$2"
PP="$1"

rm -rf problems || true
mkdir -p problems


NAMEFORMAT="n%04d_i%04d"

# Create problems
for nn in $NN; do
    for prob in $(seq 1 $NPROBS); do
        name=$(printf $NAMEFORMAT $nn $prob)
        templ_name=problems/template_"$name"
        python tools/template_gen.py $nn $nn 10000 "$templ_name"
        for pp in $PP; do
            prob_name=problems/prob_"$name"
            bb=$(python -c "print(int(round( 0.75 * $nn * 2*10000/(3*(3.141592653589793**0.5)) * $pp**(-1.5)  )))")
            ppf=$(printf "%02d" $pp)
            # Create DSA SPLP
            sed -e "s/<<FCOST>>/$bb/g" "$templ_name"_dsa_splp > "$prob_name"_p"$ppf"_dsa_splp
            # Create LP SPLP
            sed -e "s/<<FCOST>>/$bb/g" "$templ_name"_lp_splp > "$prob_name"_p"$ppf"_lp_splp
            # Create DSA p-median
            sed -e "s/<<PP>>/$pp/g" "$templ_name"_dsa_pm > "$prob_name"_p"$ppf"_dsa_pm
            # Create LP p-median
            sed -e "s/<<PP>>/$pp/g" "$templ_name"_lp_pm > "$prob_name"_p"$ppf"_lp_pm
        done
        rm "$templ_name"_*
    done
done
