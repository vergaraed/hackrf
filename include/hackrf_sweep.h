#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/common.h"

// cb handled by client from hackrf_sweep on a *cb rec from hackrf.c 
typedef void (*hackrf_sweep_cb) (hackrf_sweep_vals *val);

//static hackrf_sweep_cb retdata2client_cb = 0;

typedef struct
{
    char [32] ip;
    uint port;
    uint freq_min;
    uint freq_max;
    uint requested_fft_bin_width;
    uint num_sweeps;
    uint vga_gain;
    uint lna_gain;
    bool antenna_enable;
    hackrf_sweep_cb retdata2client_cb;
} hackrf_sweep_args;

