#ifndef COMMON_HACKRF_H
#define COMMON_HACKRF_H
////////////////////////////////////
//
//  BlockchainBPI - Cryptreserve
//  This should be a common plugin file.
//  Break out any specific hackrf refs.
//
////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdint.h>
#include "../../../include/common.h"

#define MAX_IP_SIZE 16
#define GATEWAY_CID  "QmfQkD8pBSBCBxWEwFSu4XaDVSWK6bjnNuaWZjMyQbyDub"

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

// hack rf messages ------------------------------------------------------------

#pragma_pack(1);

//This is used by the client to return requested data to the server.
// We use array of values which can be serialized across the socket.
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
    uint64_t power[];
    uint16_t power_cnt;
} hackrf_sweep_vals;

//Used internally within the PlugIn IoT device.
//as we are using pointers to memory
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
    uintptr_t **power;
    //dont need count we can use the defined macro
    //uint16_t power_cnt;
} hackrf_client_sweep_vals;

//This is used by the server to issue requests to the IOT device
typedef struct
{
    uint64_t date;
    uint64_t time;
    uint64_t hz_low;
    uint64_t hz_high;
    uint64_t hz_bin_width;
    uint16_t num_samples;
    uint64_t dB;
} hackrf_sweep_request;

//This is used by the server to issue requests to the IOT device need
//to use an array across socket to de/serialize the struct and data
typedef struct
{
    uint8_t     cmd;
    byte        cmdbuff[MAX_DATA_SIZE];
    uint16_t    cmdlen;
} server_req; // in reality this may be the IoT Gateway

typedef struct
{
    char peerCID[MAX_CID_SIZE];
    char ip[SERVER_IPV4_ADDR];
    uint16_t port;
    *(void processMessageCB(void *)) appPMFunc_cb;
} startClientSocketThreadArgs;

typedef struct
{
    char *serialNumber;
    uint16_t port;
    *(void processMessageCB(void *)) procHackRFSweepMsgCB;
} startHackRFThreadArgs;

#pragma_unpack(0);


int prepare_hrf_message(hackrf_client_sweep_vals *hrfcs_vals)
{
    //hackrf_client_sweep_args
    //Deserialize each member into a byte array
  sprintf(message->sender, "%s", sender);
  sprintf(message->data, "%s", serialize(hrfcs_vals));
  return 0;
}
 
byte* serialize_char(unsigned char* buff, int val)
{
    buff[0] = val >>24;

    byte *bytearrbuff = (byte*)malloc(sizeof(byte)*sizeof(hrfcs_vals)+1);

    sprintf(*bytearrbuff,"%f", hrfcs_vals->frequency);
}

unsigned char *serialize_uint16(unsigned char* buff, uint16_t val)
{
    buff[0] = val >> 56;
    buff[1] = val >> 48;
    buff[2] = val >> 40;
    buff[3] = val >> 32;
    buff[4] = val >> 24;
    buff[5] = val >> 16;
    buff[6] = val >> 8;
    buff[7] = val >> 16;
    buff[8] = val >> 24;
    buff[9] = val >> 16;
}
unsigned char *serialize_uint64(unsigned char* buff, uint64_t val)
{
    buff[0] = val >> 56;
    buff[1] = val >> 48;
    buff[2] = val >> 40;
    buff[3] = val >> 32;
    buff[4] = val >> 24;
    buff[5] = val >> 16;
    buff[6] = val >> 8;
    buff[7] = val >> 16;
    buff[8] = val >> 24;
    buff[9] = val >> 16;


}
unsigned char* serialize_char()
{

}
int print_hrf_message(byte *hrfsweepvalbytes)
{
    //cast *byte to *hackrf_sweep_vals
    // need to pack the struct field by field
    // cant cast since we have a variably sized array 
    hackrf_sweep_vals *hrfs_vals = malloc(sizeof(hackrf_sweep_vals));
    memcpy(hrfs_vals->tv_usec, "%f", sizeof(hrfs_vals->tv_usec));
    hrfs_vals->lowfreq;
    hrfs_vals->upperfreq;
    hrfs_vals->fft_bin_width;
    hrfs_vals->fftSize;
    hrfs_vals->sampleratefrequency1; //+(DEFAULT_SAMPLE_RATE_HZ/2)),
    hrfs_vals->sampleratefrequency2; //+((DEFAULT_SAMPLE_RATE_HZ*3)/4)),
    hrfs_vals->fft_bin_width;
    hrfs_vals->fftSampleSize;
    hrfs_vals->power = malloc(sizeof(hrfs_vals->power)*hrfs_vals->power_cnt);
    for(int i=0; i<=power_cnt; i++)
    {
        hrfs_vals->power[];
        hrfs_vals->power_cnt;
    }

    printf("Message: \"%llx: %llx\"\n", val->sender, val->data);
    printf("%llx", myval);
    return 0;
}

#endif /* COMMON_HACKRF_H */

