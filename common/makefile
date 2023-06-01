name := common

binpp := $(patsubst src/%,bin/%.o,$(wildcard src/*.cpp))
binc := $(patsubst src/%,bin/%.o,$(wildcard src/*.c))

all: clean dir $(name).so lib$(name).a c$(name).so libc$(name).a

clean:
	@echo Cleaning $(name)
	@rm -f bin/*
	@rm -f $(name)

dir: bin

bin:
	@mkdir $@

$(name).so: $(binpp)
	@echo Creating $@
	@$(CXX) -shared -o $@ $^ -std=c++2a -I headers -I ../includes

lib$(name).a: $(binpp) $(binc)
	@echo Creating $@
	@ar crv $@ $^

$(binpp):bin/%.o:src/%
	@echo Compiling $^
	@$(CXX) -c -o $@ $^ -std=c++2a -I headers -I ../includes -fPIC

c$(name).so: $(binc)
	@echo Creating $@
	@$(CC) -shared -o $@ $^ -I headers -I ../includes

libc$(name).a: $(binc)
	@echo Creating $@
	@ar crv $@ $^

$(binc):bin/%.o:src/%
	@echo Compiling $^
	@$(CC) -c -o $@ $^ -I headers -I ../includes -fPIC