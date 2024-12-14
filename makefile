include config.mk

all: $(patsubst 2015/src/%.cpp,2015/bin/%,$(wildcard 2015/src/*.cpp)) \
	$(patsubst 2020/src/%.cpp,2020/bin/%,$(wildcard 2020/src/*.cpp)) \
	$(patsubst 2022/src/%.cpp,2022/bin/%,$(wildcard 2022/src/*.cpp)) \
	$(patsubst 2023/src/%.cpp,2023/bin/%,$(wildcard 2023/src/*.cpp)) \
	$(patsubst 2024/src/%.cpp,2024/bin/%,$(wildcard 2024/src/*.cpp))

%/bin:
	mkdir -p $@

# 2015
2015/bin/%: 2015/src/%.cpp | 2015/bin
	c++ $(CPPFLAGS) $^ -o $@

2015/bin/day04: 2015/src/day04.cpp | 2015/bin
	c++ $(CPPFLAGS) $^ -lmd -o $@

# 2020
2020/bin/%: 2020/src/%.cpp | 2020/bin
	c++ $(CPPFLAGS) $^ -o $@

# 2022
2022/bin/%: 2022/src/%.cpp | 2022/bin
	c++ $(CPPFLAGS) $^ -o $@

# 2023
2023/bin/%: 2023/src/%.cpp | 2023/bin
	c++ $(CPPFLAGS) $^ -o $@

2023/bin/day24: 2023/src/day24.cpp | 2023/bin
	c++ $(CPPFLAGS) -Wno-sign-conversion -I/usr/local/include $^ -L/usr/local/lib -lz3 -o $@

# 2024
2024/bin/%: 2024/src/%.cpp | 2024/bin
	c++ $(CPPFLAGS) $^ -o $@

2024/bin/day13-z3: 2024/src/day13-z3.cpp | 2024/bin
	c++ $(CPPFLAGS) $^ -L/usr/local/lib -lz3 -o $@

clean:
	rm -rf 2015/bin 2020/bin 2022/bin 2023/bin 2024/bin

.PHONY: all clean
