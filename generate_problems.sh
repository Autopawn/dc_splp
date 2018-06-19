
if (( $# != 1 )); then
    echo "usage: bash generate_problems.sh <p_values>"
    exit 1
fi

PP="$1"

rm -rf problems || true
mkdir -p problems

NAMEFORMAT="n%04d_i%04d"

# Create problems
for templ_name in templates/template_*_dsa_splp; do
    templ_bname=$(basename "$templ_name")
    name=$(python -c "print('_'.join(\"$templ_bname\".split('_')[1:3]))")
    nn=$(python -c "print(\"$templ_bname\".split('_')[2][1:])")
    for pp in $PP; do
        prob_name=problems/prob_"$name"
        bb=$(python -c "print(int(round( int(\"$nn\") * 2*10000/(3*(3.141592653589793**0.5)) * $pp**(-1.5)  )))")
        ppf=$(printf "%02d" $pp)
        # Create DSA SPLP
        sed -e "s/<<FCOST>>/$bb/g" "$templ_name" > "$prob_name"_p"$ppf"_dsa_splp
    done
done
for templ_name in templates/template_*_dsa_pm; do
    templ_bname=$(basename "$templ_name")
    name=$(python -c "print('_'.join(\"$templ_bname\".split('_')[1:3]))")
    for pp in $PP; do
        prob_name=problems/prob_"$name"
        ppf=$(printf "%02d" $pp)
        # Create DSA p-median
        sed -e "s/<<PP>>/$pp/g" "$templ_name" > "$prob_name"_p"$ppf"_dsa_pm
    done
done
for templ_name in templates/template_*_lp_splp; do
    templ_bname=$(basename "$templ_name")
    name=$(python -c "print('_'.join(\"$templ_bname\".split('_')[1:3]))")
    nn=$(python -c "print(\"$templ_bname\".split('_')[2][1:])")
    for pp in $PP; do
        prob_name=problems/prob_"$name"
        bb=$(python -c "print(int(round( int(\"$nn\") * 2*10000/(3*(3.141592653589793**0.5)) * $pp**(-1.5)  )))")
        ppf=$(printf "%02d" $pp)
        # Create LP SPLP
        sed -e "s/<<FCOST>>/$bb/g" "$templ_name" > "$prob_name"_p"$ppf"_lp_splp
    done
done
for templ_name in templates/template_*_lp_pm; do
    templ_bname=$(basename "$templ_name")
    name=$(python -c "print('_'.join(\"$templ_bname\".split('_')[1:3]))")
    for pp in $PP; do
        prob_name=problems/prob_"$name"
        ppf=$(printf "%02d" $pp)
        # Create LP p-median
        sed -e "s/<<PP>>/$pp/g" "$templ_name" > "$prob_name"_p"$ppf"_lp_pm
    done
done
