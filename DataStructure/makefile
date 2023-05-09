name := dtkds

bin := $(patsubst src/%,bin/%.o, $(wildcard src/*.c)) $(patsubst src/%,bin/%.c.o, $(wildcard src/*.l src/*.y))

.PHONY: all
all: clean dir $(name).so lib$(name).a test/test

dir: bin

bin:
	@mkdir $@

clean:
	@echo Cleaning $(name)
	@rm -f bin/*
	@rm -f src/*.l.c src/*.y.c
	@rm -f *.so *.a
	@rm -f test/test

$(name).so: $(bin)
	@echo Linking $@
	@$(CC) -shared -o $@ $^

lib$(name).a: $(bin)
	@echo Linking $@
	@ar crv $@ $^

bin/%.c.o: src/%.c
	@echo Compiling $^
	@$(CC) -c -o $@ $^ -fPIC

bin/%.l.c.o: src/%.l
	@echo Compiling $^
	@flex -o $^.c --header-file=headers/lexer.l.h --yylineno -f $^
	@$(CC) -c -o $@ $^.c -fPIC

bin/%.y.c.o: src/%.y
	@echo Generating $@
	@bison $^ -o $^.c --header=headers/parser.y.h -k
	@$(CC) -c -o $@ $^.c -fPIC -I headers

test/test: test/main.cpp
	@echo Test 1
	@$(CXX) -o $@ $^ -L . -ldtkds
	@./$@