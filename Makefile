CC = g++
PROG = slit128cmd
CXXFLAGS = -g -O2 -Wall
# LDLIBS += -L/usr/local/lib/ -lmylib

all: $(PROG)

clean:
	rm -f *.o $(PROG)
