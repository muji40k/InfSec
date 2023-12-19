# /bin/bash

for file in ref/*
do
    echo ./a.out compress $file ${file//ref/arch}
    ./a.out compress $file ${file//ref/arch}
    echo ./a.out decompress ${file//ref/arch} ${file//ref/res}
    ./a.out decompress ${file//ref/arch} ${file//ref/res}
done

