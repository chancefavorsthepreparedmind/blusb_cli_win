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

/*
* Command line tool for 'BlUSB' - a programmable Bluetooth-USB-Hybrid keyboard firmware for AVR µCs
*
* Created: 15.04.2016 14:02:35
* Last changes: 10.09.2019
* Author : Jörn Giest
*
*
*	Notes and observations
*
* 1. Code compiles alright, but needs cleaning up
* 
* 2. The only Libusb call used in this project to transfer data is libusb_control_transfer().
*	 While using the call is mostly intuitive, some aspects of it are not. At least on Windows,
*	 it is mandatory that the first byte of the transmit buffer contain the report id.
*	 The first byte of the receive buffer, however, does not contain the report id but the
*	 first byte of payload data (as one would expect).
*	  
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "blusb_cmd_univ_comp_nodrv.h"

// Product id and Vendor id for IBM Enhanced Performance Keyboard
const uint16_t vendor = 0x04b3, product = 0x301c;

libusb_device **dev_list;
libusb_device *dev = NULL;
libusb_device_handle *handle = NULL;
struct libusb_device_descriptor dev_descr;
uint8_t verbose = 0;

void print_usage(uint8_t **argv)
{
	printf("\nUsage: %s [-option] [-optional parameter] [filename]\n\n", argv[0]);
	printf("Options:\n\n");
	printf("-read_matrix\n\r");
	printf("-read_pwm\n\r");
	printf("-write_pwm [value_USB value_BT] (Valid range: 0-255)\n\r");
	printf("-read_debounce\n\r");
	printf("-write_debounce [value] (Valid range: 1-255)\n\r");
	printf("-read_macros\n\r");
	printf("-write_macros [filename]\n\r");
	printf("-read_layout [-no_print] [-names]\n\r");
	printf("-write_layout [filename]\n\r");
	printf("-configure_layout [-update filename]\n\r");
	printf("-read_version\n\r");
	printf("-update_firmware [filename]\n\r");
	printf("-enter_bootloader\n\r");
	printf("-exit_bootloader\n\r");
	printf("-layout_dec_to_hex [filename]\n\r");
	printf("-macros_dec_to_hex [filename]\n\r");
	printf("-h, --help, /?\n\r");
}

int main(int argc, uchar **argv)
{

	uint8_t nlayers = 0;			// dummy arguments for configure_layout() if no existing key file is used
	uint16_t *p_layout_array_keyfile = NULL;
	uint8_t *p_macros_array_macrosfile = NULL;
	uint8_t scan_buf[256] = { 0 };
	unsigned char Manufacturer_String[255];
	unsigned char Product_String[255];
	int err = 0;

	printf("\n\r+++ Blusb configuration tool +++\n\n");

	if (argc < 2)
	{
		print_usage(argv);
		exit(0);
	}

	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0)
	{
		print_usage(argv);
		exit(0);
	}

	libusb_init(NULL);

	uint8_t cnt = libusb_get_device_list(NULL, &dev_list);

	//loop through device list and find device
	for (uint8_t i = 0; i < cnt; i++)
	{
		dev = dev_list[i];
		libusb_get_device_descriptor(dev, &dev_descr);
		
		/*printf("No. %d   Vendor: %x   Name: %s   Product: %x   Name: %s   Bus: %d   Address: %d\n",
			i, dev_descr.idVendor, Manufacturer_String, dev_descr.idProduct, Product_String,
		libusb_get_bus_number(dev), libusb_get_device_address(dev));*/
		
		if ((vendor == dev_descr.idVendor) && (product == dev_descr.idProduct))
		{
			printf("Device found. Trying to get handle...\n\n");

			err = libusb_open(dev, &handle);
			if (err)
			{
				printf("LIBUSB error code: %s", libusb_error_name(err));
				printf("\n\n");
			}
			else
			{
				libusb_get_string_descriptor_ascii(handle, dev_descr.iManufacturer, Manufacturer_String, 255);
				libusb_get_string_descriptor_ascii(handle, dev_descr.iProduct, Product_String, 255);
				printf("Device info: No. %d   Vendor: %x   Name: %s   Product: %x   Name: %s   Bus: %d   Address: %d\n\n",
					i, dev_descr.idVendor, Manufacturer_String, dev_descr.idProduct, Product_String,
					libusb_get_bus_number(dev), libusb_get_device_address(dev));
			}
			
			libusb_free_device_list(dev_list, 1);
			break;
		}
	}

	if (handle != NULL)
	{

		if (argc > 3 && strcmp(argv[1], "-configure_layout") == 0 && strcmp(argv[2], "-update") == 0)
		{
			nlayers = parse_keyfile(argv[3]);
			if (nlayers == 0)
			{
				printf("Zero or more than eight layers defined. Check layout file.\n");
			}
			else
			{
				printf("Key file parsed. Number of layers: %d\n", nlayers);
				p_layout_array_keyfile = fill_layout_array(nlayers, false);
				printf("Layout array filled.\n\n");
				configure_layout(nlayers, p_layout_array_keyfile);
			}
		}
		else
			if (argc > 1 && strcmp(argv[1], "-layout_dec_to_hex") == 0)
			{
				if(argv[2])
				{
					nlayers = parse_keyfile(argv[2]);
					if (nlayers == 0)
					{
						printf("Zero or more than eight layers defined. Check layout file.\n");
					}
					else
					{
						printf("Key file parsed. Number of layers: %d\n", nlayers);
						p_layout_array_keyfile = fill_layout_array(nlayers, true);
						printf("Layout array filled.\n\n");
						print_keyfile(p_layout_array_keyfile, nlayers);
					}
				}
				else
				{
					printf("You must provide a filename!\n\r");
					print_usage(argv);
				}
		}	
		else
		if (argc > 1 && strcmp(argv[1], "-macros_dec_to_hex") == 0)
		{
			if (argv[2])
			{
				parse_macrosfile(argv[2]);
				printf("Macros file parsed.\n");
				p_macros_array_macrosfile = fill_macros_array(true);
				printf("Macro array filled.\n\n");
				print_macrosfile(p_macros_array_macrosfile);
			}
			else
			{
				printf("You must provide a filename!\n\r");
				print_usage(argv);
			}
		}
		else
		if (argc > 1 && strcmp(argv[1], "-configure_layout") == 0)
			configure_layout(nlayers, p_layout_array_keyfile);
		else
		if (argc > 1 && strcmp(argv[1], "-read_matrix") == 0)
		{
			verbose = 1;
			printf("Press Esc to quit\n\n");

			while (1)
			{
				read_matrix_pos();
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
				{
					printf("\n");
					break;
				}
			}

		}
		else
		if (argc > 1 && strcmp(argv[1], "-read_version") == 0)
		{
			read_version();
		}
		else
		if (argc > 1 && strcmp(argv[1], "-read_pwm") == 0)
		{
			read_pwm();
		}
		else
		if (argc > 3 && strcmp(argv[1], "-write_pwm") == 0)
		{
			write_pwm(atoi(argv[2]), atoi(argv[3]));
		}
		else
		if (argc > 1 && strcmp(argv[1], "-read_debounce") == 0)
		{
			read_debounce();
		}
		else
		if (argc > 2 && strcmp(argv[1], "-write_debounce") == 0)
		{
			write_debounce(atoi(argv[2]));
		}
		else
		if (argc > 1 && strcmp(argv[1], "-read_layout") == 0)
		{
			bool display_names_flag = false;
			if (argc > 2)
				for (uint8_t i = 0; i < argc; i++)
					if (strcmp(argv[i], "-names") == 0)
					{
						display_names_flag = true;
						break;
					}
			p_layout_array_keyfile = (uint16_t*)read_layout(display_names_flag);						
			if (argc > 2)
				for (uint8_t i = 0; i < argc; i++)
					if (strcmp(argv[i], "-no_print") == 0)
					{
						p_layout_array_keyfile = NULL;
						break;
					}
			if (p_layout_array_keyfile != NULL)
			{
				// address 0 = nlayers, array data starts at address 1	
				print_keyfile((uint16_t*)((uint8_t*)p_layout_array_keyfile + 1), *(uint8_t*)p_layout_array_keyfile);
			}
			
		}
		else
		if (argc > 1 && strcmp(argv[1], "-write_layout") == 0)
		{
			if (argv[2])
			{
				nlayers = parse_keyfile(argv[2]);
				if (nlayers == 0)
				{
					printf("Zero or more than eight layers defined. Check layout file.\n");
				}
				else
				{
					printf("Key file parsed. Number of layers: %d\n", nlayers);
					p_layout_array_keyfile = fill_layout_array(nlayers, false);
					printf("Layout array filled.\n");
					write_layout(nlayers, p_layout_array_keyfile);
					printf("Done.\n");
				}
			}
			else
			{
				printf("You must provide a filename!\n\n");
				print_usage(argv);
			}
		}
		else
		if (argc > 1 && strcmp(argv[1], "-read_macros") == 0)
		{
			p_macros_array_macrosfile = read_macros();
			if(p_macros_array_macrosfile != NULL)
				print_macrosfile(p_macros_array_macrosfile);
			else
				printf("All fields zero. No macros programmed yet!\n");

		}
		else
		if (argc > 1 && strcmp(argv[1], "-write_macros") == 0)
		{
			if (argv[2])
			{
				if (parse_macrosfile(argv[2]) == true)
				{
					printf("\nMacro file parsed.");
					p_macros_array_macrosfile = fill_macros_array(false);
					printf("\nMacro array filled.");
					write_macros(p_macros_array_macrosfile);
					printf("\nDone.\n");
				}
			}
			else
			{
				printf("You must provide a filename!\n\r");
				print_usage(argv);
			}
		}
		else
		if (argc > 1 && strcmp(argv[1], "-update_firmware") == 0)
		{
			if (argv[2])
			{
				update_firmware(argv[2]);
			}
			else
			{
				printf("You must provide a filename!\n\r");
				print_usage(argv);
			}
			
		}
		else
		if (argc > 1 && strcmp(argv[1], "-enter_bootloader") == 0)
		{
			enter_bootloader();
		}
		else
		if (argc > 1 && strcmp(argv[1], "-exit_bootloader") == 0)
		{
			exit_bootloader();
		}
		else
		{
			printf("No valid option specified!\n");
			print_usage(argv);
		}
			
	}
	else
	{
		fprintf(stderr, "Can't get handle for specified device!\n");
	}
	
	libusb_close(handle);
	libusb_exit(NULL);
	exit(0);	
}