CLIENT_OBJS = client.o tcp_client.o run_client.o
SERVER_OBJS = server.o tcp_server.o run_server.o
OBJS = log.o util.o
CC = g++
FLAGS = -Wall -g3

all: client server

client: $(CLIENT_OBJS) $(OBJS)
	$(CC) -o client $(CLIENT_OBJS) $(OBJS)

server: $(SERVER_OBJS) $(OBJS)
	$(CC) -o server $(SERVER_OBJS) $(OBJS) -lpthread


tcp_client.o: tcp_client.cpp tcp_client.h client.h
	$(CC) $(FLAGS) -c tcp_client.cpp

client.o: client.cpp client.h log.h
	$(CC) $(FLAGS) -c client.cpp

run_client.o: run_client.cpp tcp_client.h log.h
	$(CC) $(FLAGS) -c run_client.cpp


tcp_server.o: tcp_server.cpp tcp_server.h server.h log.h
	$(CC) $(FLAGS) -c tcp_server.cpp

server.o: server.cpp server.h log.h util.h
	$(CC) $(FLAGS) -c server.cpp

run_server.o: run_server.cpp tcp_server.h log.h
	$(CC) $(FLAGS) -c run_server.cpp


log.o: log.cpp log.h
	$(CC) $(FLAGS) -c log.cpp

util.o: util.cpp util.h
	$(CC) $(FLAGS) -c util.cpp

clean:
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS) $(OBJS) client server
