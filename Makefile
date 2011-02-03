CFLAGS = -Wall -std=c99 -ggdb
CXXFLAGS = -Wall -ggdb


targets: critbit.pdf critbit.o critbit-test

critbit-test: critbit-test.o critbit.o
	$(CXX) -o $@ $(CXXFLAGS) $^

critbit-test.o: critbit-test.cc
	$(CXX) -o $@ $(CXXFLAGS) -c $<

critbit.pdf: critbit.w
	cweave critbit.w
	pdftex critbit.tex

critbit.c: critbit.w
	ctangle critbit.w

critbit.o: critbit.c
	ctangle critbit.w
	$(CC) -Wall -c critbit.c -std=c99 -ggdb

