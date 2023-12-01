include config.mk

all: bin/01a bin/01b

bin/01a: obj/01a.o | bin
	c++ $(LDFLAGS) $^ -o $@

bin/01b: obj/01b.o | bin
	c++ $(LDFLAGS) $^ -o $@

obj/%.o: src/%.cpp | obj
	c++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf bin obj

bin obj:
	mkdir $@

.PHONY: all clean
