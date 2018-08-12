CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-include std.h -std=c++17 -ggdb
LDFLAGS=-g
LDLIBS=

SRCS=$(wildcard *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

all: std.h.gch tool

tool: $(OBJS)
	$(CXX) $(OBJS) -o tool
std.h.gch: std.h
	$(CXX) $(CPPFLAGS) std.h

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)
	$(RM) std.h.gch

distclean: clean
	$(RM) *~ .depend

include .depend