if (( $# != 0 )); then
    echo "usage: bash solve_problems.sh"
    exit 1
fi

# Values for p and b
PP="4 8"
BB="256 90"

# Just in case, merge results now.
bash merge_results.sh

read -p "Delete all problems first [y/n]? " choice
case "$choice" in
  y|Y ) echo "Deleting them and creating new ones." && rm -rf problems;;
  n|N ) echo "Not deleting them.";;
  * ) echo "Invalid answer!" && exit;;
esac

read -p "Delete all results first [y/n]? " choice
case "$choice" in
  y|Y ) echo "Deleting them." && rm -rf results;;
  n|N ) echo "Not deleting them.";;
  * ) echo "Invalid answer!" && exit;;
esac

QSUBPROCS=3
LOCALPROCS=2

if [ ! -d problems ]; then
    bash _generate_problems.sh "$PP" "$BB"
fi

for pp in $PP; do
    hash qsub

    if [ $? -eq 0 ]; then

        for proct in $(seq $QSUBPROCS); do
            proc=$((proct-1))

            qsub -N pm_lps_"$pp"_"$proc" _solve_lpsolve.sh \
                -F "$proc $QSUBPROCS $pp pm"

            qsub -N sp_lps_"$pp"_"$proc" _solve_lpsolve.sh \
                -F "$proc $QSUBPROCS $pp splp"

            qsub -N pm_d050_"$pp"_"$proc" _solve_dsa.sh \
                -F "$proc $QSUBPROCS $pp pm 50"

            qsub -N sp_d050_"$pp"_"$proc" _solve_dsa.sh \
                -F "$proc $QSUBPROCS $pp splp 50"

            qsub -N pm_d200_"$pp"_"$proc" _solve_dsa.sh \
                -F "$proc $QSUBPROCS $pp pm 200 400"

            qsub -N sp_d200_"$pp"_"$proc" _solve_dsa.sh \
                -F "$proc $QSUBPROCS $pp splp 200 400"

            qsub -N pm_d400_"$pp"_"$proc" _solve_dsa.sh \
                -F "$proc $QSUBPROCS $pp pm 400 800"

            qsub -N sp_d400_"$pp"_"$proc" _solve_dsa.sh \
                -F "$proc $QSUBPROCS $pp splp 400 800"

        done

    else

        for proct in $(seq $LOCALPROCS); do
            proc=$((proct-1))

            bash _solve_lpsolve.sh "$proc" "$LOCALPROCS" "$pp" "pm"
            bash _solve_lpsolve.sh "$proc" "$LOCALPROCS" "$pp" "splp"

            bash _solve_dsa.sh "$proc" "$LOCALPROCS" "$pp" "pm" "50"
            bash _solve_dsa.sh "$proc" "$LOCALPROCS" "$pp" "splp" "50"

            bash _solve_dsa.sh "$proc" "$LOCALPROCS" "$pp" "pm" "200" "400"
            bash _solve_dsa.sh "$proc" "$LOCALPROCS" "$pp" "splp" "200" "400"

            bash _solve_dsa.sh "$proc" "$LOCALPROCS" "$pp" "pm" "400" "800"
            bash _solve_dsa.sh "$proc" "$LOCALPROCS" "$pp" "splp" "400" "800"

        done

    fi

done
