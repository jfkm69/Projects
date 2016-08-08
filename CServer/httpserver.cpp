#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "httpserver.h"

void createHttpServer(){
	int createSocket, newSocket;
	int setPortTimeout = 1;
	socklen_t addrlen;
	int len;
	int bufferSize = 1024;
	char *buffer = (char*) malloc(bufferSize);//malloc - Allocates memory and returns memory location
	struct sockaddr_in address; // stores information about the device we wish to connect

	createSocket = socket(AF_INET, SOCK_STREAM, 0); //socket(AF_INET, SOCK_STREAM, 0) 
	//This allows for the reuse of the same port without having to wait for the port to be freed
	setsockopt(createSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&setPortTimeout, sizeof(setPortTimeout));

	if (createSocket > 0){ 
		printf("The socket was created.\n");
	}

	memset(&address, 0, sizeof(address)); //Zero the struct
	address.sin_family = AF_INET;
	// address.sin_addr.s_addr = INADDR_ANY;
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //Sets the destination address as localhost or 127.0.0.1
	address.sin_port = htons(10001);

	if (bind(createSocket, (struct sockaddr *) &address, sizeof(address)) == 0){
		printf("Binding socket\n");
	}

	while (1){
		if (listen(createSocket, 10) < 0){
			perror("server: listen");
			exit(1);
		}

		if ((newSocket = accept(createSocket, (struct sockaddr*) &address, &addrlen)) < 0){
			perror("server:accept");
			exit(1);
		}
		if (newSocket > 0)
			printf("Client is connceted...\n");

		recv(newSocket, buffer, bufferSize, 0);
		printf("%s\n", buffer);
		// write(newSocket, "Hello world\n", 12);
		write(newSocket, "HTTP/1.1 200 OK\n", 16);
		write(newSocket, "Content-length: 46", 19);
		write(newSocket, "Content-Type: text/html\n\n", 25);
		write(newSocket, "<html><body><H1>Hello world</H1></body></html>",46);
		close(newSocket);
	}

	// connect(createSocket, (struct sockaddr*) &address, sizeof(struct sockaddr));
	// len = recv(createSocket, buffer, bufferSize, 0);
	// buffer[len] = '\0';
	// printf("Received %s (%d bytes)\n", buffer, len);

	close(createSocket);
}