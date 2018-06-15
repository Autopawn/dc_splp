rm -r results
mkdir -p results

qsub -o p1.out -e p1.err -N pm_lpsolve solve_lpsolve.sh pm
qsub -o p2.out -e p2.err -N splp_lpsolve solve_lpsolve.sh splp

qsub -o p3.out -e p3.err -N pm_dsa200 solve_dsa.sh pm 200
qsub -o p4.out -e p4.err -N splp_dsa200 solve_dsa.sh splp 200

qsub -o p5.out -e p5.err -N pm_dsa200vr400 solve_dsa.sh pm 200 400
qsub -o p6.out -e p6.err -N splp_dsa200vr400 solve_dsa.sh splp 200 400

qsub -o p7.out -e p7.err -N pm_dsa400vr800 solve_dsa.sh pm 400 800
qsub -o p8.out -e p8.err -N splp_dsa400vr800 solve_dsa.sh splp 400 800
