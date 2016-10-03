/* Andy White
 * ECE484 Senior Design
 * Car Rider Management System
 * serial.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "client_macros.h"
#include "client_prototypes.h"

#define BAUD_RATE B9600
#define SERIAL_PORT "/dev/ttyAMA0"

int serial_setup(struct termios* serial_uart_options){
  int rfid_uart_stream;

  rfid_uart_stream=open(SERIAL_PORT, O_RDONLY | O_NOCTTY); //Get file descriptor
  if(rfid_uart_stream==-1){
    fprintf(stderr, "Unable to open UART\n");
    exit(-1);
  }

  tcgetattr(rfid_uart_stream, serial_uart_options); //Link options to serial port
  
  //Set serial options
  serial_uart_options->c_cflag=BAUD_RATE | CS8 | CLOCAL | CREAD;
  serial_uart_options->c_iflag=IGNPAR;
  serial_uart_options->c_oflag=0;
  serial_uart_options->c_lflag=0;
  
  tcflush(rfid_uart_stream, TCIFLUSH);
  tcsetattr(rfid_uart_stream, TCSANOW, serial_uart_options); //Apply serial options

  return rfid_uart_stream;
}

int serial_rx(const int rfid_uart_stream, char* rx_buffer, const int rx_buffer_size){
  int rx_length;
  int read_times=0;
  
  for(;;){
    rx_length=read(rfid_uart_stream, (void*)rx_buffer, rx_buffer_size);

    read_times++;
    rx_buffer+=rx_length;
    
    if(rx_length<0){
      perror("read");
      return -1;
    }
    else if(rx_length==0){
      //Do nothing
    } else if(read_times==2){
      rx_buffer[0]='\0'; //Null-terminate the string
      break;
    }
  }

  return 0;
}

int serial_close(int rfid_uart_stream){
  if(close(rfid_uart_stream)==-1){
    perror("close");
    return -1;
  }

  return 0;
}

int serial_checksum(char* ascii_data){
  unsigned char hex_value[6];
  char* position=ascii_data;
  
  size_t count;
  size_t max_count=sizeof(hex_value)/sizeof(hex_value[0]);
  unsigned char check=0;

  for(count=0;count<max_count;count++){
    sscanf(position, "%2hhx", &hex_value[count]);
    position+=2;
  }

  check=hex_value[0];
  for(count=1;count<max_count-1;count++){
    check^=hex_value[count];
  }

  if(check==hex_value[max_count-1]){
    return 0;
  } else{
    return -1;
  }
}

char* serial_prepare_data(char* rx_buffer){
  char* token;
  const char delim='\r';
  char* ptr;

  ptr=rx_buffer+1; //Get rid of STX
  
  token=strtok(ptr, &delim); //get rid of trailing CR and LF
  printf("%s\n",token);
  return token;
}

char* serial_package_data(char* data, char* packaged_str){
  #ifdef ARRIVAL
  const char arrival_string[]="ARR$";
  #else
  const char arrival_string[]="DEP$";
  #endif

  data[strlen(data)-2]='\0'; //Remove checksum
  strcpy(packaged_str, arrival_string);
  strcat(packaged_str, data);

  return packaged_str;
}
