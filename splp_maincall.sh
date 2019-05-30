# NOTE: for testing
parameters0="\
    dc_dismsemin_4000_200_400 dc_dismsesum_4000_200_400 \
    dc_dishaumin_4000_200_400 dc_dishausum_4000_200_400 \
    dc_discli_4000_200_400 \
    dc_bes_0_200_0 \
    dc_ran_0_200_0 \
    "

# NOTE: for splp
parameters1="\
    dc_dismsemin_0_200_400 dc_dismsesum_0_200_400 \
    dc_dishaumin_0_200_400 dc_dishausum_0_200_400 \
    dc_discli_0_200_400 \
    dc_bes_0_200_0 \
    dc_ran_0_200_0 \
    dc_dismsemin_0_400_600 dc_dismsesum_0_400_600 \
    dc_dishaumin_0_400_600 dc_dishausum_0_400_600 \
    dc_discli_0_400_600 \
    dc_bes_0_400_0 \
    dc_ran_0_400_0 \
    dc_bes_0_1000_0 \
    dc_ran_0_1000_0 \
    "

# NOTE: for custom
parameters2="\
    dc_dismsemin_0_200_400 \
    dc_bes_0_200_0 \
    dc_ran_0_200_0 \
    dc_dismsemin_0_400_600 \
    dc_bes_0_400_0 \
    dc_ran_0_400_0 \
    dc_bes_0_1000_0 \
    dc_ran_0_1000_0 \
    "

# NOTE: for plarge and splpkmedian
parameters3="\
    dc_dismsemin_0_50_100 dc_dismsesum_0_50_100 \
    dc_dishaumin_0_50_100 dc_dishausum_0_50_100 \
    dc_discli_0_50_100 \
    dc_bes_0_200_0 \
    dc_ran_0_200_0 \
    dc_bes_0_400_0 \
    dc_ran_0_400_0 \
    dc_bes_0_1000_0 \
    dc_ran_0_1000_0 \
    "

# Delete problem_list files
mkdir -p res
for probfile in $(find res | grep problem_list); do
    rm $probfile
done

bash -xe splp_call.sh splp res "$parameters0"
# bash -xe splp_call.sh splp res "$parameters1"
# bash -xe splp_call.sh pmedian res "$parameters1"
# bash -xe splp_call.sh custom res "$parameters2"
# bash -xe splp_call.sh pmedianlarge res "$parameters3"
# bash -xe splp_call.sh splpkmedian res "$parameters3"
