#!/bin/bash

rm -fr bin
rm -fr lib
rm -fr includes
rm -fr test

#Run clean target for all modules
modules=("common" "common++" "log" "log++" "ml" "dtkmlc" "mpm")
for module in ${modules[@]}
do
    make -C $module clean
done