#! /bin/bash

for file in ref/*
do
    echo ./a.out sign ${file} ${file/ref/sign}.sig
    ./a.out sign ${file} ${file/ref/sign}.sig
    echo md5sum ${file}
    md5sum ${file} | grep -o -E [0-9a-f]{32}
    echo
done

