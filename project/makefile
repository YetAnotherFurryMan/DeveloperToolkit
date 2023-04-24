name := project

libs := -llog -lfmt

bin := $(patsubst src/%,bin/%.o,$(wildcard src/*))

all: clean $(name)

clean:
	@echo Cleaning $(name)
	@rm -f bin/*
	@rm -f $(name)

$(name): $(bin)
	@echo Linking $(name)
	@$(CXX) -o $@ $^ -L ../lib $(libs) -std=c++2b

$(bin):bin/%.o:src/%
	@echo Compiling $^
	@$(CXX) -c -o $@ $^ -std=c++2b -I headers -I ../includes