all: CypherSystem

main.o: main.c system.h config.h server.h
	gcc -Wall -Wextra -pthread -c main.c

config.o: main.c config.h
	gcc -Wall -Wextra -pthread -c config.c

system.o: system.c main.c config.c
	gcc -Wall -Wextra -pthread -c system.c

server.o: server.c main.c system.c
	gcc -Wall -Wextra -pthread -c server.c

CypherSystem: main.o server.o system.o config.o
	gcc -Wall -Wextra -pthread main.o server.o system.o config.o -o  trinity.exe