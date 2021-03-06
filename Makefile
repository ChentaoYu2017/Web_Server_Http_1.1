CC=g++
CFLAGS=-ggdb -std=c++11 -Wall -Wextra -pedantic -Werror
DEPS = httpd.h HttpFramer.hpp HttpParser.hpp util.h HandleTCPClient.hpp
SRCS = httpd.cpp HandleTCPClient.cpp HttpFramer.cpp HttpParser.cpp util.cpp
MAIN_SRCS = main.c $(SRCS)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

default: httpd

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

httpd:    $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o httpd $(MAIN_OBJS) -lpthread

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f httpd *.o
	rm -rf httpd.dSYM