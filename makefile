include config.mk

all: $(patsubst 2020/src/%.cpp,2020/bin/%,$(wildcard 2020/src/*.cpp)) \
	$(patsubst 2022/src/%.cpp,2022/bin/%,$(wildcard 2022/src/*.cpp)) \
	$(patsubst 2023/src/%.cpp,2023/bin/%,$(wildcard 2023/src/*.cpp))

%/bin:
	mkdir -p $@

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

clean:
	rm -rf 2020/bin 2022/bin 2023/bin

.PHONY: all clean
