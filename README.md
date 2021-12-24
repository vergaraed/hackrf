# Client
Client (IoT controller) that has a client profile (CID) and encrypts data to persist by the Node via RF / Bluetooth.


## Installing gnuradio on Mac

brew upgrade node

### Replace symbol links (-llibgsl) with hard links:
 /usr/local/lib/libgsl.dylib 
 /usr/local/Cellar/gsl/2.7/lib/libgslcblas.dylib

vi lib/CMakeFiles/gnuradio-fec.dir/link.txt 

## Convert to Plugin
Share the struct binary (un)packed through the socket.  common.h
This is a start, need to better define this.

```typedef struct
{
    int date;
    int time;
    int hz_low;
    int hz_high;
    int hz_bin_width;
    int num_samples;
    int dB;
} hackrf_plugin_device;
```

### common_hackrf.h

```
#pragma_pack(1);
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
    uint64_t fftSize;
} hackrf_sweep_vals;

#pragma_unpack(0);
```
```
typedef struct
{
    int date;
    int time;
    int hz_low;
    int hz_high;
    int hz_bin_width;
    int num_samples;
    int dB;
} hackrf_plugin_device;
```

### client.h



### hackrf_sweep.c
```
int hackrf_sweep(int minfreq, int maxfreq, int numsweeps)
- //int main(int argc, char** argv) - a:f:p:l:g:d:n:N:w:1BIr:h?
- hackrf_start_rx(device, rx_callback, NULL)
- hackrf_init_sweep(device, frequencies, num_ranges, num_samples * 2,
            TUNE_STEP * FREQ_ONE_MHZ, OFFSET, INTERLEAVED);

int rx_callback(hackrf_transfer* transfer)
- fftwf_execute(ifftwPlan);
- // Call cb with the fields
  // cb(time_str, time_stamp.tv_usec,
  //  (uint64_t)(frequency+(DEFAULT_SAMPLE_RATE_HZ/2)),
  //  (uint64_t)(frequency+((DEFAULT_SAMPLE_RATE_HZ*3)/4)),
  //  fft_bin_width, fftSize);
```

### hackrf.c
```
int ADDCALL hackrf_start_rx(hackrf_device* device, hackrf_sample_block_cb_fn callback, void* rx_ctx)
    device->rx_ctx = rx_ctx;
    result = create_transfer_thread(device, endpoint_address, callback);

static int create_transfer_thread(hackrf_device* device,
    const uint8_t endpoint_address,
        hackrf_sample_block_cb_fn callback)

struct hackrf_device {
    libusb_device_handle* usb_device;
    struct libusb_transfer** transfers;
    hackrf_sample_block_cb_fn callback;
    volatile bool transfer_thread_started; /* volatile shared between threads (read only) */
    pthread_t transfer_thread;
    volatile bool streaming; /* volatile shared between threads (read only) */
    void* rx_ctx;
    void* tx_ctx;
    volatile bool do_exit;
    unsigned char buffer[TRANSFER_COUNT * TRANSFER_BUFFER_SIZE];
};
```
