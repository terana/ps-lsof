CXX=g++
CXXFLAGS=-Wall -Werror --std=c++14

all: ps lsof hide_cmd

ps: ps.cpp proc.h proc.o
	$(CXX)  ps.cpp proc.o $(CXXFLAGS) -o ps

lsof: lsof.cpp proc.h proc.o
	$(CXX)  lsof.cpp proc.o $(CXXFLAGS) -o lsof

hide_cmd: hide_cmdline.cpp proc.h proc.o
	$(CXX)  hide_cmdline.cpp proc.o $(CXXFLAGS) -o hide-cmd

proc.o: proc.h proc.cpp
	$(CXX)  proc.cpp $(CXXFLAGS) -c -o proc.o

run_docker:
	docker build -t ps-lsof . && docker run -it --rm ps-lsof bash

clean:
	rm -f $(OBJS) ps
