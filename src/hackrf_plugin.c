#include <stdio.h>
#include <pthread.h>
#include "../../include/client.h"
#include "../../include/common_hackrf.h"

static hackrf_device *admin_device;
static peer_t **peers;

int main(int argc, char** argv)
{
    //Start the Curl IPFS Client and Rest API Server
    pthread_t clientSocketThread;
    pthread_t hackrfThread;

    if(pthread_create(&clientSocketThread, NULL, &ClientSocketThread, (void*) ctargs)) {
        fprintf(stderr, "Error creating Client Socket thread.\n");
        return 1;
    }
    printf("Started Client Socket Thread.\n");


    //
    // This is the device or IoT USB Device manager
    // We also handle multiple devices by serial number.
    // 
    printf("Starting HackRF Device(s) Manager Thread.\n");
    /* create a HackRF thread */
    startHackRFThreadData* hrftd = (startHackRFThreadData*)malloc(sizeof(startHackRFThreadData));
    hrftd->procHackRFSweepMsgCB = hackrf_data_updates_cb ;
    
    if(pthread_create(&hackRFThread, NULL, &HackRFThread, (void*) hrftd)) {
        fprintf(stderr, "Error creating HackRFthread\n");
        return 1;
    }
    printf("Started HackRF Thread.\n");

    /*
    // If entitled:
    printf("Starting HackRF Admininstration Thread.\n");
    // create a HackRF Admininstration HackRF Device Threadthread 
    hackrf_plugin_device* hrftd = (hackrf_plugin_device*)malloc(sizeof(hackrf_plugin_device));
    
    if(pthread_create(&hackRFAdminThread, NULL, &HackRFAdminThread, (void*) hrftd )) {
        fprintf(stderr, "Error creating Admininstration HackRF Device Thread\n");
        return 1;
    }
    printf("Started HackRF Admininstration Thread.\n");
    */

    pthread_join(clientSocketThread, NULL);
    pthread_join(hackRFThread, NULL);
    //pthread_join(hackRFAdminThread, NULL);

    printf("Exiting HackRF Plugin...");

    return argc;
}

