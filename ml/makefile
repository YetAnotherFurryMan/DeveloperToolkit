name := ml

bin := $(patsubst src/%,bin/%.c.o, $(wildcard src/*.y src/*.l)) $(patsubst src/%,bin/%.o, $(wildcard src/*.c))

.PHONY: all
all: clean dir $(name).so lib$(name).a

dir: bin

bin:
	@mkdir $@

clean:
	@echo Cleaning $(name)
	@rm -f bin/*
	@rm -f src/*.l.c headers/*.l.h
	@rm -f src/*.y.c headers/*.y.h
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
	@$(CC) -c -o $@ $^ -fPIC -I ../includes

bin/%.l.c.o: src/%.l
	@echo Compiling $^
	@flex -o $^.c --header-file=headers/lexer.l.h --yylineno --prefix=ml_yy -f $^
	@$(CC) -c -o $@ $^.c -fPIC -I ../includes

bin/%.y.c.o: src/%.y
	@echo Compiling $@
	@bison $^ -o $^.c --defines=headers/parser.y.h -k
	@$(CC) -c -o $@ $^.c -fPIC -I headers -I ../includes

test/test: test/main.cpp
	@echo Test ml
	@$(CXX) -o $@ $^ -L . -l$(name) -L ../lib -llog
	@./$@ $@.dtk $@.1.out.dtk
	@./$@ $@.1.out.dtk $@.2.out.dtk
	@if cmp -s "$@.1.out.dtk" "$@.2.out.dtk"; then echo "Passed"; else echo "Failed"; fi