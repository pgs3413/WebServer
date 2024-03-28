dir = .

include globalfile

des = all
os = LINUX

all:
	@echo "nothing to do"

.PHONY : test
test:
	@cd ./test && make $(des)

clean:
	rm -r ./test/bin/