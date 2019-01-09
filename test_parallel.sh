targets="common.c dsa.c expand.c load.c solution.c reduce.c"

make many_bin
for i in 0 1 2 3 4 5 6 7 8 9 10; do
    ./bin/dsa_ls_$$i 100 200 10 splp/Euclid/1011EuclS.txt tests/res_dc_euclid_$$i.txt tests/res_dc_euclid_ls_$$i.txt;
done
for i in 0 1 2 3 4 5 6 7 8 9 10; do
    ./bin/dsa_ls_$$i 100 200 10 splp/GapA/1032GapAS.txt tests/res_dc_gapa_$$i.txt tests/res_dc_gapa_ls_$$i.txt;
done
for i in 0 1 2 3 4 5 6 7 8 9 10; do
    ./bin/dsa_ls_$$i 100 200 10 splp/Fpp17/27FPP_17S.txt tests/res_dc_27fpp_$$i.txt tests/res_dc_27fpp_ls_$$i.txt;
done
for i in 0 1 2 3 4 5 6 7 8 9 10; do
    ./bin/dsa_ls_$$i 100 200 10 splp/M/S/MS1 tests/res_dc_ms1_$$i.txt tests/res_dc_ms1_ls_$$i.txt;
done
