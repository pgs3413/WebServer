dir = ../src

include ../globalfile

target = $(des)
bin = ./bin

all:
	@echo "test nothing to do"

$(target): cmd
	$(cc) $(obj) $@.cpp -o $(bin)/$@ ${option}
	cd $(bin) && ./$@

cmd:
	@mkdir -p bin