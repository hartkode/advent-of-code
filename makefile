include config.mk

all: bin/day01

bin/%: obj/%.o | bin
	c++ $(LDFLAGS) $^ -o $@

obj/%.o: src/%.cpp | obj
	c++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf bin obj

bin obj:
	mkdir $@

.PHONY: all clean
