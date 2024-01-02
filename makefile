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
	 bin/day15 \
	 bin/day16 \
	 bin/day17 \
	 bin/day18 \
	 bin/day19 \
	 bin/day20 \
	 bin/day21 \
	 bin/day22 \
	 bin/day23 \
	 bin/day24 \
	 bin/day25

bin:
	mkdir $@

bin/%: src/%.cpp | bin
	c++ $(CPPFLAGS) $^ -o $@

bin/day24: src/day24.cpp | bin
	c++ $(CPPFLAGS) -Wno-sign-conversion -I/usr/local/include $^ -L/usr/local/lib -lz3 -o $@

clean:
	rm -rf bin

.PHONY: all clean
