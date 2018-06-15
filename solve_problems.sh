if (( $# != 1 )); then
    echo "usage: bash solve_problems.sh <p_values>"
    exit 1
fi

bash generate_problems.sh $1

qsub -N pm_lpsolve solve_lpsolve.sh -F "5 pm" || \
    bash solve_lpsolve.sh "2" "pm"

qsub -N splp_lpsolve solve_lpsolve.sh -F "5 splp" || \
    bash solve_lpsolve.sh "2" "splp"


qsub -N pm_dsa200 solve_dsa.sh -F "5 pm 200" || \
    bash solve_dsa.sh "2" "pm" "200"

qsub -N splp_dsa200 solve_dsa.sh -F "5 splp 200" || \
    bash solve_dsa.sh "2" "splp" "200"


qsub -N pm_dsa200vr400 solve_dsa.sh -F "5 pm 200 400" || \
    bash solve_dsa.sh "2" "pm" "200" "400"

qsub -N splp_dsa200vr400 solve_dsa.sh -F "5 splp 200 400" || \
    bash solve_dsa.sh "2" "splp" "200" "400"


qsub -N pm_dsa400vr800 solve_dsa.sh -F "5 pm 400 800" || \
    bash solve_dsa.sh "2" "pm" "400" "800"

qsub -N splp_dsa400vr800 solve_dsa.sh -F "5 splp 400 800" || \
    bash solve_dsa.sh "2" "splp" "400" "800"
