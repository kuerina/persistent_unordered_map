testFile="./a.out"
rm t*
for power in {3..10} ; do
    echo  "{ time $testFile $((10**$power)) ; } 2> time; grep real time >> time.txt"
    { time $testFile $((10**$power)) ; } 2> time; grep real time >> values.txt
    echo -n "$((10**$power))" 
    spac="$((10-$power))"
    tail values.txt -n 1
    rm t*
done
