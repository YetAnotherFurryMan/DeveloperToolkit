#!/bin/bash

#Exports
rm -f bin/*
rm -f lib/*
rm -f includes/*

#log
make -C log clean

#log++
make -C log++ clean

#common
make -C common clean

#common++
make -C common++ clean

#ml
make -C ml clean

#dtkmlc
make -C dtkmlc clean

#project
make -C project clean

#Test
cd test
rm -rf bash* make* *.out.*
cd ..
