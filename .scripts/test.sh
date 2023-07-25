#!/bin/bash

test_count=0
test_passed=0
critical_faul_no=0
critical_faul_val=0

critical_faul(){
    if [[ $1 -ne 0 ]]
    then
        echo Critical Faul $critical_faul_no
        echo Return value $1
        exit
    fi

    let "critical_faul_no += 1"
}

#Enter test
mkdir test 2> /dev/null
cd test

#Clean
rm -rf ml mpm

#ml
make -C ../ml test/test

#Enter ml
mkdir ml 2> /dev/null
cd ml

#Copy needed files
cp ../../ml/test/test test
cp ../../ml/test/test1.dtk test1.dtk
cp ../../ml/test/test2.dtk test2.dtk

#Functions
ml_test(){
    echo "Testing ml ($1.dtk)"
    let "test_count += 3"

    echo -e "\n\nml_test: $1.dtk\n" >> ../test_stdout.log
    echo -e "\n\nml_test: $1.dtk\n" >> ../test_stderr.log

    ./test $1.dtk $1.out.1.dtk $1.inheritance.1.dtk $1.deinheritance.1.dtk >> ../test_stdout.log 2>> ../test_stderr.log
    critical_faul $?

    ./test $1.out.1.dtk $1.out.2.dtk $1.inheritance.2.dtk $1.deinheritance.2.dtk >> ../test_stdout.log 2>> ../test_stderr.log
    critical_faul $?

    echo -n "Output "
    if cmp -s "$1.out.1.dtk" "$1.out.2.dtk"
    then 
        echo "- Passed"
        let "test_passed += 1"
    else 
        echo "- Failed"
        echo $1.out.1.dtk != $1.out.2.dtk
    fi

    echo -n "Inheritance "
    if cmp -s "$1.inheritance.1.dtk" "$1.inheritance.2.dtk"
    then 
        echo "- Passed"
        let "test_passed += 1"
    else 
        echo "- Failed"
        echo $1.inheritance.1.dtk != $1.inheritance.2.dtk
    fi

    echo -n "Deinheritance "
    if cmp -s "$1.deinheritance.1.dtk" "$1.deinheritance.2.dtk"
    then 
        echo "- Passed"
        let "test_passed += 1"
    else 
        echo "- Failed"
        echo $1.deinheritance.1.dtk != $1.deinheritance.2.dtk
    fi
}

#Tests
ml_test test1
ml_test test2

#Exit ml
cd ..

#Enter mpm
mkdir mpm 2> /dev/null
cd mpm

#Copy needed files
cp ../../bin/mpm mpm
critical_faul $?

#Functions
mpm(){
    echo -n "Return Value Test "
    let "test_count += 1"

    echo -e "\n\nmpm: mpm $2\n" >> ../test_stdout.log
    echo -e "\n\nmpm: mpm $2\n" >> ../test_stderr.log

    ./mpm $2 >> ../test_stdout.log 2>> ../test_stderr.log
    r=$?

    if [[ $r -eq $1 ]]
    then
        echo "- Passed"
        let "test_passed += 1"
    else
        echo "- Failed"
        echo "$r != $1"
        let "critical_faul_val += 1"
    fi
}

pr(){
    cd $2

    echo -n "Return Value Test "
    let "test_count += 1"

    echo -e "\n\npr: mpm $3\n" >> ../../test_stdout.log
    echo -e "\n\npr: mpm $3\n" >> ../../test_stderr.log

    ../mpm $3 >> ../../test_stdout.log 2>> ../../test_stderr.log
    r=$?

    if [[ $r -eq $1 ]]
    then
        echo "- Passed"
        let "test_passed += 1"
    else
        echo "- Failed"
        echo "$r != $1"
        let "critical_faul_val += 1"
    fi

    cd ..
}

pr_run(){
    cd $1

    echo -e "\n\npr_run: ./run\n" >> ../../test_stdout.log
    echo -e "\n\npr_run: ./run\n" >> ../../test_stderr.log

    bash run >> ../../test_stdout.log 2>> ../../test_stderr.log
    if [[ $? -ne 0 ]]
    then
        let "critical_faul_val += 1"
    fi

    cd ..
}

run_test(){
    echo -n "Run Test "
    let "test_count += 1"

    echo -e "\n\nrun_test: ./$2\n" >> ../test_stdout.log
    echo -e "\n\nrun_test: ./$2\n" >> ../test_stderr.log

    ./$2/$2 $3 >> ../test_stdout.log 2>> ../test_stderr.log
    r=$?

    if [[ $r -eq $1 ]]
    then
        echo "- Passed"
        let "test_passed += 1"
    else
        echo "- Failed"
        echo "$r != $1"
    fi
}

