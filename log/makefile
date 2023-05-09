name := log

bin := $(patsubst src/%,bin/%.o,$(wildcard src/*))

all: clean dir $(name).so lib$(name).a

clean:
	@echo Cleaning $(name)
	@rm -f bin/*
	@rm -f $(name)

dir: bin

bin:
	@mkdir $@

$(name).so: $(bin)
	@echo Creating $@
	@$(CXX) -shared -o $@ $^ -std=c++2a -I jeaders -I ../includes

lib$(name).a: $(bin)
	@echo Creating $@
	@ar crv $@ $^

$(bin):bin/%.o:src/%
	@echo Compiling $^
	@$(CXX) -c -o $@ $^ -std=c++2a -I headers -I ../includes -fPIC