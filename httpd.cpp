#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <vector>
#include "httpd.h"
#include "HandleTCPClient.hpp"
#include "util.h"
#include <string.h>
#include <errno.h>

#include <unordered_map>


using namespace std;

static const int MAXPENDING = 5;



void *ThreadMain(void *arg); // Main program of a thread

// Structure of arguments to pass to client thread
struct ThreadArgs {
  int clntSock; // Socket descriptor for client
};

string public_doc_root;

void start_httpd(unsigned short port, string doc_root)
{
	cerr << "Starting server (port: " << port <<
		", doc_root: " << doc_root << ")" << endl;

	public_doc_root = doc_root;

	int servSock; // socket descripter for server
	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		DieWithErr("socket() failed");
	}

	// Construct local address structure
 	struct sockaddr_in servAddr;                  // Local address
 	memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
 	servAddr.sin_family = AF_INET;                // IPv4 address family
 	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
 	servAddr.sin_port = htons(port);          // Local port

	 // Bind
	if (::bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
	 	DieWithErr("bind() failed");

	// listen
	if (::listen(servSock, MAXPENDING) < 0)
		DieWithErr("listen() failed");


    for (;;) {
	    struct sockaddr_in clntAddr; // Client address
	    // Set length of client address structure (in-out parameter)
	    socklen_t clntAddrLen = sizeof(clntAddr);

	    // Wait for a client to connect
	    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);

	    if (clntSock < 0)
	    	DieWithErr("accept() failed");


	    // set receive timeout as 5 seconds.
	    struct timeval timeout;
	    timeout.tv_sec = 5;
        timeout.tv_usec = 0;

	    if (setsockopt(clntSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
    		   sizeof(timeout)) < 0) {
	    	DieWithErr("set timeout failed");
	    }

	    if (setsockopt(clntSock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
    		   sizeof(timeout)) < 0) {
	    	DieWithErr("set timeout failed");
	    }


	    // Create separate memory for client argument
	    struct ThreadArgs *threadArgs = (struct ThreadArgs *) malloc(
	        sizeof(struct ThreadArgs));
	    if (threadArgs == NULL)
	      DieWithErr("malloc() failed");
	    threadArgs->clntSock = clntSock;

	    // Create client thread
	    pthread_t threadID;
	    int returnValue = pthread_create(&threadID, NULL, ThreadMain, threadArgs);

	    if (returnValue != 0)
	      DieWithErr("pthread_create() failed");

	    printf("with thread %ld\n", (long int) threadID);
	    // HandleTCPClient(clntSock, doc_root);
	}
	// NOT REACHED
	close(servSock);
}



void *ThreadMain(void *threadArgs) {
  // Guarantees that thread resources are deallocated upon return
  pthread_detach(pthread_self());

  // Extract socket file descriptor from argument
  int clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;
  free(threadArgs); // Deallocate memory for argument

  HandleTCPClient(clntSock, public_doc_root);

  return (NULL);
}






