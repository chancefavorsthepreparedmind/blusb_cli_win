/*
Copyright (C) 2019  Jörn Giest
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "blusb_cmd_univ_comp_nodrv.h"

uint16_t read_matrix_pos()
{
	static hid_ctrl_report_t hid_ctrl;
	static uint8_t buffer_last[8] = { 0 }, buffer_changed = 0;
	uint8_t i = 0;

	hid_ctrl.id = HID_REPORT_ID_FEATURE_READ_MATRIX;

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS,
		LIBUSB_REQUEST_GET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT | HID_REPORT_ID_FEATURE_READ_MATRIX,
		0, hid_ctrl.buffer, sizeof(hid_ctrl.buffer), 1000);

	if (hid_ctrl.buffer[7])
		for (i = 0; i < 2; i++)
			if (hid_ctrl.buffer[i] ^ buffer_last[i])
			{
				buffer_changed = 1;
				break;
			}

	if (buffer_changed)
	{
		buffer_changed = 0;
		for (i = 0; i < 2; i++)
			buffer_last[i] = hid_ctrl.buffer[i];
		if (verbose)
		{
			printf("\r%24c", '\0');
			printf("\rRow: %d Column: %d", hid_ctrl.buffer[0], hid_ctrl.buffer[1]);
		}
		return hid_ctrl.buffer[0] | hid_ctrl.buffer[1] << 8;			// return row, col
	}
	else
		return 0xffff;
}

void read_version()
{
	static hid_ctrl_report_t hid_ctrl;

	hid_ctrl.id = HID_REPORT_ID_FEATURE_READ_VERSION;

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS,
		LIBUSB_REQUEST_GET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT | HID_REPORT_ID_FEATURE_READ_VERSION,
		0, hid_ctrl.buffer, sizeof(hid_ctrl.buffer), 1000);

	printf("Firmware Version: %u.%u\n", hid_ctrl.buffer[0], hid_ctrl.buffer[1]);

}

void read_pwm()
{
	static hid_ctrl_report_t hid_ctrl;

	hid_ctrl.id = HID_REPORT_ID_FEATURE_READ_WRITE_BR;

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS,
		LIBUSB_REQUEST_GET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT | HID_REPORT_ID_FEATURE_READ_WRITE_BR,
		0, hid_ctrl.buffer, sizeof(hid_ctrl.buffer), 1000);

	printf("PWM_USB:%-5u PWM_BT:%u\n", hid_ctrl.buffer[0], hid_ctrl.buffer[1]);

}

void write_pwm(int pwm_USB, int pwm_BT)
{
	static hid_ctrl_report_t hid_ctrl;

	memset(hid_ctrl.buffer, 0, sizeof(hid_ctrl.buffer));

	hid_ctrl.id = HID_REPORT_ID_FEATURE_READ_WRITE_BR;

	if ((pwm_USB < 0 || pwm_USB > 255) || (pwm_BT < 0 || pwm_BT > 255))
	{
		printf("Value out of range, no changes applied. Exiting.\n");
		return;
	}

	hid_ctrl.payload[0] = (uint8_t)pwm_USB;
	hid_ctrl.payload[1] = (uint8_t)pwm_BT;

	printf("PWM_USB:%-5u PWM_BT:%u\n", pwm_USB, pwm_BT);

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS,
		LIBUSB_REQUEST_SET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT | HID_REPORT_ID_FEATURE_READ_WRITE_BR,
		0, hid_ctrl.buffer, sizeof(hid_ctrl.buffer), 1000);

}

void read_debounce()
{
	static hid_ctrl_report_t hid_ctrl;

	hid_ctrl.id = HID_REPORT_ID_FEATURE_READ_WRITE_DEBOUNCE;

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS,
		LIBUSB_REQUEST_GET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT | HID_REPORT_ID_FEATURE_READ_WRITE_DEBOUNCE,
		0, hid_ctrl.buffer, sizeof(hid_ctrl.buffer), 1000);

	printf("Debounce time: %u ms\n", hid_ctrl.buffer[0]);

}

void write_debounce(int debounce_value)
{
	static hid_ctrl_report_t hid_ctrl;

	memset(hid_ctrl.buffer, 0, sizeof(hid_ctrl.buffer));

	hid_ctrl.id = HID_REPORT_ID_FEATURE_READ_WRITE_DEBOUNCE;

	if ((debounce_value < 1 || debounce_value > 255))
	{
		printf("Value out of range, no changes applied. Exiting.\n");
		return;
	}

	hid_ctrl.payload[0] = (uint8_t)debounce_value;

	printf("Debounce time: %u ms\n", debounce_value);

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS,
		LIBUSB_REQUEST_SET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT | HID_REPORT_ID_FEATURE_READ_WRITE_DEBOUNCE,
		0, hid_ctrl.buffer, sizeof(hid_ctrl.buffer), 1000);
}

uint8_t* read_macros()
{
	static hid_macro_data_report_t macros;
	uint8_t bad_value1 = 0, bad_value2 = 0, macro_cnt = 0;

	macros.id = HID_REPORT_ID_FEATURE_READ_WRITE_MACROS;

	libusb_control_transfer(handle, LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS |
		LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_GET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT |
		HID_REPORT_ID_FEATURE_READ_WRITE_MACROS, 0, macros.buffer, sizeof(macros.buffer), 1000);

	// don't get it twisted: libusb_control_transfer() doesn't return a report id,
	// so macro data starts at address 0, not 1
	for (uint8_t i = 0; i < NUM_MACROKEYS*LEN_MACRO; i++)
	{
		if (macros.buffer[i] == 0) bad_value1++;
		if (macros.buffer[i] == 0xff) bad_value2++;
	}

	if (bad_value1 == NUM_MACROKEYS*LEN_MACRO || bad_value2 == NUM_MACROKEYS*LEN_MACRO)
		return NULL;


	printf("Macro key table\n\n");
	printf("            Mods%-3cRsvd%-3cKey1%-3cKey2%-3cKey3%-3cKey4%-3cKey5%-3cKey6\n\n", '\0', '\0', '\0', '\0', '\0', '\0', '\0');
	printf("Macro %-6u", ++macro_cnt);

	for (uint8_t i = 0; i < NUM_MACROKEYS*LEN_MACRO; i++)
	{
		printf("%-7X", macros.buffer[i]);
		if (i && i != NUM_MACROKEYS*LEN_MACRO - 1)
			if ((i + 1) % 8 == 0)
			{
				printf("\n");
				printf("Macro %-6u", ++macro_cnt);
			}
	}
	printf("\n\n");

	return macros.buffer;
}

uint8_t *read_layout(bool display_names_flag)
{
	hid_layout_data_report_t layout;
	static uint8_t page_buffer[2816];
	char string_buf[16];
	uint8_t row_cnt, i, j, k, nlayers;

	memset(page_buffer, 0, sizeof(page_buffer));
	layout.id = HID_REPORT_ID_FEATURE_READ_WRITE_LAYOUT;

	// repeat call until all pages transferred
	while (1)
	{
		libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN |
			LIBUSB_REQUEST_TYPE_CLASS, LIBUSB_REQUEST_GET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT |
			HID_REPORT_ID_FEATURE_READ_WRITE_LAYOUT, 0, layout.buffer, sizeof(layout.buffer), 1000);

		// check if buffer empty
		if (layout.num_pgs == 0) break;

		// decrease page count by 1 to calculate correct offset value
		memcpy(page_buffer + SPM_PAGESIZE*(layout.pg_cnt - 1), layout.page_data, SPM_PAGESIZE);

		if (layout.pg_cnt == layout.num_pgs)
			break;
	}

	// first byte of page buffer contains number of layers
	nlayers = page_buffer[0];

	if (nlayers == 0)
	{
		printf("No layers configured.\n");
		return NULL;
	}

	if (nlayers > 8)
	{
		printf("More than 8 layers reported, bad flash value!\n");
		return NULL;
	}

	printf("Number of layers: %d", nlayers);
	printf("\n\n");

	/*for (uint16_t i = 1; i < nlayers * NUMKEYS + 1; i++)
	printf("%-10u", *((uint16_t*)char_ctr_buf + i));*/

	printf("Layer 1");
	printf("\n\n");
	printf("    ");

	for (j = 0; j < NUMCOLS; j++)
	{
		if (display_names_flag == true)
			printf("C%-7u", j);
		else
			printf("C%-5u", j);
	}

	printf("\n\n");
	row_cnt = 0;
	printf("R%u  ", row_cnt++);
	for (k = 0; k < nlayers; k++)
	{

		for (i = 0; i < NUMKEYS; i++)
		{
			if(display_names_flag == true)
			{
				for (uint8_t l = 0; l < sizeof(hid_code_array) / 2; l++)
				{
					if (*((uint16_t*)(page_buffer + 1) + i + k*NUMKEYS) == hid_code_array[l])
					{
						strcpy(string_buf, vk_name_abbrev_array[l]);
						break;
					}
					else
						strcpy(string_buf, "-");
				}

				printf("%-8s", string_buf);
				memset(string_buf, 0, sizeof(string_buf));
			}
			else
				printf("%-6X", *((uint16_t*)(page_buffer + 1) + i + k*NUMKEYS));			
			
			if (i && i != NUMKEYS - 1)
				if ((i + 1) % 20 == 0)
				{
					printf("\n\r");
					printf("R%u  ", row_cnt++);
				}
		}
		if (i == NUMKEYS && k == nlayers - 1) break;
		row_cnt = 0;
		printf("\n\n");
		printf("Layer %u", k + 2);
		printf("\n\n");
		printf("    ");
		for (j = 0; j < NUMCOLS + 1; j++)
		{
			if (display_names_flag == true)
				printf("C%-7u", j);
			else
				printf("C%-5u", j);
		}
		printf("\n\n");
		printf("R%u  ", row_cnt++);
	}
	printf("\n");

	return page_buffer;
}

