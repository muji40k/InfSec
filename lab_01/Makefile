.PHONY: default build debug run drun test clean

SRC_DIR := src

FLAGS = 

FILES=$(shell find $(SRC_DIR) -type f)
CPP=$(shell find $(SRC_DIR) -type f -name "*.cpp")
DIRS=$(shell find $(SRC_DIR) -type d)
INCS=$(foreach dir, $(DIRS), -I$(dir))

default: run

.build%: clean
	rm -f .build*
	touch $@

build: .buildrelease a.out

debug: FLAGS += -g3
debug: .builddebug a.out

test:
	@echo $(INCS)

a.out: $(FILES)
	g++ $(FLAGS) $(INCS) $(CPP)

run: build
	./a.out

drun: debug
	./a.out

clean:
	rm -f *.out

