TARGETS = benchmark critbit-test

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
CFLAGS += $(CBOTHFLAGS) -std=c99
CXXFLAGS += $(CBOTHFLAGS)


all: $(TARGETS)

FORCE:

benchmark: benchmark.o benchmark-data.o critbit.o
#benchmark: benchmark.o benchmark-data.o critbit.o radix.o
	$(CXX) -o $@ $(CXXFLAGS) $^

critbit-test: critbit-test.o critbit.o
	$(CXX) -o $@ $(CXXFLAGS) $^

run-benchmark: benchmark FORCE
	@./benchmark 0 1000
	@./benchmark 1 1000
	#@./benchmark 2 1000

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o : %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

critbit.pdf: critbit.w
	cweave critbit.w
	pdftex critbit.tex

critbit.c: critbit.w
	ctangle critbit.w

clean:
	rm -f $(TARGETS) $(OBJECTS)

