#! /bin/bash

for file in ref/*
do
    echo ./a.out verify ${file} ${file/ref/sign}.sig
    ./a.out verify ${file} ${file/ref/sign}.sig

    if [ 0 -eq $? ]; then
        echo -e "\033[01;32mPASS\033[00m"
    else
        echo -e "\033[01;31mFAILURE\033[00m"
    fi
done

