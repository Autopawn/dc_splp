if (( $# != 1 )); then
    echo "usage: bash solve_problems.sh <p_values>"
    exit 1
fi

# NOTE: should delete problems/
rm -rf results
bash _generate_problems.sh "$1"

for pp in $1; do

    qsub -N pm_lpsolve _solve_lpsolve.sh -F "$pp 5 pm" || \
        bash _solve_lpsolve.sh "$pp" "2" "pm"
    qsub -N splp_lpsolve _solve_lpsolve.sh -F "$pp 5 splp" || \
        bash _solve_lpsolve.sh "$pp" "2" "splp"

    qsub -N pm_dsa50 _solve_dsa.sh -F "$pp 3 pm 50" || \
        bash _solve_dsa.sh "$pp" "1" "pm" "50"
    qsub -N splp_dsa50 _solve_dsa.sh -F "$pp 3 splp 50" || \
        bash _solve_dsa.sh "$pp" "1" "splp" "50"

    qsub -N pm_dsa200vr400 _solve_dsa.sh -F "$pp 5 pm 200 400" || \
        bash _solve_dsa.sh "$pp" "2" "pm" "200" "400"
    qsub -N splp_dsa200vr400 _solve_dsa.sh -F "$pp 5 splp 200 400" || \
        bash _solve_dsa.sh "$pp" "2" "splp" "200" "400"

    qsub -N pm_dsa400vr800 _solve_dsa.sh -F "$pp 5 pm 400 800" || \
        bash _solve_dsa.sh "$pp" "2" "pm" "400" "800"
    qsub -N splp_dsa400vr800 _solve_dsa.sh -F "$pp 5 splp 400 800" || \
        bash _solve_dsa.sh "$pp" "2" "splp" "400" "800"

done
