all: run

run:
	g++ -std=c++11 -pthread -o server.exe server.cpp
	g++ -std=c++11 -pthread -o client.exe client.cpp
