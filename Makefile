CXX = g++
CXXFLAGS = -g -O0 -Wall -std=c++11 -I/usr/local/vcpkg/installed/x64-linux/include -DDEBUG=1
LDFLAGS = -lpthread -lgtest_main -lgtest -L/usr/local/vcpkg/installed/x64-linux/lib -lboost_graph
RM = gio trash -f

TARGETS = bitmap_test weighted_matching_example tsa_example

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGETS)

bitmap_test: bitmap_test.o
	-$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

weighted_matching_example: weighted_matching_example.cpp
	-$(CXX) $^ -o $@ -O3 -Wall -I/usr/local/vcpkg/installed/x64-linux/include -L/usr/local/vcpkg/installed/x64-linux/lib -lboost_graph

tsa_example: tsa_example.o 
	-$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)  

clean:
	-$(RM) $(TARGETS) $(OBJS)