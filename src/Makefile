all: chat_server chat_client

chat_server: chat_server.o server.o
	g++ -g -o chat_server chat_server.o server.o
server.o: server.cpp server.h
	g++ -c server.cpp
chat_server.o: chat_server.cpp
	g++ -c chat_server.cpp

chat_client: chat_client.o client.o
	g++ -g -o chat_client chat_client.o client.o
client.o: client.cpp client.h
	g++ -c client.cpp
chat_client.o: chat_client.cpp
	g++ -c chat_client.cpp


