CXX = g++
CXXFLAGS = -g -O0 -Wall
LDFALGS = 
RM = gio trash -f

TARGETS = bitmap_test

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)


bitmap_test: bitmap_test.o
	-$(CXX) $^ $@ $(CXXFLAGS) $(LDFALGS)

clean:
	-$(RM) $(TARGETS) $(OBJS)