run_mod_test(){
    echo -n "Run Mod Test "
    let "test_count += 1"

    echo -e "\n\nrun_mod_test: ./$2/bin/$3\n" >> ../test_stdout.log
    echo -e "\n\nrun_mod_test: ./$2/bin/$3\n" >> ../test_stderr.log

    ./$2/bin/$3 $4 >> ../test_stdout.log 2>> ../test_stderr.log
    r=$?

    if [[ $r -eq $1 ]]
    then
        echo "- Passed"
        let "test_passed += 1"
    else
        echo "- Failed"
        echo "$r != $1"
    fi
}

exe(){
    cd $1
    ./$2 $3
    cd ..
}

hello_world_test(){
    echo -n "Hello World Test "
    let "test_count += 1"

    if [[ $@ == "Hello World!" ]]
    then
        echo "- Passed"
        let "test_passed += 1"
    else
        echo "- Failed"
        echo "\"$@\" != \"Hello World!\""
    fi
}

math_test(){
    echo -n "Math Test "
    let "test_count += 1"
    
    excepted=`echo "scale=6; sqrt($1)" | bc`

    if [[ $2 == $excepted ]]
    then
        echo Passed
        let "test_passed += 1"
    else
        echo Failed
        echo "$2 != $excepted"
    fi
}

#Tests
mpm 0 ""
critical_faul $?

mpm 0 "help"
critical_faul $?

#mpm invalid [TASK] parameter
mpm 130 "invalid"

#mpm init
mpm 0 "init -n bash -b bash"
mpm 0 "init -n bash_c -b bash -t c -T ../../templates/c.dtk"
mpm 0 "init -n bash_c_math -b bash -t c_math -T ../../templates/c.dtk"
mpm 0 "init -n bash_cpp -b bash -t cpp -T ../../templates/cpp.dtk"

mpm 0 "init -n make -b make"
mpm 0 "init -n make_c -b make -t c -T ../../templates/c.dtk"
mpm 0 "init -n make_c_math -b make -t c_math -T ../../templates/c.dtk"
mpm 0 "init -n make_cpp -b make -t cpp -T ../../templates/cpp.dtk"

critical_faul $critical_faul_val

#mpm Excepted argument after -n
mpm 83 "init -n"

#mpm Name already has been set
mpm 83 "init -n name1 -n name2"

#mpm Excepted argument after -b
mpm 83 "init -b"

#mpm Project build system already has been set
mpm 83 "init -b bash -b bash"
mpm 83 "init -b make -b bash"

#mpm Unsupported build system
mpm 83 "init -b unsupported"

#mpm Excepted argument after -t
mpm 83 "init -t"

#mpm Project template already has been set
mpm 83 "init -t a -t b"

#mpm Excepted argument after -T
mpm 83 "init -T"

#mpm Unknown argument
mpm 158 "init -abc"

#mpm Name not given
mpm 83 "init"

#mpm Template not found
mpm 83 "init -n name -t not_found"
mpm 83 "init -n name -t not_found -T ../../templates/c.dtk"

#Clear critical_faul_val
let "critical_faul_val = 0"

#mpm info
pr 0 "bash" "info"
pr 0 "bash_c" "info"
pr 0 "bash_c_math" "info"
pr 0 "bash_cpp" "info"

pr 0 "make" "info"
pr 0 "make_c" "info"
pr 0 "make_c_math" "info"
pr 0 "make_cpp" "info"

critical_faul $critical_faul_val

#mpm pr_run
pr_run "bash"
pr_run "bash_c"
pr_run "bash_c_math"
pr_run "bash_cpp"

pr_run "make"
pr_run "make_c"
pr_run "make_c_math"
pr_run "make_cpp"

critical_faul $critical_faul_val

#mpm run_test
run_test 0 "bash_c" ""
run_test 0 "bash_c_math" ""
run_test 0 "bash_cpp" ""

run_test 0 "make_c" ""
run_test 0 "make_c_math" ""
run_test 0 "make_cpp" ""

#mpm hello world test
hello_world_test $(exe "bash_c" "bash_c" "")
hello_world_test $(exe "bash_cpp" "bash_cpp" "")

hello_world_test $(exe "make_c" "make_c" "")
hello_world_test $(exe "make_cpp" "make_cpp" "")

#mpm math test
math_test 4 $(exe "bash_c_math" "bash_c_math" "4")
math_test 64 $(exe "bash_c_math" "bash_c_math" "")
math_test 256 $(exe "bash_c_math" "bash_c_math" "256")

math_test 4 $(exe "make_c_math" "make_c_math" "4")
math_test 64 $(exe "make_c_math" "make_c_math" "")
math_test 256 $(exe "make_c_math" "make_c_math" "256")

#Clear critical_faul_val
let "critical_faul_val = 0"

#mpm enable log
pr 0 "bash" "enable log"
pr 0 "bash_c" "enable log"
pr 0 "bash_c_math" "enable log"
pr 0 "bash_cpp" "enable log"

