
if (( $# != 2 )); then
    echo "usage: bash generate_templates.sh <n_values> <n_probs>"
    exit 1
fi

NPROBS="$2"
NN="$1"

rm -rf templates || true
mkdir -p templates


NAMEFORMAT="n%04d_i%04d"

# Create problems
for nn in $NN; do
    for prob in $(seq 1 $NPROBS); do
        name=$(printf $NAMEFORMAT $nn $prob)
        templ_name=templates/template_"$name"
        python tools/template_gen.py $nn $nn 10000 "$templ_name"
    done
done
