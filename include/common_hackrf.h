#ifndef COMMON_HACKRF_H
#define COMMON_HACKRF_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdint.h>
#include "../../../include/common.h"

char* ullx(uint64_t val)
{
    static char buf[34] = { [0 ... 33] = 0 };
    char* out = &buf[33];
    uint64_t hval = val;
    unsigned int hbase = 16;

    do {
        *out = "0123456789abcdef"[hval % hbase];
        --out;
        hval /= hbase;
    } while(hval);

    *out-- = 'x', *out = '0';

    return out;
}

/* Maximum bytes that can be send() or recv() via net by one call.
 * It's a good idea to test sending one byte by one.
 */
#define MAX_SEND_SIZE 150

/* Size of send queue (messages). */
#define MAX_MESSAGES_BUFFER_SIZE 550
#define MAX_CID_SIZE 64
#define MAX_IP_SIZE 16
#define MAX_SENDER_SIZE 158
#define MAX_DATA_SIZE 112
#define SERVER_IPV4_ADDR "127.0.0.1"
#define SERVER_LISTEN_PORT 33235
#define SERVER_CID  "QmfQkD8pBSBCBxWEwFSu4XaDVSWK6bjnNuaWZjMyQbyDub"
// This should work since we are counting the pointers in a 
#define NUM_ELEM(x) (sizeof (x) / sizeof (*(x)))

//void delete_peer(struct peer_t *peer, int totalpeers, int i)
int delete_from_obj_array(obj** array, int i, int &totalobjs)
{
    obj *tmp = (obj *) realloc(array, (--totalobjs)*sizeof(obj*));
    
    for (int c = i; c < totalobjs; c++)
        array[c] = array[c+1];

    return totalobjs;
}

void add_to_obj_array(const obj* const new_obj, obj** array) //, const int *number_of_elements)
{
    int number_of_elements = NUM_ELEM(array);

    // expand array with one more item
    array = (obj**)realloc(array, (++(number_of_elements)) * sizeof(new_obj));

    if (array == NULL )
    {
        /* memory request refused :( */
        return;
    }

    // Put new item at the last place on the array
    // Fix this to insert in an ordered by CID list.
    // Better yet create an index list of addresses to 
    // these structs ordered bu CID etc...
    // Then lookup speeds improve using binary tree searches.
    array[number_of_elements] = new_obj;
}

// message --------------------------------------------------------------------

#define SAMPLES_NUM

#pragma_pack(1);

//This is used by the client to return requested data to the server.
typedef struct
{
    //time_str = time_str;
    uint64_t tv_usec;
    uint64_t lowfreq;
    uint64_t upperfreq;
    uint64_t fft_bin_width;
    uint64_t fftSize;
    uint64_t sampleratefrequency1; //+(DEFAULT_SAMPLE_RATE_HZ/2)),
    uint64_t sampleratefrequency2; //+((DEFAULT_SAMPLE_RATE_HZ*3)/4)),
    uint64_t fft_bin_width;
    uint64_t fftSampleSize;
    uint64_t **power;
    uint64_t power_cnt;
} hackrf_sweep_vals;

//This is used by the server to issue requests to the IOT device
typedef struct
{
    uint64_t date;
    uint64_t time;
    uint64_t hz_low;
    uint64_t hz_high;
    uint64_t hz_bin_width;
    uint64_t num_samples;
    uint64_t dB;
} hackrf_sweep_request;

//This is used by the server to issue requests to the IOT device
typedef struct
{
    uint8_t     cmd;
    byte        *cmdbuff;
    uint32_t    cmdlen;
} server_req;


typedef struct
{
    char ip[17];
    uint16 port;
    *(void processMessageCB(void *)) procSocketMsgCB;
} startClientSocketThreadArgs;

typedef struct
{
    char ip[17];
    uint16 port;
    *(void processMessageCB(void *)) procHackRFSweepMsgCB;
} startHackRFThreadArgs;

#pragma_unpack(0);


//startHackRFThreadData
int InitHackRFDevice(device *)
{

#ifdef _MSC_VER
    SetConsoleCtrlHandler( (PHANDLER_ROUTINE) sighandler, TRUE );
#else
    signal(SIGINT, &sigint_callback_handler);
    signal(SIGILL, &sigint_callback_handler);
    signal(SIGFPE, &sigint_callback_handler);
    signal(SIGSEGV, &sigint_callback_handler);
    signal(SIGTERM, &sigint_callback_handler);
    signal(SIGABRT, &sigint_callback_handler);
#endif
    fprintf(stderr, "call hackrf_sample_rate_set(%.03f MHz)\n",
           ((float)DEFAULT_SAMPLE_RATE_HZ/(float)FREQ_ONE_MHZ));
    result = hackrf_set_sample_rate_manual(device, DEFAULT_SAMPLE_RATE_HZ, 1);
    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "hackrf_sample_rate_set() failed: %s (%d)\n",
               hackrf_error_name(result), result);
        usage();
        return EXIT_FAILURE;
    }

    fprintf(stderr, "call hackrf_baseband_filter_bandwidth_set(%.03f MHz)\n",
            ((float)DEFAULT_BASEBAND_FILTER_BANDWIDTH/(float)FREQ_ONE_MHZ));
    result = hackrf_set_baseband_filter_bandwidth(device, DEFAULT_BASEBAND_FILTER_BANDWIDTH);
    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "hackrf_baseband_filter_bandwidth_set() failed: %s (%d)\n",
               hackrf_error_name(result), result);
        usage();
        return EXIT_FAILURE;
    }

    return 0;
}

