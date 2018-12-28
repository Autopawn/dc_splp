fnames=$(find splp/ | grep -v '.opt' | grep -v '.bub' | grep -v 'README' | grep -v '.c' | grep -v '.lst')

parameters="50_0 100_200 200_400"

make

rm -rd res || true

for params in $parameters; do
    problems=""
    for p in $fnames; do
        if [[ -d $p ]]; then
            if [ -n "$problems" ]; then
                name="$params"_"$(basename $group)"
                echo qsub -N $name solve.sh \
                    -F "$params \"$group\" \"$problems\"" || \
                bash solve.sh $params "$group" "$problems"
            fi
            problems=""
            group="$p"
        elif [[ -f $p ]]; then
            problems="$p $problems"
        else
            echo "$p is not valid"
            exit 1
        fi
    done
done
