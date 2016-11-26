#!/bin/bash

#../set_gen/a.out 5 | ./dlx
#mv gmon.out gmon.sum

for i in `seq 8 15`
do
    for j in `seq 1 10`
    do
        echo $i $j
        ../set_gen/a.out $i | ./dlx > /dev/null
        gprof -s dlx gmon.out gmon.sum
    done
done
