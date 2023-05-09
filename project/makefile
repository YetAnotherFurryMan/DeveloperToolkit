name := project

libs := -llog -lcommon

bin := $(patsubst src/%,bin/%.o,$(wildcard src/*))

all: clean dir $(name)

clean:
	@echo Cleaning $(name)
	@rm -f bin/*
	@rm -f $(name)

dir: bin

bin:
	@mkdir $@

$(name): $(bin)
	@echo Linking $(name)
	@$(CXX) -o $@ $^ -L ../lib $(libs) -std=c++2a

$(bin):bin/%.o:src/%
	@echo Compiling $^
	@$(CXX) -c -o $@ $^ -std=c++2a -I headers -I ../includes