name := project

libs := -llog++ -lcommon++ -lml -lcommon -llog
std := -std=c++2a

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
	@$(CXX) -o $@ $^ -L ../lib $(libs) $(std)

$(bin):bin/%.o:src/%
	@echo Compiling $^
	@$(CXX) -c -o $@ $^ $(std) -I headers -I ../includes