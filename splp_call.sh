fnames=$(find splp/ | grep -v 'kmedian' | grep -v '.opt' | grep -v '.bub' | grep -v 'README' | grep -v '.c' | grep -v '.lst' | grep -v '~')

parameters="\
    dc_norm_m_200_400 dc_norm_s_200_400 \
    dc_haus_m_200_400 dc_haus_s_200_400 \
    dc_norm_m_200_-1 dc_norm_s_200_-1 \
    dc_best_m_200_400 dc_best_s_200_400 "

make

# rm -rf res || true

for params in $parameters; do
    problems=""
    for p in $fnames; do
        if [[ -d $p ]]; then
            if [ -n "$problems" ]; then
                gname="$(echo $group | tr / _ | cut -d'_' -f2-)"
                n2=$(echo $params | cut -d'_' -f2)
                n3=$(echo $params | cut -d'_' -f3)
                ng=$(echo $params | cut -d'_' -f4-5)
                name=dc_"${n2:0:1}${n3:0:1}"_"$ng"_"$gname"
                qsub -N $name splp_solve.sh \
                    -F "$params \"$group\" \"$problems\"" || \
                bash splp_solve.sh $params "$group" "$problems"
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