void write_layout(uint8_t nlayers, uint16_t *p_layout_array)
{
	static hid_layout_data_report_t layout;
	// 8 layers maximum byte count + 1 byte number of layers
	static uint8_t page_buffer[2816];
	static uint16_t len;

	memset(page_buffer, -1, sizeof(page_buffer));

	// calculate length of layout array
	len = nlayers*NUMKEYS * 2;
	// copy layout data to page buffer
	page_buffer[0] = nlayers;
	for (uint16_t i = 1, j = 0; j < len; i++, j++)
		*(page_buffer + i) = *((uint8_t*)p_layout_array + j);

	layout.id = HID_REPORT_ID_FEATURE_READ_WRITE_LAYOUT;
	// calculate number of pages necessary + 1 byte to store number of layers
	layout.num_pgs = 1 + (1 + NUMKEYS * 2 * nlayers) / SPM_PAGESIZE;
	// set current page, starting with 1
	layout.pg_cnt = 1;

	while (1)
	{
		// fill layout buffer and send
		memcpy(layout.page_data, page_buffer + SPM_PAGESIZE*(layout.pg_cnt - 1), SPM_PAGESIZE);

		libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS,
			LIBUSB_REQUEST_SET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT | HID_REPORT_ID_FEATURE_READ_WRITE_LAYOUT,
			0, layout.buffer, sizeof(layout.buffer), 1000);

		if (layout.pg_cnt == layout.num_pgs) break;
		layout.pg_cnt++;

	}
}

