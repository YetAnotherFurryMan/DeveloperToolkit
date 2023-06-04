name := log++

std := -std=c++2a

bin := $(patsubst src/%,bin/%.o,$(wildcard src/*))

all: clean dir $(name).so lib$(name).a

clean:
	@echo Cleaning $(name)
	@rm -f bin/*
	@rm -f *$(name)*

dir: bin

bin:
	@mkdir $@

$(name).so: $(bin)
	@echo Creating $@
	@$(CXX) -shared -o $@ $^ $(std)

lib$(name).a: $(bin)
	@echo Creating $@
	@ar crv $@ $^

$(bin):bin/%.o:src/%
	@echo Compiling $^
	@$(CXX) -c -o $@ $^ $(std) -I headers -I ../includes -fPIC