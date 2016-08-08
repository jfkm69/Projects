#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>

#define MAXCONN 1000
#define BYTES 1024

char *ROOT;
int listenfd, clients[MAXCONN];
void error(char *);
void startWebServer(char *);
void respond(int);

int main(int argc, char *argv[])
{
	struct sockaddr_in clientaddr; // Stores the location of the address of the client
	socklen_t addrlen; // The unsigned length of the socket
	char c; // This is used to get the command line arguments

	//Create a default path and PORT
	char PORT[6];
	ROOT = getenv("PWD"); // Get the current path
	strcpy(PORT, "10000"); //Set the default port as 10000

	int slot = 0; // This is used to refer to an element of clients ie a certain connection

	//Parse through the command line arguments
	while ((c = getopt (argc, argv, "p:r:")) != -1)
		switch (c){
			case 'r':
			ROOT = (char*) malloc(strlen(optarg));
			strcpy(ROOT, optarg);
			break;
			case 'p':
			strcpy(PORT, optarg);
			break;
			case '?':
			exit (1);
			default:
			exit(1);
		}
		//Color codes for C "033[92m" - GREEN, "033[94m" - BLUE, "033[0m" - END
		printf("Server started on port no. %s%s%s with root directory %s%s%s\n", "\033[92m", PORT, "\033[0m", "\033[92m", ROOT, "\033[0m" );
		//Set all the elements as -1 to show that there is no one connected to the server
		int i;
		for (i = 0; i < MAXCONN; i++)
			clients[i] = -1;
		//Start the server
		startWebServer(PORT);
		// createHttpServer();	

		//ACCEPT connections
		while(1){
			addrlen = sizeof(clientaddr);
			clients[slot] = accept(listenfd, (struct sockaddr *) &clientaddr, &addrlen);

			if (clients[slot] < 0)
				perror("server: accept");
			else {
				if (fork() == 0)
				{
					respond(slot);
					exit(0);
				}
			}
			while (clients[slot] != -1) slot = (slot+1)%MAXCONN; //If client occupies slot move to the next slot
		}
	return 0;
}


//Start the web server
void startWebServer(char *port){
	struct addrinfo hints, *res, *p;

	//get the address information of the host
	memset (&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; //Desired family address of the desired address ie IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // returns an address suitable for binding socket and accepting conncetions...AI_PASSIVE retruns wildcard ip
	if (getaddrinfo(NULL, port, &hints, &res) != 0){ //Retuns one or more addressinfo structures
		perror("getaddrinfo() error");
		exit(1);
	}
	//Socket and bind
	for (p = res; p!=NULL; p=p->ai_next){
		listenfd = socket(p->ai_family, p->ai_socktype, 0);
		if (listenfd == -1) continue;
		if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
	} 
	if (p==NULL){
		perror("socket() or bind()");
		exit(1);
	}

	freeaddrinfo(res);

	//listen for incoming connections
	if (listen(listenfd, 1000000) != 0){
		perror("listen() error");
		exit(1);
	}
}

//Respond to the specific client connection
void respond(int n){
	char message[99999], *requestLine[3], dataToSend[BYTES], path[99999];
	int received, forward, bytes_read;

	memset( (void*)message, (int)'\0', 99999); //Set the char[] to void and fill with nulls

	received = recv(clients[n], message, 99999, 0);//receive message from socket (socketfd(socket descriptor containing address information, buffer, bufferSize, flags)

	if (received < 0) //Receive error
		fprintf(stderr, ("recv() error\n"));
	else if (received == 0) //Receive socket closed
		fprintf(stderr, "Client disconnected unexplectedly.\n");
	else { // Message received
		printf("%s", message);
		requestLine[0] = strtok (message, " \t\n");//strtok breaks a string into tokens
		//If we are dealing with a GET request or a POST request
		if (strncmp(requestLine[0], "GET\0", 4) == 0 || strncmp(requestLine[0], "POST\0", 5) == 0){ //strncmp(3) compare 2 string, (string1, string2, stringlength)
			requestLine[1] = strtok(NULL, " \t");
			requestLine[2] = strtok(NULL, " \t\n");
			//Check if the HTTP protocol is 1.0 or 1.1 else return 404
			if (strncmp(requestLine[2], "HTTP/1.0", 8) != 0 && strncmp(requestLine[2], "HTTP/1.1", 8) != 0){
				write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
			} else {
				//Check if the path already contains '\\'
				if (strncmp(requestLine[1], "//\0", 3) == 0)
					requestLine[1] = (char*) "/";
				//Check for the path of the file
				if (strncmp(requestLine[1], "/\0", 2) == 0){
					requestLine[1] = (char*) "/index.html"; //If no file is specified then go to index.html
				}
				strcpy(path, ROOT);
				strcpy(&path[strlen(ROOT)], requestLine[1]);
				printf("file: %s\n", path);

				if ((forward = open(path, O_RDONLY)) != -1) //The specified file has been found
				{
					send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
					while ((bytes_read = read(forward, dataToSend, BYTES)) > 0)
						write(clients[n], dataToSend, bytes_read);
				} else {
					write(clients[n], "HTTP/1.0 404 Not Found\n", 23); //The file was not found
					write(clients[n], "<html><body><h1>404 File Not Found</h1></body></html>\n", 54);
				}
			}
		}
	}
	//Close the socket on completion
	shutdown(clients[n], SHUT_RDWR); //Disable all send and receive operations
	close(clients[n]);
	clients[n]=-1;
}