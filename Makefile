CLIENT_OBJS = client.o tcp_client.o
OBJS = log.o test.o
CC = g++
FLAGS = -Wall -g3

all: test

test: $(CLIENT_OBJS) $(OBJS)
	$(CC) -o test $(CLIENT_OBJS) $(OBJS)

tcp_client.o: tcp_client.cpp tcp_client.h client.h
	$(CC) $(FLAGS) -c tcp_client.cpp

client.o: client.cpp client.h log.h
	$(CC) $(FLAGS) -c client.cpp

test.o: test.cpp tcp_client.h log.h
	$(CC) $(FLAGS) -c test.cpp

log.o: log.cpp log.h
	$(CC) $(FLAGS) -c log.cpp

clean:
	rm -f $(CLIENT_OBJS) $(OBJS) test
