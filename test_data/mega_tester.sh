#!/bin/bash

# CONFIG
MIN_SIZE=10
MAX_SIZE=17
MIN_FROW=1
MAX_FROW=6
MIN_NP=2
MAX_NP=9
N_TESTS=10

BIN=../dlx_src/dlx

DATA_FOLDER=data

# Tests for the existence of the 'data' folder, creates it if necessary
if [[ ! -d "$DATA_FOLDER" ]]
then
    mkdir -v $DATA_FOLDER
fi

# Checks if the binary exists, if not then make it
if [[ ! -e "$BIN" ]]
then
    cd ../dlx_src
    make -j
    cd -
fi

for size in `seq $MIN_SIZE $MAX_SIZE`
do
    for frow in `seq $MIN_FROW $MAX_FROW`
    do
        echo -n "$bin $size $frow: "
        for i in `seq $N_TESTS`
        do
            echo -n "${i} "
            mpirun -np 9 ./$BIN $size $frow >> $DATA_FOLDER/${size}_${frow}
        done
        echo -e ''
    done
done
