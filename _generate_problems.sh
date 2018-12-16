
if (( $# != 2 )); then
    echo "usage: bash generate_problems.sh <p_vals> <b_vals>"
    exit 1
fi

PP="$1"
BB="$2"
NELEMS=$(echo "$PP" | wc -w)

rm -rf problems || true
mkdir -p problems

NAMEFORMAT="n%04d_i%04d"

# Create problems
for templ_name in templates/template_*_dsa_splp; do
    templ_bname=$(basename "$templ_name")
    name=$(python -c "print('_'.join(\"$templ_bname\".split('_')[1:3]))")
    nn=$(python -c "print(\"$templ_bname\".split('_')[1][1:])")
    for ii in $(seq $NELEMS); do
        pp=$(echo $PP | tr " " "\n"|sed -n "$ii"'p')
        bb=$(echo $BB | tr " " "\n"|sed -n "$ii"'p')
        bb=$((bb*10#$nn))
        prob_name=problems/prob_"$name"
        ppf=$(printf "%02d" $pp)
        # Create DSA SPLP
        sed -e "s/<<FCOST>>/$bb/g" "$templ_name" > "$prob_name"_p"$ppf"_dsa_splp
    done
done
for templ_name in templates/template_*_dsa_pm; do
    templ_bname=$(basename "$templ_name")
    name=$(python -c "print('_'.join(\"$templ_bname\".split('_')[1:3]))")
    for ii in $(seq $NELEMS); do
        pp=$(echo $PP | tr " " "\n"|sed -n "$ii"'p')
        bb=$(echo $BB | tr " " "\n"|sed -n "$ii"'p')
        bb=$((bb*10#$nn))
        prob_name=problems/prob_"$name"
        ppf=$(printf "%02d" $pp)
        # Create DSA p-median
        sed -e "s/<<PP>>/$pp/g" "$templ_name" > "$prob_name"_p"$ppf"_dsa_pm
    done
done
for templ_name in templates/template_*_lp_splp; do
    templ_bname=$(basename "$templ_name")
    name=$(python -c "print('_'.join(\"$templ_bname\".split('_')[1:3]))")
    nn=$(python -c "print(\"$templ_bname\".split('_')[1][1:])")
    for ii in $(seq $NELEMS); do
        pp=$(echo $PP | tr " " "\n"|sed -n "$ii"'p')
        bb=$(echo $BB | tr " " "\n"|sed -n "$ii"'p')
        bb=$((bb*10#$nn))
        prob_name=problems/prob_"$name"
        ppf=$(printf "%02d" $pp)
        # Create LP SPLP
        sed -e "s/<<FCOST>>/$bb/g" "$templ_name" > "$prob_name"_p"$ppf"_lp_splp
    done
done
for templ_name in templates/template_*_lp_pm; do
    templ_bname=$(basename "$templ_name")
    name=$(python -c "print('_'.join(\"$templ_bname\".split('_')[1:3]))")
    for ii in $(seq $NELEMS); do
        pp=$(echo $PP | tr " " "\n"|sed -n "$ii"'p')
        bb=$(echo $BB | tr " " "\n"|sed -n "$ii"'p')
        bb=$((bb*10#$nn))
        prob_name=problems/prob_"$name"
        ppf=$(printf "%02d" $pp)
        # Create LP p-median
        sed -e "s/<<PP>>/$pp/g" "$templ_name" > "$prob_name"_p"$ppf"_lp_pm
    done
done
