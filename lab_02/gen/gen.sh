#! /bin/bash

mkdir $1
mkdir ${1}/f_block
mkdir ${1}/key_block

./a.out ${1}/ip_block 64

for (( i = 1; 9 > i; i++ ))
do
    ./a.out ${1}/f_block/s${i} 64 16
done

./a.out ${1}/f_block/p_block 32

./key_gen.out ${1}/key_block/key

./a.out cd 56
split -b 224 cd

mv xaa ${1}/key_block/c_block
mv xab ${1}/key_block/d_block
rm cd

./a.out ${1}/key_block/shift_blocks 16

./a.out ${1}/key_block/h_block 48 56

