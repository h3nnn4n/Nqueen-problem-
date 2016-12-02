#!/bin/bash

if [[ -e gmon.sum ]]
then
    echo -n ''
else
    ./dlx >> /dev/null
    mv gmon.out gmon.sum
fi

for i in `seq 4 10`
do
    echo -n "$i "
    for j in `seq 1 10`
    do
        ./dlx $i 1 >> /dev/null
        ./dlx $i 2 >> /dev/null
        ./dlx $i 3 >> /dev/null
        gprof -s dlx gmon.out gmon.sum
    done
done
echo ''
