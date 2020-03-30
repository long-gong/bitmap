CXX = g++
CXXFLAGS = -g -O0 -Wall  
LDFLAGS = -lpthread -lgtest_main -lgtest 
RM = gio trash -f

TARGETS = bitmap_test weighted_matching_example

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGETS)

bitmap_test: bitmap_test.o
	-$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

weighted_matching_example: weighted_matching_example.cpp
	-$(CXX) $^ -o $@ -O3 -Wall -I/usr/local/vcpkg/installed/x64-linux/include -L/usr/local/vcpkg/installed/x64-linux/lib -lboost_graph

clean:
	-$(RM) $(TARGETS) $(OBJS)