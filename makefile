dir = ./src

include globalfile

des = all
os = LINUX
bin = ./build

all: cmd
	$(cc) -D$(os) $(obj) ${dir}/main.cpp -std=c++11 -o $(bin)/server ${option}

.PHONY : test
test:
	@cd ./test && make $(des)

cmd:
	@mkdir -p build

clean:
	rm -r ./test/bin/