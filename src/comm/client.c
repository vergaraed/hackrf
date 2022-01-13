// Simple example of client.
// Client prints received messages to stdout and sends from stdin.

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/client.h"

peer_t server;

void shutdown_properly(int code);

void handle_signal_action(int sig_number)
{
  if (sig_number == SIGINT) {
    printf("SIGINT was catched!\n");
    shutdown_properly(EXIT_SUCCESS);
  }
  else if (sig_number == SIGPIPE) {
    printf("SIGPIPE was catched!\n");
    shutdown_properly(EXIT_SUCCESS);
  }
}

int setup_signals()
{
  struct sigaction sa;
  sa.sa_handler = handle_signal_action;
  if (sigaction(SIGINT, &sa, 0) != 0) {
    perror("sigaction()");
    return -1;
  }
  if (sigaction(SIGPIPE, &sa, 0) != 0) {
    perror("sigaction()");
    return -1;
  }
  
  return 0;
}

int get_client_name(char *ip, char *client_name)
{
  if (strlen(ip) > 5)
    strcpy(client_name, ip);
  else
    strcpy(client_name, "no name");
  
  return 0;
}

int connect_server(char *ip, int port, peer_t *server)
{
  // create socket
  server->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server->socket < 0) {
    perror("socket()");
    return -1;
  }
  
  // set up addres
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip); //SERVER_IPV4_ADDR);
  server_addr.sin_port = htons(port); //SERVER_LISTEN_PORT);

  printf("Port %d\n", port);

  server->addres = server_addr;
  
  if (connect(server->socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) != 0) {
    perror("connect()");
    return -1;
  }
  
  //printf("Connected to %s:%d.\n", SERVER_IPV4_ADDR, SERVER_LISTEN_PORT);
  printf("Connected to %s:%d.\n", ip, port);
  
  return 0;
}

int build_fd_sets(peer_t *server, fd_set *read_fds, fd_set *write_fds, fd_set *except_fds)
{
  FD_ZERO(read_fds);
  FD_SET(STDIN_FILENO, read_fds);
  FD_SET(server->socket, read_fds);
  
  FD_ZERO(write_fds);
  // there is smth to send, set up write_fd for server socket
  if (server->send_buffer.current > 0)
    FD_SET(server->socket, write_fds);
  
  FD_ZERO(except_fds);
  FD_SET(STDIN_FILENO, except_fds);
  FD_SET(server->socket, except_fds);
  
  return 0;
}

int handle_read_from_stdin(peer_t *server, char (*client_name)[] )
{
  char read_buffer[DATA_MAXSIZE]; // buffer for stdin
  
  //if (read_from_stdin(read_buffer, DATA_MAXSIZE) != 0)
  if (!parseClientInput(client_name, read_buffer, DATA_MAXSIZE))
    return 0;
 
  printf("stdin cmd: \n.", read_buffer);

  // Create new message and enqueue it.
  message_t new_message;
  prepare_message(client_name, read_buffer, &new_message);
  print_message(&new_message);
  
  if (peer_add_to_send(server, &new_message) < 0) {
    printf("Send buffer is overflowed, we lost this message!\n");
    return -1;
  }
  printf("New message to send was enqueued right now.\n");
  
  return 0;
}

/* You should be careful when using this function in multythread program. 
 * Ensure that server is thread-safe. */
void shutdown_properly(int code)
{
  delete_peer(&server);
  printf("Shutdown client properly.\n");
  //exit(code);
}

processMessageCB *plugin_cb;

//void handle_received_message(hackrf_sweep_args *req_args)
void handle_received_message(server_req *req_args)
{    
    printf("Received message from server process request to IOT.\n");
    print_message(req_args);
    //send to plugin::sweep_request_cb
    // startClientSocketThreadArgs::procSocketMsgCB

    //sweep_request_cb(req_args);
    plugin_cb(req_args);

    return 0;
}

int StartClientSocket(startClientSocketThreadArgs *inArgs)
{
    startClientSocketThreadArgs *sCSTA = (*startClientSocketThreadArgs) inArgs;
    //processMessageCB *cb = sCSTA->procSocketMsgCB;
    plugin_cb = sCSTA->procSocketMsgCB;

    int ret = clientconnect(sCSTA->ip, sCSTA->port); 

    return ret;
}
int clientconnect(char *ip, int port, char *CID )
{
    if (setup_signals() != 0)
    {
	    fprintf(stderr,
			"signals setup error: NULL\n");
        return EXIT_FAILURE;
    }
  
    char client_name[256];
    get_client_name(ip, client_name);
    printf("Client '%s' start.\n", client_name);
    strncpy(server->CID, CID, CID_MAX_SIZE);  
    if (create_peer(&server, CID) > 0)
        return -1;

    if (connect_server(ip,port,&server) != 0)
    {
        shutdown_properly(EXIT_FAILURE);
        return -1;
    }

  /* Set nonblock for stdin. */
  int flag = fcntl(STDIN_FILENO, F_GETFL, 0);
  flag |= O_NONBLOCK;
  fcntl(STDIN_FILENO, F_SETFL, flag);
  
  fd_set read_fds;
  fd_set write_fds;
  fd_set except_fds;
  
  printf("Waiting for server message or stdin input. Please, type text to send:\n");
  
  // server socket always will be greater then STDIN_FILENO
  int maxfd = server.socket;
  
  while (1) {
    // Select() updates fd_set's, so we need to build fd_set's before each select()call.
    build_fd_sets(&server, &read_fds, &write_fds, &except_fds);
        
    int activity = select(maxfd + 1, &read_fds, &write_fds, &except_fds, NULL);
    
    switch (activity) {
      case -1:
        perror("select()");
        shutdown_properly(EXIT_FAILURE);
        return -1;

      case 0:
        // you should never get here
        printf("select() returns 0.\n");
        shutdown_properly(EXIT_FAILURE);
        return -1;
      default:
        /* All fd_set's should be checked. */
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
          if (handle_read_from_stdin(&server, client_name) != 0)
          {
              shutdown_properly(EXIT_FAILURE);
              return -1;
          }
        }

        if (FD_ISSET(STDIN_FILENO, &except_fds)) {
          printf("except_fds for stdin.\n");
          shutdown_properly(EXIT_FAILURE);
          return -1;
        }

        if (FD_ISSET(server.socket, &read_fds)) {
          if (receive_from_peer(&server, &handle_received_message) != 0)
          {
              shutdown_properly(EXIT_FAILURE);
              return -1;
          }
        }

        if (FD_ISSET(server.socket, &write_fds)) {
          if (send_to_peer(&server) != 0)
          {
              shutdown_properly(EXIT_FAILURE);
              return -1;
          }
        }

        if (FD_ISSET(server.socket, &except_fds)) {
          printf("except_fds for server.\n");
          shutdown_properly(EXIT_FAILURE);
          return -1;
        }
    }
    
    printf("And we are still waiting for server or stdin activity. You can type something to send:\n");
  }
  
  return 0;
}
