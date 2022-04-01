/*
 
    Service to HackRF which will handle the 
    running of a long term 
    RF Monitoring Application.  

    1. HeartBeat Monitor Thread
    2. Shutdown and Restart capablilties
    3. Add to the HackRF CryptReserve Plugins

    Copywrite BlockchainBPI 2022
    Copywrite Crypt Reserve 2022
    Copywrite Ed Vergara   2022

*/

#include <hackrf.h>
#include <stdio.h>
#include <stdlib.h>

int CheckHackRFStatus(void *HRFPID)
{
	int result = HACKRF_SUCCESS;
	uint8_t board_id = BOARD_ID_INVALID;
	char version[255 + 1];
	uint16_t usb_version;
	read_partid_serialno_t read_partid_serialno;
	uint8_t operacakes[8];
	hackrf_device_list_t *list;
	hackrf_device* device;
	int i, j;

	result = hackrf_init();
	if (result != HACKRF_SUCCESS) {
		fprintf(stderr, "hackrf_init() failed: %s (%d)\n",
				hackrf_error_name(result), result);
		return EXIT_FAILURE;
	}

	printf("hackrf_info version: %s\n", TOOL_RELEASE);
	printf("libhackrf version: %s (%s)\n", hackrf_library_release(),
			hackrf_library_version());
	
	list = hackrf_device_list();
	
	if (list->devicecount < 1 ) {
        // Hack RF Disconnected
        //    Alarm User
		printf("No HackRF boards found.\n");
        bNoConnection=1;
        //Kill the running
		return EXIT_FAILURE;
	}
	
	for (i = 0; i < list->devicecount; i++) {
		if (i > 0)
			printf("\n");
			
		printf("Found HackRF\n");
		printf("Index: %d\n", i);
		
		if (list->serial_numbers[i])
			printf("Serial number: %s\n", list->serial_numbers[i]);

		device = NULL;
		result = hackrf_device_list_open(list, i, &device);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_open() failed: %s (%d)\n",
					hackrf_error_name(result), result);
			if(result == HACKRF_ERROR_LIBUSB) {
				continue;
			}
            //HackRF is busy with another PID - Is this us?
            continue;
			//return EXIT_FAILURE;
		}

		result = hackrf_board_id_read(device, &board_id);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_board_id_read() failed: %s (%d)\n",
					hackrf_error_name(result), result);
			return EXIT_FAILURE;
		}
		printf("Board ID Number: %d (%s)\n", board_id,
				hackrf_board_id_name(board_id));

		result = hackrf_version_string_read(device, &version[0], 255);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_version_string_read() failed: %s (%d)\n",
					hackrf_error_name(result), result);
			return EXIT_FAILURE;
		}

		result = hackrf_usb_api_version_read(device, &usb_version);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_usb_api_version_read() failed: %s (%d)\n",
					hackrf_error_name(result), result);
			return EXIT_FAILURE;
		}
		printf("Firmware Version: %s (API:%x.%02x)\n", version,
				(usb_version>>8)&0xFF, usb_version&0xFF);

		result = hackrf_board_partid_serialno_read(device, &read_partid_serialno);	
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_board_partid_serialno_read() failed: %s (%d)\n",
					hackrf_error_name(result), result);
			return EXIT_FAILURE;
		}
		printf("Part ID Number: 0x%08x 0x%08x\n", 
					read_partid_serialno.part_id[0],
					read_partid_serialno.part_id[1]);

		result = hackrf_get_operacake_boards(device, &operacakes[0]);
		if ((result != HACKRF_SUCCESS) && (result != HACKRF_ERROR_USB_API_VERSION)) {
			fprintf(stderr, "hackrf_get_operacake_boards() failed: %s (%d)\n",
					hackrf_error_name(result), result);
				return EXIT_FAILURE;
		}
		if(result == HACKRF_SUCCESS) {
			for(j=0; j<8; j++) {
				if(operacakes[j] == 0)
					break;
				printf("Operacake found, address: 0x%02x\n", operacakes[j]);
			}
		}
/*
		uint32_t cpld_crc = 0;
		result = hackrf_cpld_checksum(device, &cpld_crc);
		if ((result != HACKRF_SUCCESS) && (result != HACKRF_ERROR_USB_API_VERSION)) {
			fprintf(stderr, "hackrf_cpld_checksum() failed: %s (%d)\n",
					hackrf_error_name(result), result);
				return EXIT_FAILURE;
		}
		if(result == HACKRF_SUCCESS) {
			printf("CPLD checksum: 0x%08x\n", cpld_crc);
		}
*/
		result = hackrf_close(device);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_close() failed: %s (%d)\n",
					hackrf_error_name(result), result);
		}
	}
	
	hackrf_device_list_free(list);
	hackrf_exit();

	return EXIT_SUCCESS;
}