void write_macros(uint8_t *p_char_ctrl_buf)
{
	hid_macro_data_report_t macros;

	macros.id = HID_REPORT_ID_FEATURE_READ_WRITE_MACROS;
	memcpy(macros.macro_data, p_char_ctrl_buf, NUM_MACROKEYS*LEN_MACRO);

	libusb_control_transfer(handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS |
		LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_SET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT |
		HID_REPORT_ID_FEATURE_READ_WRITE_MACROS, 0, macros.buffer, sizeof(macros.buffer), 1000);
}

void update_firmware(uint8_t *hexfile_namestring)
{
	char dataBuffer[65536 + 256];    /* buffer for file binary data */
	hid_page_data_report_t hid_data;				// buffer for page data
	hid_ctrl_report_t hid_ctrl;			// buffer for control data

	int  startAddress, endAddress;

	memset(hid_ctrl.buffer, 0, sizeof(hid_ctrl.buffer));
	memset(dataBuffer, -1, sizeof(dataBuffer));
	memset(hid_data.buffer, 0, sizeof(hid_data.buffer));
	// for page data report
	hid_data.id = HID_REPORT_ID_PAGE_DATA;
	// for control report to reboot device
	hid_ctrl.id = HID_REPORT_ID_EXIT_BOOTLOADER;

	startAddress = sizeof(dataBuffer);
	endAddress = 0;


	parseIntelHex(hexfile_namestring, dataBuffer, &startAddress, &endAddress);
	if (startAddress >= endAddress)
	{
		fprintf(stderr, "No data in input file, exiting.\n");
		return;
	}

	printf("Updating firmware\n\n");

	int mask = SPM_PAGESIZE - 1;

	startAddress &= ~mask;                  /* round down */
	endAddress = (endAddress + mask) & ~mask;  /* round up */

	while (startAddress < endAddress)
	{

		hid_data.page_address = startAddress;
		memcpy(hid_data.page_data, dataBuffer + startAddress, SPM_PAGESIZE);

		printf("Address: 0x%05x ... 0x%05x%5c", startAddress, startAddress + SPM_PAGESIZE, '\0');
		printf("%u%% of page data transferred\r", (int)((float)(startAddress + SPM_PAGESIZE) / (float)endAddress*(float)100));
		fflush(stdout);

		// send set_report request, feature type
		libusb_control_transfer(handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS |
			LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_SET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT |
			HID_REPORT_ID_PAGE_DATA, 0, hid_data.buffer, sizeof(hid_data.buffer), 1000);
		startAddress += SPM_PAGESIZE;
	}

	printf("\n\n");
	printf("Done. Press a key to reboot\n");
	while (1)
	{
		if (_kbhit())
		{
			// eat key pressed
			_getch();

			// reboot
			libusb_control_transfer(handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS |
				LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_SET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT |
				HID_REPORT_ID_EXIT_BOOTLOADER, 0, hid_ctrl.buffer, sizeof(hid_ctrl.buffer), 1000);
			break;
		}
	}

}

void enter_bootloader()
{
	static hid_ctrl_report_t hid_ctrl;
	memset(hid_ctrl.buffer, 0, sizeof(hid_ctrl.buffer));
	hid_ctrl.id = HID_REPORT_ID_FEATURE_ENTER_BOOTLOADER;

	printf("Entering bootloader\n\r");
	libusb_control_transfer(handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | \
		LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_SET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT | \
		HID_REPORT_ID_FEATURE_ENTER_BOOTLOADER, 0, hid_ctrl.buffer, sizeof(hid_ctrl.buffer), 1000);
}

void exit_bootloader()
{
	static hid_ctrl_report_t hid_ctrl;
	memset(hid_ctrl.buffer, 0, sizeof(hid_ctrl.buffer));
	hid_ctrl.id = HID_REPORT_ID_EXIT_BOOTLOADER;

	libusb_control_transfer(handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS |
		LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_SET_REPORT, LIBUSB_REQUEST_FEATURE_REPORT |
		HID_REPORT_ID_EXIT_BOOTLOADER, 0, hid_ctrl.buffer, sizeof(hid_ctrl.buffer), 1000);

	printf("Exiting bootloader\n\r");
}