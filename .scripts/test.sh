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
    ../bin/project init -n $1 -b $2 $3 -T ../templates/cpp.dtk -T ../templates/c.dtk
}

pr_init(){
    _pr_init $1 $1 ""
    _pr_init $1_c $1 "-t c"
    _pr_init $1_cpp $1 "-t cpp"
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

_test(){
    echo -n "Testing $1 - "
    if [[ $2 == "Hello World!" ]]
    then
        echo Passed
    else
        echo Failed
    fi
}

test(){
    echo Running { $1* }
    _run $1
    _run $1_c
    _run $1_cpp
    _test "C" "$(_exe $1_c)"
    _test "C++" "$(_exe $1_cpp)"
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
    _pr_do $1 disabe $2
}

pr_template(){
    echo project { $1 } template -T $2
    _pr $1 template "-T $2"
}

pr_module(){
    echo project { $1 } module -n module_$2 -t $2
    _pr $1 module "-n module_$2 -t $2"
}

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

#project template

pr_template make ../templates/c.txt
pr_template bash ../templates/c.txt

#project module

pr_module make c
pr_module bash c

#Test #5

test make
test bash

#Leave test
cd ..