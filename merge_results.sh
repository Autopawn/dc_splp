# Merges the current results on a single file
for fo in $(find results/* -maxdepth 0 -type d); do
    for part in $fo"/"*_p; do
        cat "$part"/* > ${part%_p} 2>/dev/null
    done
done
