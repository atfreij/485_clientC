/* Andy White
 * ECE484 Senior Design
 * Car Rider Management System
 * client_main.c
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <termios.h>
#include "client_macros.h"
#include "client_prototypes.h"

#define RING_BUFFER_SIZE (4)

int main(int argc, char** argv) {
  struct sockaddr_in server;
  struct termios serial_uart_options;
  char rx_buffer[RX_BUFFER_SIZE];
  char* prepared_str;
  char packaged_str[RX_BUFFER_SIZE+5];
  char recent_arrivals[RING_BUFFER_SIZE][RX_BUFFER_SIZE];
  unsigned short ring_counter=0;
  
  if(argc<2){
    printf("Enter IP\n");
    return SOME_SORT_OF_ERROR;
  }

  int new_socket;

  int rfid_uart_stream=serial_setup(&serial_uart_options);

 begin:
  for(;;){
    //Wait for and read data from RFID reader
    if(serial_rx(rfid_uart_stream, rx_buffer, RX_BUFFER_SIZE)==SOME_SORT_OF_ERROR){
      fprintf(stderr, "Error with serial communications.\n");
      _exit(-1);
    }

    //Check if vehicle was recently scanned
    for(int i=0; i<RING_BUFFER_SIZE; i++){
      if(strcmp(recent_arrivals[i], rx_buffer)==0){
	//If it was, restart scan
	goto begin;
      }
    }
    
    //If not, store new arrival
    strcpy(recent_arrivals[ring_counter], rx_buffer);
    if(++ring_counter == RING_BUFFER_SIZE){
      ring_counter=0;
    }

    //Prepare and package the RFID data
    printf("%s\n",rx_buffer);
    prepared_str=serial_prepare_data(rx_buffer);
    printf("Prepared data\n");
    printf("%s\n",prepared_str);
    serial_package_data(prepared_str,packaged_str);
    printf("%s\n",packaged_str);
        
    //Connect to network
    // new_socket=network_set_up(&server, argv[1]);
    // if(network_connect(&server, new_socket)==SOME_SORT_OF_ERROR){
    //  fprintf(stderr, "Error connecting to server.\n");
    //  _exit(-1);
    // }
    //
    // send(new_socket, packaged_str, strlen(packaged_str), 0);
    //  
    // network_close(new_socket);
    printf("Network closed.\n");    
  }
  
  return (EXIT_SUCCESS);
}
