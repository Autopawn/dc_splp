#

target="splp_custom"

# Tunning for the HPC cluster:
if [ -n "${PBS_O_WORKDIR+1}" ]; then
    cd "$PBS_O_WORKDIR"
    export lp_solve="$HOME/lp_solve_5.5/lp_solve/bin/ux64/lp_solve"
else
    export lp_solve="lp_solve"
fi

files=$(find $target | grep '\.lp')

for file in $files; do
    result="${file%.lp}.opt"
    if [[ ! -e "$result" ]]; then
        tmpfile=$(mktemp /tmp/splp_custom_lpsolve.XXXXXXXX)
        # Solve:
        time_start=$(date +%s)
        "$lp_solve" "$file" > "$tmpfile"
        time_end=$(date +%s)
        runtime=$((time_end-time_start))
        #
        cat "$tmpfile" | grep 'Value' | cut -d':' -f2 > "$result"
        cat "$tmpfile" | grep 'Y' | grep ' 1' | cut -f1 -d' ' | cut -d'c' -f2 >> "$result"
        nfacs=$(cat "$tmpfile" | grep 'X' | grep ' 1' | wc -l)
        cat "$result" > "$tmpfile"
        cat "$tmpfile" | tr '\n' ' ' > "$result"
        echo -e '\n#runtime: '"$runtime" >> "$result"
        echo -e '\n#nfacs: '"$nfacs" >> "$result"
        rm "$tmpfile"
    fi
done
