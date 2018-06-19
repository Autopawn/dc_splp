if (( $# != 1 )); then
    echo "usage: bash solve_problems.sh <p_values>"
    exit 1
fi

bash generate_problems.sh $1

qsub -N pm_lpsolve solve_lpsolve.sh -F "5 pm" || \
    bash solve_lpsolve.sh "2" "pm"
qsub -N splp_lpsolve solve_lpsolve.sh -F "5 splp" || \
    bash solve_lpsolve.sh "2" "splp"

# qsub -N pm_dsa100vr400 solve_dsa.sh -F "5 pm 100 500" || \
#     bash solve_dsa.sh "2" "pm" "100" "500"
# qsub -N splp_dsa100vr400 solve_dsa.sh -F "5 splp 100 500" || \
#     bash solve_dsa.sh "2" "splp" "100" "500"

qsub -N pm_dsa100 solve_dsa.sh -F "3 pm 100" || \
    bash solve_dsa.sh "1" "pm" "100"
qsub -N splp_dsa100 solve_dsa.sh -F "3 splp 100" || \
    bash solve_dsa.sh "1" "splp" "100"

qsub -N pm_dsa200vr500 solve_dsa.sh -F "5 pm 200 500" || \
    bash solve_dsa.sh "2" "pm" "200" "500"
qsub -N splp_dsa200vr500 solve_dsa.sh -F "5 splp 200 500" || \
    bash solve_dsa.sh "2" "splp" "200" "500"

qsub -N pm_dsa400vr1000 solve_dsa.sh -F "5 pm 400 1000" || \
    bash solve_dsa.sh "2" "pm" "400" "1000"
qsub -N splp_dsa400vr1000 solve_dsa.sh -F "5 splp 400 1000" || \
    bash solve_dsa.sh "2" "splp" "400" "1000"
