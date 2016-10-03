#include <netinet/in.h>
#include <termios.h>

void cht_func(char* packaged_str,char* cht);
  
// Network prototypes
int network_set_up(struct sockaddr_in* server, char* address);
int network_connect(struct sockaddr_in* server, int network_socket);
int network_close(int network_socket);
int network_shutdown(int network_socket);

//Serial prototypes
int serial_setup(struct termios* serial_uart_options);
int serial_rx(const int rfid_uart_stream, char* rx_buffer, const int rx_buffer_size);
int serial_close(int rfid_uart_stream);
int serial_checksum(char* ascii_data);
char* serial_prepare_data(char* rx_buffer);
char* serial_package_data(char* data, char* packaged_str);
