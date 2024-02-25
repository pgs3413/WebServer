dir = ../src

include ../globalfile

target = $(des)
bin = ./bin
args = --help

all:
	@echo "test nothing to do"

$(target): cmd
	$(cc) $(obj) test_$@.cpp -o $(bin)/$@ ${option}
	cd $(bin) && ./$@ $(args)

cmd:
	@mkdir -p bin