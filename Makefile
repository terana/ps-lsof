CXX=g++
CXXFLAGS=-Wall -Werror --std=c++14

all: ps

ps: ps.cpp proc.h proc.cpp
	$(CXX)  ps.cpp proc.cpp $(CXXFLAGS) -o ps

run_docker:
	docker build -t ps-lsof . && docker run -it --rm ps-lsof bash

clean:
	rm -f $(OBJS) ps
