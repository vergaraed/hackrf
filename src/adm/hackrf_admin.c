#include "hackrf_admin.h"
#include <libusb.h>

static hackrf_device *admin_device;

static int ConnectHackRFDevice(hackrf_device *device)
{
    int result = hackrf_init();
    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "hackrf_init() failed: %s (%d)\n", hackrf_error_name(result), result);
        usage();
        return EXIT_FAILURE;
    }

	result = hackrf_open(&device);
			//result = hackrf_open_by_serial(serial_number, &device);
    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "hackrf_open() failed: %s (%d)\n", hackrf_error_name(result), result);
        usage();
        return EXIT_FAILURE;
    }

    return 0;
}

//hackrf_usb_api_version_read

static void StartHackRF(hackrf_sweep_request *hrfadmin)
{
    // Put a mutex on the device number to prevent 
    // multiple administrators on the same device.
    //
    // TryLockMutex(hrfadmindev->device->serial_number) {}

    printf("HackRF PlugIn Starting HackRF Admininstration Device Service Thread./n");
    
    // We should setup the callback, 
    //  1. List all devices with stats. (hackrf_device_list),
    //  2. Accept all setup commands,
    //  3. Manage Users.

    int retval = ConnectHackRFDevice(hrfadmin->device);
    printf("HackRF PlugIn Stopped HackRF Admininstration Device Service Thread./n");
    return retval;
}

//API
void StartHackRFAdmin(hackrf_sweep_request device)
{
    hrfadmin = (hackrf_sweep_request*)malloc(sizeof(hackrf_sweep_request));
    printf("HackRF PlugIn Starting HackRF Admininstration Device Service Thread./n");
    StartHackRF(hrfadmin);
    ConnectHackRFDevice(hrfadmin->device);
    printf("HackRF PlugIn Stopped HackRF Admininstration Device Service Thread./n");
}

boolean ListedContainsDevice(libusb_device *device, int device_cnt,
        hackrf_sweep_request **list)
{
    for (int i=0; i<=device_cnt; i++)
    {
        if (device->serial_number == *(list+i*sizeof(hackrf_sweep_request)->serial_number)
            return true;
    }
    return false;
}

void StartHackRFAdmins(hackrf_sweep_request *devices, hackrf_sweep_request *started_devices)
{
    // discover devices
    libusb_device **list;
    libusb_device *found = NULL;
    ssize_t cnt = libusb_get_device_list(NULL, &list);
    ssize_t i = 0;
    int err = 0;
    if (cnt < 0)
        error();
 
    for (i = 0; i < cnt; i++) {
        libusb_device *device = list[i];
        if (deviceListed(device, cnt, *devices)) {
            found = device;
            break;
        }
    }
 
    if (found) {
        libusb_device_handle *handle;
 
        err = libusb_open(found, &handle);
        if (err)
            error();
        // etc
    }
 
    libusb_free_device_list(list, 1);
}


int GetListofHackRFDevices(hackrf_sweep_request *sweepreq, const char* const desired_serial_number)
{
    printf("HackRF Admininstration Device Service: GetListofHackRFDevices./n");
    hackrf_device_list_t* devices = hackrf_device_list();
    if (devices != NULL)
    {
        printf("HackRF Admininstration Device Service: GetListofHackRFDevices: %d./n", 
            devices->usb_devicecount);

        libusb_device_handle* usb_device = NULL;
        libusb_device** devices = NULL;
        const ssize_t list_length = libusb_get_device_list(g_libusb_context, &devices);
        ssize_t match_len = 0;
        ssize_t i;
        char serial_number[64];
        int serial_number_length;

        if( desired_serial_number ) {
            /* If a shorter serial number is specified, only match against the suffix.
            * Should probably complain if the match is not unique, currently doesn't.
            */
            match_len = strlen(desired_serial_number);
            if ( match_len > 32 )
                return NULL;
        }
    
        for (i=0; i<list_length; i++) {
            struct libusb_device_descriptor device_descriptor;
            libusb_get_device_descriptor(devices[i], &device_descriptor);
    
            if( device_descriptor.idVendor == hackrf_usb_vid ) {
                if((device_descriptor.idProduct == hackrf_one_usb_pid) ||
                (device_descriptor.idProduct == hackrf_jawbreaker_usb_pid) ||
                (device_descriptor.idProduct == rad1o_usb_pid)) {
    
                    if( desired_serial_number != NULL ) {
                        const uint_fast8_t serial_descriptor_index = device_descriptor.iSerialNumber;
                        if( serial_descriptor_index > 0 ) {
                            if( libusb_open(devices[i], &usb_device) != 0 ) {
                                usb_device = NULL;
                                continue;
                            }
                            serial_number_length = libusb_get_string_descriptor_ascii(usb_device, serial_descriptor_index, (unsigned char*)serial_number, sizeof(serial_number));
    
                            if( serial_number_length >= USB_MAX_SERIAL_LENGTH )
                                serial_number_length = USB_MAX_SERIAL_LENGTH;
                            serial_number[serial_number_length] = 0;
                            if( strncmp(serial_number + serial_number_length-match_len, desired_serial_number, match_len) == 0 ) {
                                break;
                            } else {
                                libusb_close(usb_device);
                                usb_device = NULL;
                            }
                        }
                    } else {
                        libusb_open(devices[i], &usb_device);
                        break;
                    }
                }
            }
        }

        libusb_free_device_list(devices, 1);
        return devices->usb_devicecount;
    }

    //No devices found or error.
    return -1;
}