int prepare_val_message(char *sender, byte *data, hackrf_sweep_vals *message)
{
  sprintf(message->sender, "%s", sender);
  sprintf(message->data, "%s", data);
  return 0;
}
 
int print_val_message(hackrf_sweep_vals *val)
{
  printf("Message: \"%llx: %llx\"\n", val->sender, val->data);

printf("%llx", myval);


  return 0;
}

// message queue --------------------------------------------------------------

typedef struct {
  int size;
  message_t *data;
  int current;
} message_queue_t;

int create_message_queue(int queue_size, message_queue_t *queue)
{
  queue->data = calloc(queue_size, sizeof(char) * MAX_DATA_SIZE+1);
  queue->size = queue_size;
  queue->current = 0;
  
  return 0;
}

void delete_message_queue(message_queue_t *queue)
{
  free(queue->data);
  queue->data = NULL;
  queue->current = 0;
}

int enqueue(message_queue_t *queue, message_t *message)
{
  if (queue->current == queue->size)
    return -1;
      
  print_message(message);

  memcpy(&queue->data[queue->current], message->data, sizeof(message->data));
  //message->current = queue->current+1;
  queue->current++;
  
  return queue->current;
}

int dequeue(message_queue_t *queue, message_t *message)
{
  if (queue->current == 0)
    return -1;
 
  //lock using a lock per queue. 
  memcpy(message->data, &queue->data[queue->current - 1], MAX_DATA_SIZE);
  //message->current = queue->current-1;

  print_message(message);

  printf("Dequeued(%d).\n", queue->current);

  //maybe a strncpy with a max size appending a newline if necessary
  //printf("Dequeued(%d): %s\n", queue->current,(char *)message->data);
  queue->current--;
  
  return queue->current;
}

int dequeue_all(message_queue_t *queue)
{
  queue->current = 0;
  
  return 0;
}

// peer -----------------------------------------------------------------------

typedef struct {
  char *CID;
  int socket;
  struct sockaddr_in addres;
  
  /* Messages that waiting for send. */
  message_queue_t send_buffer;
  
  /* Buffered sending message.
   * 
   * In case we doesn't send whole message per one call send().
   * And current_sending_byte is a pointer to the part of data that will be send next call.
   */
  message_t sending_buffer;
  size_t current_sending_byte;
  
  /* The same for the receiving message. */
  message_t receiving_buffer;
  size_t current_receiving_byte;
} peer_t;

typedef peer_t **peers;
static int peer_cnt=0;

int add_peer(peer_t *peer)
{
    return add_to_obj_array(peers, peer);
}

void delete_peer(peer_t *peer)
{
    for (int i=0; i<peer_cnt; i++)
    {
       if (strncmp(peers[i]->CID, peer->CID, MAX_CID_SIZE)==0)
       {
           delete_from_obj_array((obj**)peers, i, &peer_cnt);
           close(peer->socket);
           delete_message_queue(&peer->send_buffer.data);
           free(peer->CID);
           return;
       }
    }
    //peer not found
   // return -1;
}

int create_peer(peer_t *peer)
{
    create_message_queue(MAX_MESSAGES_BUFFER_SIZE, &peer->send_buffer);
  
    peer->current_sending_byte   = -1;
    peer->current_receiving_byte = 0;
    peer->size; 
    peer_cnt = add_peer(peer, sizeof(peer)); 
    return peer_cnt;
}

char *peer_get_addres_str(peer_t *peer)
{
  static char ret[INET_ADDRSTRLEN + 10];
  char peer_ipv4_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &peer->addres.sin_addr, peer_ipv4_str, INET_ADDRSTRLEN);
  sprintf(ret, "%s:%d", peer_ipv4_str, peer->addres.sin_port);
  
  return ret;
}

int peer_add_to_send(peer_t *peer, message_t *message)
{
  return enqueue(&peer->send_buffer, message);
}

