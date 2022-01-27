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
#include "../../include/common.h"

void shutdown_properly(int code);

int get_client_name(char *ip, char *client_name)
{
  if (strlen(ip) > 5)
    strcpy(client_name, ip);
  else
    strcpy(client_name, "no name");
  
  return 0;
}

int create_client(char *ip, int port)
{
    int sock;
    struct sockaddr_in l_addr;
    socklen_t l_add_len;
    peer_t client_peer;
    create_peer(&client_peer);
    
    // create client socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("connect_client() socket");
        return -1;
    }

    /* get the IP of my bound interface */
    if((call_state = getsockname(sock, (struct sockaddr*) &l_addr, &l_add_len)) < 0) {
        close(sock);
        return -1;
    }

    /* Time to populate the p2p struct */

    memset(&client_peer->p2p.local_addr, 0, sizeof(l_addr));
    client_peer->p2p.local_addr = l_addr;
    client_peer->p2p.client_sock = sock;
    
    // Client connect to server 
    if (connect(client_peer->p2p.client_socket, 
                (struct sockaddr *)&client_peer->p2p.server_addr, 
                sizeof(client_peer->p2p.server_addr)) != 0) 
    {
        perror("client connect() to server.");
        return -1;
    }
 
    add_peer(client_peer); 
    printf("Connected to %s:%d.\n", ip, port);
  
    return 0;
}

int sendMessageToPeer(peer_t *client, void *buffMsg, int sz)
{
  char read_buffer[MAX_DATA_SIZE]; // buffer for stdin
  
  printf("stdin cmd: \n.", read_buffer);

  // Create new message and enqueue it.
  message_t new_message;
  prepare_message(read_buffer, &new_message, sz);
  print_message(&new_message);
  
  if (add_to_send_buff(client, &new_message, new_message.size) < 0) {
    printf("Send buffer is overflowed, we lost this message!\n");
    return -1;
  }
  printf("New message to send was enqueued right now.\n");
  
  return 0;
}

void StartClientSocket(startClientSocketThreadArgs args)
{
    if (setup_signals() != 0)
    {
	    fprintf(stderr, "signals setup error: NULL\n");
        return EXIT_FAILURE;
    }
  
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;
  
    printf("Running Rest API Request Listener Thread\n");

    /* create a Request Listener thread */
    pthread_t threadProcMsg = create the process message thread
    if(pthread_create(&threadProcMsg, NULL, (void*)ProcMsgThread, (void*)args.appPMFunc_cb)) {
        fprintf(stderr, "Error creating ProcMsgThread!\n");
        return 1;
    }
    printf("Running Client Socket Process Message Thread\n");

    run_sock_fd_sets(0);

    pthread_join(threadProcMsg, NULL);

}

int StopClient()
{
    isRunning=0;
    //cleanup
}