pr 0 "make" "enable log"
pr 0 "make_c" "enable log"
pr 0 "make_c_math" "enable log"
pr 0 "make_cpp" "enable log"

critical_faul $critical_faul_val

#mpm pr_run
pr_run "bash"
pr_run "bash_c"
pr_run "bash_c_math"
pr_run "bash_cpp"

pr_run "make"
pr_run "make_c"
pr_run "make_c_math"
pr_run "make_cpp"

critical_faul $critical_faul_val

#mpm run_test
run_test 0 "bash_c" ""
run_test 0 "bash_c_math" ""
run_test 0 "bash_cpp" ""

run_test 0 "make_c" ""
run_test 0 "make_c_math" ""
run_test 0 "make_cpp" ""

#mpm hello world test
hello_world_test $(exe "bash_c" "bash_c" "")
hello_world_test $(exe "bash_cpp" "bash_cpp" "")

hello_world_test $(exe "make_c" "make_c" "")
hello_world_test $(exe "make_cpp" "make_cpp" "")

#mpm math test
math_test 4 $(exe "bash_c_math" "bash_c_math" "4")
math_test 64 $(exe "bash_c_math" "bash_c_math" "")
math_test 256 $(exe "bash_c_math" "bash_c_math" "256")

math_test 4 $(exe "make_c_math" "make_c_math" "4")
math_test 64 $(exe "make_c_math" "make_c_math" "")
math_test 256 $(exe "make_c_math" "make_c_math" "256")

#mpm Failed to open project file
mpm 111 "enable"

#mpm No arguments
pr 83 "bash" "enable"

#mpm Invalid functionality name
pr 130 "bash" "enable invalid"

#Clear critical_faul_val
let "critical_faul_val = 0"

#mpm enable test
pr 0 "bash" "enable test"
pr 0 "bash_c" "enable test"
pr 0 "bash_c_math" "enable test"
pr 0 "bash_cpp" "enable test"

pr 0 "make" "enable test"
pr 0 "make_c" "enable test"
pr 0 "make_c_math" "enable test"
pr 0 "make_cpp" "enable test"

critical_faul $critical_faul_val

#mpm pr_run
pr_run "bash"
pr_run "bash_c"
pr_run "bash_c_math"
pr_run "bash_cpp"

pr_run "make"
pr_run "make_c"
pr_run "make_c_math"
pr_run "make_cpp"

#Not supported yet
#critical_faul $critical_faul_val

#mpm run_test
run_test 0 "bash_c" ""
run_test 0 "bash_c_math" ""
run_test 0 "bash_cpp" ""

run_test 0 "make_c" ""
run_test 0 "make_c_math" ""
run_test 0 "make_cpp" ""

#mpm hello world test
hello_world_test $(exe "bash_c" "bash_c" "")
hello_world_test $(exe "bash_cpp" "bash_cpp" "")

hello_world_test $(exe "make_c" "make_c" "")
hello_world_test $(exe "make_cpp" "make_cpp" "")

#mpm math test
math_test 4 $(exe "bash_c_math" "bash_c_math" "4")
math_test 64 $(exe "bash_c_math" "bash_c_math" "")
math_test 256 $(exe "bash_c_math" "bash_c_math" "256")

math_test 4 $(exe "make_c_math" "make_c_math" "4")
math_test 64 $(exe "make_c_math" "make_c_math" "")
math_test 256 $(exe "make_c_math" "make_c_math" "256")

#Clear critical_faul_val
let "critical_faul_val = 0"

#mpm disable log
pr 0 "bash" "disable log"
pr 0 "bash_c" "disable log"
pr 0 "bash_c_math" "disable log"
pr 0 "bash_cpp" "disable log"

pr 0 "make" "disable log"
pr 0 "make_c" "disable log"
pr 0 "make_c_math" "disable log"
pr 0 "make_cpp" "disable log"

critical_faul $critical_faul_val

#mpm pr_run
pr_run "bash"
pr_run "bash_c"
pr_run "bash_c_math"
pr_run "bash_cpp"

pr_run "make"
pr_run "make_c"
pr_run "make_c_math"
pr_run "make_cpp"

#Not supported yet
#critical_faul $critical_faul_val

#mpm run_test
run_test 0 "bash_c" ""
run_test 0 "bash_c_math" ""
run_test 0 "bash_cpp" ""

run_test 0 "make_c" ""
run_test 0 "make_c_math" ""
run_test 0 "make_cpp" ""

#mpm hello world test
hello_world_test $(exe "bash_c" "bash_c" "")
hello_world_test $(exe "bash_cpp" "bash_cpp" "")

hello_world_test $(exe "make_c" "make_c" "")
hello_world_test $(exe "make_cpp" "make_cpp" "")