/* Receive message from peer and handle it with message_handler(). */
int receive_from_peer(peer_t *peer, int (*message_handler)(message_t *))
{
  printf("Ready for recv() from %s.\n", peer_get_addres_str(peer));
  
  size_t len_to_receive;
  ssize_t received_count;
  size_t received_total = 0;
  do {
    // Is completely received?
    if (peer->current_receiving_byte >= sizeof(peer->receiving_buffer.data)) {
      message_handler(&peer->receiving_buffer);
      peer->current_receiving_byte = 0;
    }
    
    // Count bytes to send.
    len_to_receive = sizeof(peer->receiving_buffer.data) - peer->current_receiving_byte;
    if (len_to_receive > MAX_SEND_SIZE)
      len_to_receive = MAX_SEND_SIZE;
    
    printf("Let's try to recv() %zd bytes... ", len_to_receive);
    received_count = recv(peer->socket, (char *)&peer->receiving_buffer.data + peer->current_receiving_byte, len_to_receive, MSG_DONTWAIT);
    if (received_count < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        printf("peer is not ready right now, try again later.\n");
      }
      else {
        perror("recv() from peer error");
        return -1;
      }
    } 
    else if (received_count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      break;
    }
    // If recv() returns 0, it means that peer gracefully shutdown. Shutdown client.
    else if (received_count == 0) {
      printf("recv() 0 bytes. Peer gracefully shutdown.\n");
      return -1;
    }
    else if (received_count > 0) {
      peer->current_receiving_byte += received_count;
      received_total += received_count;
      printf("recv() %zd bytes\n", received_count);
      //testing - remove the following line.
      message_handler(&peer->receiving_buffer);
      peer->current_receiving_byte = 0;
    }
  } while (received_count > 0);
  
  printf("Total recv()'ed %zu bytes.\n", received_total);
  return 0;
}

int send_to_peer(peer_t *peer)
{
  printf("Ready for send() to %s.\n", peer_get_addres_str(peer));
  
  size_t len_to_send;
  ssize_t send_count;
  size_t send_total = 0;
  do {
    // If sending message has completely sent and there are messages in queue, why not send them?
    if (peer->current_sending_byte < 0 || peer->current_sending_byte >= sizeof(peer->sending_buffer.data)) {
      printf("There is no pending to send() message, maybe we can find one in queue... ");
      int curr = dequeue(&peer->send_buffer.data, &peer->sending_buffer.data); 
      if (curr < 0) {
        peer->current_sending_byte = -1;
        printf("No, there is nothing to send() anymore.\n");
        break;
      }
      printf("Yes, pop and send() one of them.\n");
      peer->current_sending_byte = 0;
    }
    
    // Count bytes to send.
    len_to_send = sizeof(peer->sending_buffer.data) - peer->current_sending_byte;
    if (len_to_send > MAX_SEND_SIZE)
      len_to_send = MAX_SEND_SIZE;
    
    //printf("Let's try to send(%zd ): %s\n", len_to_send, (char *)peer->sending_buffer);
    send_count = send(peer->socket, (char *)&peer->sending_buffer.data, len_to_send, 0);
    if (send_count < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        printf("peer is not ready right now, try again later.\n");
      }
      else {
        perror("send() from peer error");
        return -1;
      }
    }
    // we have read as many as possible
    else if (send_count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      break;
    }
    else if (send_count == 0) {
      printf("send()'ed 0 bytes. It seems that peer can't accept data right now. Try again later.\n");
      break;
    }
    else if (send_count > 0) {
      peer->current_sending_byte += send_count;
      send_total += send_count;
      printf("send()'ed %zd bytes.\n", send_count);
    }
  } while (send_count > 0);

  printf("Total send()'ed %zu bytes.\n", send_total);
  return 0;
}

// common ---------------------------------------------------------------------

/* Reads from stdin and create new message. This message enqueues to send queueu. */
int read_from_stdin(char *read_buffer, size_t max_len)
{
  memset(read_buffer, 0, max_len);
  
  ssize_t read_count = 0;
  ssize_t total_read = 0;
  
  do {
    read_count = read(STDIN_FILENO, read_buffer, max_len);
    if (read_count < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
      perror("read()");
      return -1;
    }
    else if (read_count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      break;
    }
    else if (read_count > 0) {
      total_read += read_count;
      if (total_read > max_len) {
        printf("Message too large and will be chopped. Please try to be shorter next time.\n");
        fflush(STDIN_FILENO);
        break;
      }
    }
  } while (read_count > 0);
  
  size_t len = strlen(read_buffer);
  if (len > 0 && read_buffer[len - 1] == '\n')
    read_buffer[len - 1] = '\0';
  
  printf("Read from stdin %zu bytes. Let's prepare message to send.\n", strlen(read_buffer));

  return 0;
}

#endif /* COMMON_HACKRF_H */

