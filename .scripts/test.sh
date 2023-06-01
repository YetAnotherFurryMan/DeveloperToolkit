#!/bin/bash

#Enter test
cd test

#Clean
rm -rf bash* make* *.out.*

#ml
make test/test -C ../ml

#dtkmlc
_dtkmlc(){
    ../bin/dtkmlc -$1 $2.dtk -o $2.out.$1
}

_dtkmlc txt test
_dtkmlc xml test

#Do not test project
exit

#Functions
_pr_init(){
    ../bin/project init -n $1 -b $2 $3 -T templates/cpp.txt -T templates/c.txt
}

pr_init(){
    _pr_init $1 $1 ""
    _pr_init $1_c $1 "-t c"
    _pr_init $1_cpp $1 "-t cpp"
    _pr_init $1_c_cpp $1 "-t c_cpp"
}

_run(){
    cd $1
    chmod +x run
    chmod +x .scripts/*.sh
    ./run
    cd ..
}

_exe(){
    cd $1
    ./$1
    cd ..
}

test(){
    echo Running $1*
    _run $1
    _run $1_c
    _run $1_cpp
    _run $1_c_cpp
    echo Executing C
    _exe $1_c
    echo Executing C++
    _exe $1_cpp
    echo Executing C/C++
    _exe $1_c_cpp
}

_pr(){
    cd $1
    ../../bin/project $2 $3
    ../../bin/project info
    cd ..
}

pr_enable(){
    echo project { $1* } enable $2
    _pr $1 enable $2
    _pr $1_c enable $2
    _pr $1_cpp enable $2
    _pr $1_c_cpp enable $2
}

pr_disable(){
    echo project { $1* } disable $2
    _pr $1 disable $2
    _pr $1_c disable $2
    _pr $1_cpp disable $2
    _pr $1_c_cpp disable $2
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

#add permossions
chmod +x bash/module_c/*.sh

#Test #5

test make
test bash

#Leave test
cd ..