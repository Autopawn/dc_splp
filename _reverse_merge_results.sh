for fo in $(find results/* -maxdepth 1 -type f); do
    echo "reversing" $fo
    mkdir -p "$fo"_p
    #
    while read lin; do
        tgt=$(echo "$lin" | awk '{print $1}')
        val=$(echo "$lin" | awk '{print $2}')
        if [[ ! -z "$tgt" && ! -z "$val" ]]; then
            fname="$fo"_p/"$tgt"
            echo "$lin" > "$fname"
        fi
    done < "$fo"
done
