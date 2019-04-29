
algorithms="\
dc_bes_200_0 \
dc_ran_200_0 \
dc_dishaumin_200_400 \
dc_dishausum_200_400 \
dc_dismsemin_200_400 \
dc_dismsesum_200_400 \
dc_discli_200_400 \
dc_bes_400_0 \
dc_ran_400_0 \
dc_dishaumin_400_600 \
dc_dishausum_400_600 \
dc_dismsemin_400_600 \
dc_dismsesum_400_600 \
dc_discli_400_600 \
dc_bes_1000_0 \
dc_ran_1000_0 \
"

echo "Table SPLP opt"
first=1
for alg in $algorithms; do
    if [ "$first" -eq 1 ]; then
        python summary.py splp/ res/"$alg"/splp/ | grep "TOTALS:" -A2
        first=0
    else
        python summary.py splp/ res/"$alg"/splp/ | grep "TOTALS:" -A2 | tail -n 1
    fi
done

echo "Table SPLP bub"
first=1
for alg in $algorithms; do
    if [ "$first" -eq 1 ]; then
        python summary.py splp/ res/"$alg"/splp/ | grep "BUB:" -A2
        first=0
    else
        python summary.py splp/ res/"$alg"/splp/ | grep "BUB:" -A2 | tail -n 1
    fi
done


echo "Table p-median opt"
first=1
for alg in $algorithms; do
    if [ "$first" -eq 1 ]; then
        python summary.py pmedian/ res/"$alg"/pmedian/ 2>/dev/null | grep "TOTALS:" -A2
        first=0
    else
        python summary.py pmedian/ res/"$alg"/pmedian/ 2>/dev/null | grep "TOTALS:" -A2 | tail -n 1
    fi
done
