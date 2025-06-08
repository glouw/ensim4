commits=$(git rev-list --reverse master)

for commit in $commits; do
    git checkout --quiet "$commit" || continue

    lines=$(find src -type f -exec cat {} + 2>/dev/null | wc -l)

    t0=$(date +%s.%N)
    make > /dev/null 2>&1
    t1=$(date +%s.%N)

    compile_time_secs=$(echo "$t1 - $t0" | bc)

    printf "%-40s %10d lines %10.3f seconds\n" "$commit" "$lines" "$compile_time_secs"
done

git checkout master
