all: compile

ces:
	cs s ech
cec:
	cc c ech

cs:
	@g++ -std=c++11 -pthread -o server server.cpp
	echo "Oki Doki Protocol [SERVER] compiled successfully."
	echo "[ODP]: Ready to use"
cc:
	@g++ -std=c++11 -pthread -o client client.cpp
	echo "Oki Doki Protocol [CLIENT] compiled successfully."
	echo "[ODP]: Ready to use"
s:
	./server
c:
	./client
compile:
	@g++ -std=c++11 -pthread -o server server.cpp
	@g++ -std=c++11 -pthread -o client client.cpp
	@echo "Oki Doki Protocol compiled successfully."
	@echo "[ODP]: Ready to use"

ech:
	@echo "Oki Doki Protocol compiled successfully."
	@echo "[ODP]: Ready to use"
