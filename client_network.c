/* Andy White
 * ECE484 Senior Design
 * Car Rider Management System
 * client_network.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "client_macros.h"
#include "client_prototypes.h"

/* This function takes a socket address and returns a socket descriptor after setting 
 * up the network. 
 * Return value: socket descriptor, if successful. -1, if error. 
 */
int network_set_up(struct sockaddr_in* server, char* address){
  int new_socket;
  
  memset(server, 0, sizeof(struct sockaddr)); //Clear struct completely
    
  server->sin_family=AF_INET;
  server->sin_addr.s_addr=inet_addr(address);
  server->sin_port=htons(PORTNUM);

  new_socket=socket(AF_INET, SOCK_STREAM, 0); //Create new socket
  if(new_socket==SOME_SORT_OF_ERROR){
    perror("socket");
    return SOME_SORT_OF_ERROR;
  }

  return new_socket;
}

int network_connect(struct sockaddr_in* server, int network_socket){
  printf("Attempting connection...");
  if(connect(network_socket, (struct sockaddr *)server, sizeof(struct sockaddr))==-1){
    printf("Failed to connect.\n");
    return SOME_SORT_OF_ERROR;
  }
    
  printf("Connected\n");
  return 0;
}

/*This function simply closes the socket descriptor. 
 * Return value: 0, if successful. -1, if error. 
 */
int network_close(int network_socket){
  if(close(network_socket)==SOME_SORT_OF_ERROR){
    perror("close");
    return SOME_SORT_OF_ERROR;
  }
    
  return 0;
}

int network_shutdown(int network_socket){
  if(shutdown(network_socket,2)==SOME_SORT_OF_ERROR){
    perror("shutdown");
    return SOME_SORT_OF_ERROR;
  }

  return 0;
}

