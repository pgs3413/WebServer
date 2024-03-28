dir = ../src

include ../globalfile

target = $(des)
bin = ./bin
args = --help
platform = $(os)

all:
	@echo "test nothing to do"

$(target): cmd
	$(cc) -D$(platform) $(obj) test_$@.cpp -std=c++11 -o $(bin)/$@ ${option} 
	cd $(bin) && ./$@ $(args)

cmd:
	@mkdir -p bin