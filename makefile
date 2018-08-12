CC=gcc
CXX=clang++
RM=rm -f
CPPFLAGS=-include std.hpp -std=c++17 -ggdb -Wall -Wextra
LDFLAGS=-g
LDLIBS=

SRCS=$(wildcard *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

all: std.hpp.gch tool

tool: $(OBJS)
	$(CXX) $(OBJS) -o tool
std.hpp.gch: std.hpp
	$(CXX) $(CPPFLAGS) std.hpp

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)
	$(RM) std.hpp.gch

distclean: clean
	$(RM) *~ .depend

include .depend