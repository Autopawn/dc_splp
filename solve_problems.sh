if (( $# != 1 )); then
    echo "usage: bash solve_problems.sh <p_values>"
    exit 1
fi

bash generate_problems.sh $1

qsub -o proc1.out -e proc1.err -N pm_lpsolve solve_lpsolve.sh -F "pm"
qsub -o proc2.out -e proc2.err -N splp_lpsolve solve_lpsolve.sh -F "splp"

qsub -o proc3.out -e proc3.err -N pm_dsa200 solve_dsa.sh -F "pm 200"
qsub -o proc4.out -e proc4.err -N splp_dsa200 solve_dsa.sh -F "splp 200"

qsub -o proc5.out -e proc5.err -N pm_dsa200vr400 solve_dsa.sh -F "pm 200 400"
qsub -o proc6.out -e proc6.err -N splp_dsa200vr400 solve_dsa.sh -F "splp 200 400"

qsub -o proc7.out -e proc7.err -N pm_dsa400vr800 solve_dsa.sh -F "pm 400 800"
qsub -o proc8.out -e proc8.err -N splp_dsa400vr800 solve_dsa.sh -F "splp 400 800"
