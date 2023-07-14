#!/bin/bash

#Enter test
mkdir test 2> /dev/null
cd test

#Clean
rm -rf bash* make* *.out.*

#ml
make -C ../ml test/test

#dtkmlc
_dtkmlc(){
    ../bin/dtkmlc -$1 $2.dtk -o $2.out.$1
}

cp ../ml/test/test.dtk test.dtk

_dtkmlc txt test
_dtkmlc xml test

#Functions
_pr_init(){
    ../bin/mpm init -n $1 -b $2 $3
}

pr_init(){
    _pr_init $1 $1 ""
    _pr_init $1_c $1 "-t c -T ../templates/c.dtk"
    _pr_init $1_c_m $1 "-t c_math -T ../templates/c.dtk"
    _pr_init $1_cpp $1 "-t cpp -T ../templates/cpp.dtk"
}

_run(){
    cd $1
    bash run
    cd ..
}

_exe(){
    cd $1
    ./$1
    cd ..
}

_exe2(){
    cd $1
    ./$1 $2
    cd ..
}

test_count=0
test_passed=0

_test_hello_world(){
    echo -n "Testing $1 - "
    let "test_count += 1"
    if [[ $2 == "Hello World!" ]]
    then
        echo Passed
        let "test_passed += 1"
    else
        echo Failed
    fi
}

_test_math(){
    echo -n "Testing $1 - "
    let "test_count += 1"
    excepted=`echo "scale=6; sqrt($3)" | bc`
    if [[ $2 == $excepted ]]
    then
        echo Passed
        let "test_passed += 1"
    else
        echo Failed
    fi
}

test(){
    echo Running { $1* }
    _run $1
    _run $1_c
    _run $1_c_m
    _run $1_cpp
    _test_hello_world "C" "$(_exe $1_c)"
    _test_hello_world "C++" "$(_exe $1_cpp)"
    _test_math "C Math" "$(_exe $1_c_m)" 64
    _test_math "C Math (4)" "$(_exe2 $1_c_m 4)" 4
    _test_math "C Math (256)" "$(_exe2 $1_c_m 256)" 256
}

_pr(){
    cd $1
    ../../bin/project $2 $3
    ../../bin/project info
    cd ..
}

_pr_do(){
    echo project { $1* } $2 $3
    for x in $1*
    do 
        _pr $x $2 $3
    done
}

pr_enable(){
    _pr_do $1 enable $2
}

pr_disable(){
    _pr_do $1 disable $2
}

pr_add(){
    echo project { $1 } add $2 $3
    _pr $1 add "$2 $3"
}

pr_add_template(){
    pr_add $1 template $2
}

pr_add_filtered_templates(){
    pr_add $1 template "$2 $3"
}

pr_add_module(){
    pr_add $1 module "-n module_$2 -t $2"
}

#project help
../bin/mpm
../bin/mpm help

#project init

pr_init make
pr_init bash

#Test #1

test make
test bash

#project enable log

pr_enable make log
pr_enable bash log

#Test #2

test make
test bash

#project enable test

pr_enable make test
pr_enable bash test

#Test #3

test make
test bash

#project disable test

pr_disable make test
pr_disable bash test

#Test #4

test make
test bash

#project disable log

pr_disable make log
pr_disable bash log

#Test #4

test make
test bash

#project add template

pr_add_template make ../../templates/c.dtk
pr_add_template bash ../../templates/c.dtk

pr_add_filtered_templates make ../../templates/c_cpp_templates_collection.dtk "-n cpp"
pr_add_filtered_templates bash ../../templates/c_cpp_templates_collection.dtk "-n cpp"

#project module

pr_add_module make c
pr_add_module bash c

pr_add_module make cpp
pr_add_module bash cpp

#Test #5

test make
test bash

#Summary

echo Summary $test_passed / $test_count

#Leave test
cd ..