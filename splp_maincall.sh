# NOTE: for splp
parameters1="\
    dc_dismsemin_200_400 dc_dismsesum_200_400 \
    dc_dishaumin_200_400 dc_dishausum_200_400 \
    dc_discli_200_400 \
    dc_bes_200_0 \
    dc_ran_200_0 \
    dc_dismsemin_400_600 dc_dismsesum_400_600 \
    dc_dishaumin_400_600 dc_dishausum_400_600 \
    dc_discli_400_600 \
    dc_bes_400_0 \
    dc_ran_400_0 \
    dc_bes_1000_0 \
    dc_ran_1000_0 \
    "

# NOTE: for custom (we way want to extend it to splp)
parameters2="\
    dc_dismsemin_200_400 \
    dc_bes_200_0 \
    dc_ran_200_0 \
    dc_dismsemin_400_600 \
    dc_bes_400_0 \
    dc_ran_400_0 \
    dc_bes_1000_0 \
    dc_ran_1000_0 \
    "

# NOTE: for plarge and splpkmedian
parameters3="\
    dc_dismsemin_50_100 dc_dismsesum_50_100 \
    dc_dishaumin_50_100 dc_dishausum_50_100 \
    dc_discli_50_100 \
    dc_bes_200_0 \
    dc_ran_200_0 \
    dc_bes_400_0 \
    dc_ran_400_0 \
    dc_bes_1000_0 \
    dc_ran_1000_0 \
    "

# Delete problem_list files
mkdir -p res
for probfile in $(find res | grep problem_list); do
    rm $probfile
done

bash -xe splp_call.sh splp res "$parameters1"
bash -xe splp_call.sh pmedian res "$parameters1"
bash -xe splp_call.sh custom res "$parameters2"
bash -xe splp_call.sh pmedianlarge res "$parameters3"
bash -xe splp_call.sh splpkmedian res "$parameters3"
