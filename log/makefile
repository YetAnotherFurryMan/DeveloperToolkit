name := log

bin := $(patsubst src/%,bin/%.o,$(wildcard src/*))

all: clean dir $(name).so lib$(name).a

clean:
	@echo Cleaning $(name)
	@rm -f bin/*
	@rm -f *.a *.so

dir: bin

bin:
	@mkdir $@

$(name).so: $(bin)
	@echo Creating $@
	@$(LD) -shared -o $@ $^

lib$(name).a: $(bin)
	@echo Creating $@
	@ar crv $@ $^

$(bin):bin/%.o:src/%
	@echo Compiling $^
	@$(CC) -c -o $@ $^ -I headers -I ../includes -fPIC