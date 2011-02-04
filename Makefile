TARGETS = benchmark critbit-test libcritbit.a

PREFIX ?= /usr

SOURCES_C = \
	critbit.c

SOURCES_CXX = \
	critbit-test.cc \
	benchmark-data.cc \
	benchmark.cc

OBJECTS = $(SOURCES_C:.c=.o) $(SOURCES_CXX:.cc=.o)

CBOTHFLAGS += -Wall
CBOTHFLAGS += -O2
# CBOTHFLAGS += -ggdb
# CBOTHFLAGS += -pg
CFLAGS += $(CBOTHFLAGS) -std=c99
CXXFLAGS += $(CBOTHFLAGS)


all: $(TARGETS)

benchmark: benchmark.o benchmark-data.o critbit.o
#benchmark: benchmark.o benchmark-data.o critbit.o radix.o
	$(CXX) -o $@ $(CXXFLAGS) $^

critbit-test: critbit-test.o critbit.o
	$(CXX) -o $@ $(CXXFLAGS) $^

libcritbit.a: critbit.o
	$(AR) rs $@ $^

install: $(PREFIX)/lib/libcritbit.a $(PREFIX)/include/critbit.h

$(PREFIX)/lib/libcritbit.a: libcritbit.a
	install -p -D -m 0644 $< $@

$(PREFIX)/include/critbit.h: critbit.h
	install -p -D -m 0644 $< $@

uninstall:
	rm -f $(PREFIX)/lib/libcritbit.a $(PREFIX)/include/critbit.h

run-benchmark: benchmark FORCE
	@./benchmark 0 1000
	@./benchmark 1 1000
	@./benchmark 2 1000
	#@./benchmark 3 1000

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o : %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

critbit.pdf: critbit.w
	cweave critbit.w
	pdftex critbit.tex

critbit.c: critbit.w
	ctangle critbit.w

FORCE:

clean:
	rm -f $(TARGETS) $(OBJECTS)

