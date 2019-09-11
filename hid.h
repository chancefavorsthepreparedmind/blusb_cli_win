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



#include <stdint.h>

uint16_t read_matrix_pos(void);
void read_pwm(void);
void read_version(void);
void write_pwm(int pwm_USB, int pwm_BT);
void read_debounce(void);
void write_debounce(int debounce_value);
uint8_t *read_layout(bool display_names_flag);
void write_layout(uint8_t nlayers, uint16_t *p_layout_array);
uint8_t *read_macros(void);
void write_macros(uint8_t *p_char_ctrl_buf);
void update_firmware(uint8_t *hexfile_namestring);
void enter_bootloader(void);
void exit_bootloader(void);


// we need to make sure the members of the struct and union are byte-aligned and packed contiguously
// to be able to cast them over the memory buffer
#pragma pack(push, 1)
typedef union
{
	struct
	{
		uint8_t id;
		uint16_t page_address;
		uint8_t page_data[256];
	};
	uint8_t buffer[259];

} hid_page_data_report_t;

typedef union
{
	struct
	{
		uint8_t id;
		uint8_t num_pgs;
		uint8_t pg_cnt;
		uint8_t page_data[256];
	};
	uint8_t buffer[259];

} hid_layout_data_report_t;

typedef union
{
	struct
	{
		uint8_t id;
		uint8_t macro_data[192];
	};
	uint8_t buffer[193];

} hid_macro_data_report_t;

typedef union
{
	struct
	{
		uint8_t id;
		uint8_t payload[7];
	};
	uint8_t buffer[8];
} hid_ctrl_report_t;

#pragma pack(pop)

// interface numbers application
#define HID_IFACE_NUMBER_KBD							0						
#define HID_IFACE_NUMBER_MEDIA_CTRL_FEAT				1						

// HID report ids for bootloader
#define HID_REPORT_ID_PAGE_DATA							0x01							
#define HID_REPORT_ID_EXIT_BOOTLOADER					0x02						

// HID report ids for application

// interface 0 - no ids used
#define HID_REPORT_ID_BOOT								0x00							
#define HID_REPORT_ID_KEYBOARD							0x01							

// interface 1
#define HID_REPORT_ID_MEDIA													0x02							
#define HID_REPORT_ID_SYSCTRL												0x03							
#define HID_REPORT_ID_FEATURE_READ_WRITE_LAYOUT			0x01		
#define HID_REPORT_ID_FEATURE_READ_WRITE_MACROS			0x02		
#define HID_REPORT_ID_FEATURE_READ_MATRIX						0x03				
#define HID_REPORT_ID_FEATURE_READ_WRITE_BR					0x04			
#define HID_REPORT_ID_FEATURE_READ_VERSION						0x05				
#define HID_REPORT_ID_FEATURE_READ_WRITE_DEBOUNCE		0x06		
#define HID_REPORT_ID_FEATURE_ENTER_BOOTLOADER				0x07			

#define LIBUSB_REQUEST_GET_REPORT						1
#define LIBUSB_REQUEST_SET_REPORT						9
#define LIBUSB_REQUEST_IN_REPORT						(uint16_t)1<<8	
#define LIBUSB_REQUEST_OUT_REPORT						(uint16_t)2<<8
#define LIBUSB_REQUEST_FEATURE_REPORT					(uint16_t)3<<8

#define SPM_PAGESIZE									256

// vendor request ids
#define USB_ENTER_BOOTLOADER							0xFF
#define USB_EXIT_BOOTLOADER								0xFE
#define USB_ENABLE_VENDOR_RQ							0x11
#define USB_DISABLE_VENDOR_RQ							0x10
#define USB_READ_BR										0x20
#define USB_WRITE_BR									0x21
#define USB_READ_MATRIX									0x30
#define USB_READ_LAYOUT									0x40
#define USB_WRITE_LAYOUT								0x41
#define USB_READ_DEBOUNCE								0x50
#define USB_WRITE_DEBOUNCE								0x51
#define USB_READ_MACROS									0x60
#define USB_WRITE_MACROS								0x61
#define USB_READ_VERSION								0x70