pthread_t RunClientSocket()
{ 
    pthread_t clientSocketThread;

    //Client Socket Connection Thread to manage connections
    //Connect to Gateway and allow it to request the connection and disconnection
    // add/remove to other Clients.
    printf("Starting a Client Socket Thread.\n");

    startClientSocketThreadArgs *cstargs = malloc(sizeof(startClientSocketThreadArgs));
    ctargs.ip = malloc(sizeof(char*), MAX_IP_SIZE);
    strncpy(ctargs.ip, SERVER_IPV4_ADDR, SERVER_IPV4_ADDR);
    ctargs.port = SERVER_LISTEN_PORT;
    ctargs.CID = malloc(strlen(MAX_CID_LEN")+1);
    strncpy(ctargs.CID, SERVER_CID, strlen(SERVER_CID));
    ctargs.cbIncomingMsg = &client_request_cb;
    ctargs.clientSweepRequestCB = &sweep_request_cb;
    
    // Create a Client Socket thread to Server (or gateway).  This is a basic IoT thin client owned by a gateway.
    // Each Gateway has n clients or IoT devices.  The IoT devices will encrypt the data with the Gateways key.
    // This plugin service should get commands from the server by providing a callback func. (sweep_request_cb)
    if(pthread_create(&clientSocketThread, NULL, &ClientSocketThread, (void*) ctargs)) 
    {
        fprintf(stderr, "Error creating Client Socket thread.\n");
        return 1;
    }
    printf("Started Client Socket Thread.\n");
}

static void* AdminHackRFDeviceThread(void *args)
{   
    hackrf_plugin_device* hrfadmin = (hackrf_plugin_device*)malloc(sizeof(hackrf_plugin_device))
    printf("HackRF PlugIn Starting HackRF Admininstration Device Service Thread./n");
    StartHackRFAdmin(hrfadmin);
    printf("HackRF PlugIn Stopped HackRF Admininstration Device Service Thread./n");
}


static void* HackRFThread(void *threadHackRFArgs)
{
    startHackRFThreadData* hrftd = threadHackRFArgs;
    printf("HackRF PlugIn Starting HackRF Device Service Thread./n");
    StartHackRFSweepService(hrftd);
    printf("HackRF PlugIn Stopped HackRF Device Service Thread./n");
}

static void* ClientSocketThread(void *args)
{
    // Connect to Server(s)
    printf("Starting HackRF Plugin ClientSocketThread./n");
    pthread_t clientSocketThread;

    //Client Socket Connection Thread to manage connections
    //Connect to Gateway and allow it to request the connection and disconnection
    // add/remove to other Clients.
    printf("Starting a Client Socket Thread.\n");

    startClientSocketThreadArgs *cstargs = malloc(sizeof(startClientSocketThreadArgs));
    //ctargs->ip = malloc(sizeof(char*), MAX_IP_SIZE);
    strncpy(ctargs.ip, SERVER_IPV4_ADDR, SERVER_IPV4_ADDR);
    ctargs.port = SERVER_LISTEN_PORT;
    ctargs.CID = malloc(strlen(MAX_CID_SIZE")+1);
    strncpy(ctargs.CID, SERVER_CID, strlen(SERVER_CID));
    ctargs.procServerMsgCB = &client_request_cb;
    ctargs.clientSweepRequestCB = &sweep_request_cb;
    // Create a Cliet Socket thread to Server (or gateway).  This is a basic IoT thin client owned by a gateway.
    // Each Gateway has n clients or IoT devices.  The IoT devices will encrypt the data with the Gateways key.
    // This plugin service should get commands from the server by providing a callback func. (sweep_request_cb)
    if(pthread_create(&clientSocketThread, NULL, &ClientSocketThread, (void*) ctargs)) 
    {
    startClientSocketThreadArgs* ctd = (startClientSocketThreadArgs*)args;
    StartClientSocket(ctd);
    printf("Stopped ClientSocketThread./n");
}


void hackrf_data_updates_cb(byte *buff)
{
    //convert from hackrf_clientsweep_vals to hackrf_sweep_vals
    
    //Here we'll encrypt and put the results into IPFS.
    //But for now we'll just print out or send somewhere.

    if (init) 
    {
        char    ip[STRIPMAX];
        size_t  len;
        int len = strncpy(ip, "127.0.0.1", STRIPMAX);
        clientconnect(ip, port);
    }
    
    //encrypt(buff);
    //Send buffer to socket
    //sendMessageToPeer
}


void client_request_cb(message_t *req)
{
    // get the cmd 
    hackrf_sweep_request *sw_req = (hackrf_sweep_request *)req->data);

    //Process Sweep/Connect/Disconnect
    if (sw_req->cmdtype == 1)
        hackrf_sweep(sw_req);
    //sweep_request_cb((hackrf_sweep_request *)req->data);
    else if (sw_req->cmdtype == 2)
        connect_to_new_server(req->cmd);

}

//
//This will be called from the cient once connected.
//
void sweep_request_cb(hackrf_sweep_request *args)
{
    //hackrf_sweep_request *req = (*hackrf_sweep_request)malloc(sizeof(hackrf_sweep_args));

    //parse the args into the struct
    // we'll cheat...
    //args->retdata2client_cb = hackrf_data_updates_cb;
    req->retdata2client_cb = args->hackrf_data_updates_cb;

    //hackrf_sweep_request
    hackrf_sweep(args);
    postHackRFDeviceRequest(req);

}

//This will be called from the cient once connected.
//
void connect_to_new_server(byte *buff)
{
//    hackrf_sweep_args *args = (*hackrf_sweep_args)malloc(sizeof(hackrf_sweep_args));
    hackrf_sweep_request *req = (*hackrf_sweep_request)malloc(sizeof(hackrf_sweep_args));

    //parse the args into the struct
    // we'll cheat...
    args->retdata2client_cb = hackrf_data_updates_cb;
    req->retdata2client_cb = hackrf_data_updates_cb;

    //hackrf_sweep_request
    hackrf_sweep(args);
    postHackRFDeviceRequest(req);

}

