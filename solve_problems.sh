if (( $# != 1 )); then
    echo "usage: bash solve_problems.sh <p_values>"
    exit 1
fi

bash generate_problems.sh $1

qsub -N pm_lpsolve solve_lpsolve.sh -F "pm" || \
    bash solve_lpsolve.sh "pm"

qsub -N splp_lpsolve solve_lpsolve.sh -F "splp" || \
    bash solve_lpsolve.sh "splp"


qsub -N pm_dsa200 solve_dsa.sh -F "pm 200" || \
    bash solve_dsa.sh "pm" "200"

qsub -N splp_dsa200 solve_dsa.sh -F "splp 200" || \
    bash solve_dsa.sh "splp" "200"


qsub -N pm_dsa200vr400 solve_dsa.sh -F "pm 200 400" || \
    bash solve_dsa.sh "pm" "200" "400"

qsub -N splp_dsa200vr400 solve_dsa.sh -F "splp 200 400" || \
    bash solve_dsa.sh "splp" "200" "400"


qsub -N pm_dsa400vr800 solve_dsa.sh -F "pm 400 800" || \
    bash solve_dsa.sh "pm" "400" "800"

qsub -N splp_dsa400vr800 solve_dsa.sh -F "splp 400 800" || \
    bash solve_dsa.sh "splp" "400" "800"
