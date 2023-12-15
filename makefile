include config.mk

all: bin/day01 \
	 bin/day02 \
	 bin/day03 \
	 bin/day04 \
	 bin/day05 \
	 bin/day06 \
	 bin/day07 \
	 bin/day08 \
	 bin/day09 \
	 bin/day10 \
	 bin/day11 \
	 bin/day12 \
	 bin/day13 \
	 bin/day14 \
	 bin/day15

bin:
	mkdir $@

bin/%: src/%.cpp | bin
	c++ $(CPPFLAGS) $^ -o $@

clean:
	rm -rf bin

.PHONY: all clean