#mpm math test
math_test 4 $(exe "bash_c_math" "bash_c_math" "4")
math_test 64 $(exe "bash_c_math" "bash_c_math" "")
math_test 256 $(exe "bash_c_math" "bash_c_math" "256")

math_test 4 $(exe "make_c_math" "make_c_math" "4")
math_test 64 $(exe "make_c_math" "make_c_math" "")
math_test 256 $(exe "make_c_math" "make_c_math" "256")

#mpm Failed to open project file
mpm 111 "disable"

#mpm No arguments
pr 83 "bash" "disable"

#mpm Invalid functionality name
pr 130 "bash" "disable invalid"

#Clear critical_faul_val
let "critical_faul_val = 0"

#mpm disable test
pr 0 "bash" "disable test"
pr 0 "bash_c" "disable test"
pr 0 "bash_c_math" "disable test"
pr 0 "bash_cpp" "disable test"

pr 0 "make" "disable test"
pr 0 "make_c" "disable test"
pr 0 "make_c_math" "disable test"
pr 0 "make_cpp" "disable test"

critical_faul $critical_faul_val

#mpm pr_run
pr_run "bash"
pr_run "bash_c"
pr_run "bash_c_math"
pr_run "bash_cpp"

pr_run "make"
pr_run "make_c"
pr_run "make_c_math"
pr_run "make_cpp"

critical_faul $critical_faul_val

#mpm run_test
run_test 0 "bash_c" ""
run_test 0 "bash_c_math" ""
run_test 0 "bash_cpp" ""

run_test 0 "make_c" ""
run_test 0 "make_c_math" ""
run_test 0 "make_cpp" ""

#mpm hello world test
hello_world_test $(exe "bash_c" "bash_c" "")
hello_world_test $(exe "bash_cpp" "bash_cpp" "")

hello_world_test $(exe "make_c" "make_c" "")
hello_world_test $(exe "make_cpp" "make_cpp" "")

#mpm math test
math_test 4 $(exe "bash_c_math" "bash_c_math" "4")
math_test 64 $(exe "bash_c_math" "bash_c_math" "")
math_test 256 $(exe "bash_c_math" "bash_c_math" "256")

math_test 4 $(exe "make_c_math" "make_c_math" "4")
math_test 64 $(exe "make_c_math" "make_c_math" "")
math_test 256 $(exe "make_c_math" "make_c_math" "256")

#Clear critical_faul_val
let "critical_faul_val = 0"

#mpm add template
pr 0 "bash" "add template ../../../templates/c.dtk"
pr 0 "bash" "add template ../../../templates/c_cpp_templates_collection.dtk -n cpp"

pr 0 "make" "add template ../../../templates/c.dtk"
pr 0 "make" "add template ../../../templates/c_cpp_templates_collection.dtk -n cpp"

critical_faul $critical_faul_val

#mpm pr_run
pr_run "bash"
pr_run "make"

critical_faul $critical_faul_val

#mpm Excepted argument after -n
pr 83 "bash" "add template -n"

#mpm Excepted argument after 'add'
mpm 130 "add"

#Clear critical_faul_val
let "critical_faul_val = 0"

#mpm add module
pr 0 "bash" "add module -n module_c -t c"
pr 0 "bash" "add module -n module_cpp -t cpp"

pr 0 "make" "add module -n module_c -t c"
pr 0 "make" "add module -n module_cpp -t cpp"

critical_faul $critical_faul_val

#mpm pr_run
pr_run "bash"
pr_run "make"

critical_faul $critical_faul_val

#mpm run test
run_mod_test 0 "bash" "module_c" ""
run_mod_test 0 "bash" "module_cpp" ""

run_mod_test 0 "make" "module_c" ""
run_mod_test 0 "make" "module_cpp" ""

#mpm hello world test
hello_world_test $(exe "bash" "bin/module_c" "")
hello_world_test $(exe "bash" "bin/module_cpp" "")

hello_world_test $(exe "make" "bin/module_c" "")
hello_world_test $(exe "make" "bin/module_cpp" "")

#mpm Project is non-modular
pr 130 "bash_c" "add module"

#mpm Excepted argument after -t
pr 83 "bash" "add module -t"

#mpm Template already setted
pr 83 "bash" "add module -t a -t b"

#mpm Excepted argument after -n
pr 83 "bash" "add module -n"

#mpm Name already setted
pr 83 "bash" "add module -n name1 -n name2"

#mpm Name has not been specified
pr 83 "bash" "add module"

#mpm Template has not been specified
pr 83 "bash" "add module -n name"

#mpm Module named '' already exists
pr 83 "bash" "add module -n module_c -t c"

#Clear critical_faul_val
let "critical_faul_val = 0"

#Exit mpm
cd ..

#Summary
echo Summary $test_passed / $test_count

#Exit test
cd ..