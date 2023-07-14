name := dtkmlc

libs := -L ../lib -llog++ -lml -lcommon++
std := -std=c++2a

bin := $(patsubst src/%,bin/%.o,$(wildcard src/*.cpp))

.PHONY: all
all: dirs clean $(name)

dirs: bin

bin:
	@mkdir $@ 2> /dev/null

clean:
	@echo Cleaning $(name)
	@rm -f bin/*
	@rm -f $(name)

$(name): $(bin)
	@echo Linking $(name)
	@$(CXX) -o $@ $^ $(libs) $(std)

bin/%.o:src/%
	@echo Compiling $^
	@$(CXX) -c -o $@ $^ -I ../includes $(std)