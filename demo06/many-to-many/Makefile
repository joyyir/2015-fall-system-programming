all : app

app : chat_server.o chat_client.o practical.o
	gcc -o chat_server chat_server.o practical.o
	gcc -o chat_client chat_client.o practical.o

chat_server.o : chat_server.c
	gcc -c chat_server.c

chat_client.o : chat_client.c
	gcc -c chat_client.c

practical.o : practical.c
	gcc -c practical.c

clean :
	rm *.o chat_server chat_client
