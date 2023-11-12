# /bin/bash

echo ./a.out enc ref/$1 enc/$1
./a.out enc ref/$1 enc/$1

echo ./a.out dec enc/$1 res/$1
./a.out dec enc/$1 res/$1

