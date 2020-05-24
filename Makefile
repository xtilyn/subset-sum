.SUFFIXES: .txt .test
TXTS=$(shell find . -type f -name 'test*.txt' | sort )
TESTS=$(TXTS:.txt=.test)


a.out:	subset.cpp Makefile
	g++ -O2 -Wall subset.cpp

clean:
	rm -f *~ a.out

tests: 	 a.out $(TESTS)

.txt.test:
	@ printf "Testing " $<
	time ./a.out < $<


