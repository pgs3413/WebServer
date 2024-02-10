dir = .

include globalfile

des = all

all:
	@echo "nothing to do"

.PHONY : test
test:
	@cd ./test && make $(des)

clean:
	rm -r ./test/bin/