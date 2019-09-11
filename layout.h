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


#include <stdbool.h>
#include <stdint.h>

extern const uint8_t vk_code_array[104];
extern const char vk_name_array[104][16];
extern const char vk_name_abbrev_array[104][16];
extern const uint16_t hid_code_array[104];


void print_keyfile(uint16_t *p_layout_array, uint8_t nlayers);
void print_macrosfile(uint8_t *p_macros_array);
uint16_t *fill_layout_array(uint8_t nlayers, bool convert_dec_hex);
uint8_t *fill_macros_array(bool convert_dec_hex);
uint8_t parse_keyfile(uint8_t *keyfile_namestring);
void parse_macrosfile(uint8_t *keyfile_namestring);
void configure_layout(uint8_t nlayers, uint16_t *p_layout_array_keyfile);


#define NUMLAYERS_MAX		8
#define NUMLAYERS			1
#define NUMKEYS				160
#define NUMROWS				8
#define NUMCOLS				20
#define NUM_MACROKEYS		24
#define LEN_MACRO			8

//Keycode defines

/************************************************************************/
/*                      Additional virtual keycode defines				*/
/************************************************************************/

#define VK_KEY_0	0x30
#define VK_KEY_1	0x31
#define VK_KEY_2	0x32
#define VK_KEY_3	0x33
#define VK_KEY_4	0x34
#define VK_KEY_5	0x35
#define VK_KEY_6	0x36
#define VK_KEY_7	0x37
#define VK_KEY_8	0x38 
#define VK_KEY_9	0x39
#define VK_KEY_A	0x41
#define VK_KEY_B	0x42
#define VK_KEY_C	0x43
#define VK_KEY_D	0x44
#define VK_KEY_E	0x45
#define VK_KEY_F	0x46
#define VK_KEY_G	0x47
#define VK_KEY_H	0x48
#define VK_KEY_I	0x49
#define VK_KEY_J	0x4A
#define VK_KEY_K	0x4B
#define VK_KEY_L	0x4C
#define VK_KEY_M	0x4D
#define VK_KEY_N	0x4E
#define VK_KEY_O	0x4F
#define VK_KEY_P	0x50
#define VK_KEY_Q	0x51
#define VK_KEY_R	0x52
#define VK_KEY_S	0x53
#define VK_KEY_T	0x54
#define VK_KEY_U	0x55
#define VK_KEY_V	0x56
#define VK_KEY_W	0x57
#define VK_KEY_X	0x58
#define VK_KEY_Y	0x59
#define VK_KEY_Z	0x5A


/************************************************************************/
/*                          ASCII keycode defines                       */
/************************************************************************/

#define ASCII_ESC			0x1b
#define ASCII_F1			0x3b
#define ASCII_F2			0x3c
#define ASCII_F3			0x3d
#define ASCII_F4			0x3e
#define ASCII_F5			0x3f
#define ASCII_F6			0x40
#define ASCII_F7			0x41
#define ASCII_F8			0x42
#define ASCII_F9			0x43
#define ASCII_F10			0x44
#define ASCII_F11			0x85
#define ASCII_F12			0x86
#define ASCII_PRTSCRN		
#define ASCII_SCRLCK
#define ASCII_PAUSE
#define ASCII_CARET			0x5e
#define ASCII_KB_1			0x31
#define ASCII_KB_2			0x32
#define ASCII_KB_3			0x33
#define ASCII_KB_4			0x34
#define ASCII_KB_5			0x35
#define ASCII_KB_6			0x36
#define ASCII_KB_7			0x37
#define ASCII_KB_8			0x38
#define ASCII_KB_9			0x39
#define ASCII_KB_0			0x30
#define ASCII_GER_SZ		0xe1
#define ASCII_ACUTE_ACC		0xef
#define ASCII_BKSPC			0x08
#define ASCII_TAB			0x09
#define ASCII_Q				0x71
#define ASCII_W				0x77
#define ASCII_E				0x65
#define ASCII_R				0x72
#define ASCII_T				0x74
#define ASCII_Z				0x7a
#define ASCII_U				0x75
#define ASCII_I				0x69
#define ASCII_O				0x6f
#define ASCII_P				0x70
#define ASCII_OPBRK			0x81
#define ASCII_CLSBRK		0x2b
#define ASCII_ENTER			0x0d
#define ASCII_A				0x61
#define ASCII_S				0x73
#define ASCII_D				0x64
#define ASCII_F				0x66
#define ASCII_G				0x67
#define ASCII_H				0x68
#define ASCII_J				0x6a
#define ASCII_K				0x6b
#define ASCII_L				0x6c
#define ASCII_SEMICOLON		0x94
#define ASCII_SGDBLQUOT		0x84
#define ASCII_NUMBER		0x23
#define ASCII_LESS			0x3c
#define ASCII_Y				0x79
#define ASCII_X				0x78
#define ASCII_C				0x63
#define ASCII_V				0x76
#define ASCII_B				0x62
#define ASCII_N				0x6e
#define ASCII_M				0x6d
#define ASCII_COMMA			0x2c
#define ASCII_PERIOD		0x2e
#define ASCII_HYPHEN		0x2d
#define ASCII_SPACE			0x20

/************************************************************************/
/*                        internal keycodes								*/
/************************************************************************/

//TYPE 255: Miscellaneous
#define TYPE_MISC			0xFF
#define JMP_BOOT			0xFF00 //Jump to bootloader
#define CODE_JMP_BOOT		0x00
#define BR_UP				0xFF01 //Increase LED brightness
#define CODE_BR_UP			0x01
#define BR_DOWN				0xFF02 //Decrease LED brightness
#define CODE_BR_DOWN		0x02

// TYPE 7: Macro keys
#define TYPE_MACRO			0x07
#define MACRO_1				0x0700
#define MACRO_2				0x0701
#define MACRO_3				0x0702
#define MACRO_4				0x0703
#define MACRO_5				0x0704
#define MACRO_6				0x0705
#define MACRO_7				0x0706
#define MACRO_8				0x0707
#define MACRO_9				0x0708
#define MACRO_10			0x0709
#define MACRO_11			0x070A
#define MACRO_12			0x070B
#define MACRO_13			0x070C
#define MACRO_14			0x070D
#define MACRO_15			0x070E
#define MACRO_16			0x070F
#define MACRO_17			0x0710
#define MACRO_18			0x0711
#define MACRO_19			0x0712
#define MACRO_20			0x0713
#define MACRO_21			0x0714
#define MACRO_22			0x0715
#define MACRO_23			0x0716
#define MACRO_24			0x0717	

// TYPE 6: System Control
#define TYPE_SYSTEM			0x06
#define SYSCTRL_POWERDOWN	0x0600
#define SYSCTRL_SLEEP		0x0601
#define SYSCTRL_WAKEUP		0x0602

// TYPE 5: Layer Toggle
#define TYPE_TOGGLE			0x05
#define TLAYER_0			0x0500
#define TLAYER_1			0x0501
#define TLAYER_2			0x0502
#define TLAYER_3			0x0503
#define TLAYER_4			0x0504
#define TLAYER_5			0x0505
#define TLAYER_6			0x0506
#define TLAYER_7			0x0507

// TYPE 4: Momentary Layer Actuation
#define TYPE_MOMENTARY 		0x04
#define MLAYER_0			0x0400
#define MLAYER_1			0x0401
#define MLAYER_2			0x0402
#define MLAYER_3			0x0403
#define MLAYER_4			0x0404
#define MLAYER_5			0x0405
#define MLAYER_6			0x0406
#define MLAYER_7			0x0407

// TYPE 3: Reserved for Mouse keys
#define TYPE_MOUSE   		0x03

// TYPE 2: Media keys
#define TYPE_MEDIA   		0x02
#define MEDIA_PLAY			0x0200
#define MEDIA_PAUSE			0x0201
#define MEDIA_NEXT			0x0202
#define MEDIA_PREVIOUS		0x0203
#define MEDIA_STOP			0x0204
#define MEDIA_MUTE			0x0205
#define MEDIA_VOLUP			0x0206
#define MEDIA_VOLDOWN		0x0207
#define	MEDIA_EMAIL			0x0208
#define	MEDIA_CALC			0x0209
#define	MEDIA_BROWSER		0x020A
#define	MEDIA_SEARCH		0x020B
#define	MEDIA_HOME			0x020C
#define	MEDIA_BACK			0x020D
#define	MEDIA_FORWARD		0x020E
#define	MEDIA_REFRESH		0x020F

// TYPE 1: Keyboard modifiers
#define TYPE_MOD   		0x01
#define KB_CTRL			0x0101
#define KB_SHIFT		0x0102
#define KB_ALT			0x0104
#define KB_GUI			0x0108
#define KB_LEFT_CTRL	0x0101
#define KB_LEFT_SHIFT	0x0102
#define KB_LEFT_ALT		0x0104
#define KB_LEFT_GUI		0x0108
#define KB_RIGHT_CTRL	0x0110
#define KB_RIGHT_SHIFT	0x0120
#define KB_RIGHT_ALT	0x0140
#define KB_RIGHT_GUI	0x0180
//Space friendly Versions
#define KB_LCTRL		0x0101
#define KB_LSHFT		0x0102
#define KB_LALT			0x0104
#define KB_LGUI			0x0108
#define KB_RCTRL		0x0110
#define KB_RSHFT		0x0120
#define KB_RALT			0x0140
#define KB_RGUI			0x0180

/************************************************************************/
/*                        external keycodes								*/
/************************************************************************/

/* The following key codes are identical with the codes in */
/* the USB HID Keyboard/Keypad usage table.				   */

//TYPE 0: Keyboard keycodes
#define TYPE_KEY				0
#define KB_NONE					0
#define	KB_ERROR_ROLLOVER		0x01
#define	KB_POST_FAIL			0x02
#define	KB_ERROR_UNDEFINED		0x03
#define KB_A					0x04
#define KB_B					0x05
#define KB_C					0x06
#define KB_D					0x07
#define KB_E					0x08
#define KB_F					0x09
#define KB_G					0x0A
#define KB_H					0x0B
#define KB_I					0x0C
#define KB_J					0x0D
#define KB_K					0x0E
#define KB_L					0x0F
#define KB_M					0x10
#define KB_N					0x11
#define KB_O					0x12
#define KB_P					0x13
#define KB_Q					0x14
#define KB_R					0x15
#define KB_S					0x16
#define KB_T					0x17
#define KB_U					0x18
#define KB_V					0x19
#define KB_W					0x1A
#define KB_X					0x1B
#define KB_Y					0x1C
#define KB_Z					0x1D
#define KB_1					0x1E
#define KB_2					0x1F
#define KB_3					0x20
#define KB_4					0x21
#define KB_5					0x22
#define KB_6					0x23
#define KB_7					0x24
#define KB_8					0x25
#define KB_9					0x26
#define KB_0					0x27
#define KB_ENTER				0x28	
#define KB_ESC					0x29
#define KB_BKSPC				0x2A	
#define KB_TAB					0x2B
#define KB_SPACE				0x2C	
#define KB_MINUS				0x2D
#define KB_EQUAL				0x2E
#define KB_LBRCE				0x2F
#define KB_RBRCE				0x30
#define KB_BSLSH				0x31
#define KB_NUMBER				0x32 // Non-US # and ~ 
#define KB_SMCLN				0x33
#define KB_QUOTE				0x34
#define KB_TILDE				0x35
#define KB_COMMA				0x36
#define KB_DOT					0x37
#define KB_SLASH				0x38
#define KB_CAPLK				0x39
#define KB_F1					0x3A
#define KB_F2					0x3B
#define KB_F3					0x3C
#define KB_F4					0x3D
#define KB_F5					0x3E
#define KB_F6					0x3F
#define KB_F7					0x40
#define KB_F8					0x41
#define KB_F9					0x42
#define KB_F10					0x43
#define KB_F11					0x44
#define KB_F12					0x45
#define KB_PSCRN				0x46		
#define KB_SCRLK				0x47		
#define KB_PAUSE				0x48
#define KB_INS					0x49	
#define KB_HOME					0x4A
#define KB_PGUP					0x4B
#define KB_DEL					0x4C
#define KB_END					0x4D
#define KB_PGDN					0x4E	
#define KB_RIGHT				0x4F
#define KB_LEFT					0x50
#define KB_DOWN					0x51
#define KB_UP					0x52
#define KB_NUMLK				0x53
#define KP_SLASH				0x54	
#define KP_ASTRX				0x55	
#define KP_MINUS				0x56	
#define KP_PLUS					0x57	
#define KP_ENTER				0x58	
#define KP_1					0x59
#define KP_2					0x5A
#define KP_3					0x5B	
#define KP_4					0x5C	
#define KP_5					0x5D	
#define KP_6					0x5E	
#define KP_7					0x5F	
#define KP_8					0x60	
#define KP_9					0x61
#define KP_0					0x62	
#define KP_DOT					0x63
#define KB_PIPE 				0x64
#define KB_APP					0x65
#define KB_POWER 				0x66
#define KP_EQUAL				0x67
#define KB_F13					0x68
#define KB_F14					0x69
#define KB_F15					0x6A
#define KB_F16					0x6B
#define KB_F17					0x6C
#define KB_F18					0x6D
#define KB_F19					0x6E
#define KB_F20					0x6F
#define KB_F21					0x70
#define KB_F22					0x71
#define KB_F23					0x72
#define KB_F24					0x73
#define KB_EXECUTE				0x74
#define KB_HELP					0x75
#define KB_MENU					0x76
#define KB_SELECT				0x77
#define KB_STOP					0x78
#define KB_AGAIN				0x79
#define KB_UNDO					0x7A
#define KB_CUT					0x7B
#define KB_COPY					0x7C
#define KB_PASTE				0x7D
#define KB_FIND					0x7E
#define KB_MUTE					0x7F
#define KB_VOLUMEUP				0x80
#define KB_VOLUMEDOWN			0x81
#define KB_LOCKINGCAPSLOCK		0x82
#define KB_LOCKINGNUMLOCK		0x83
#define KB_LOCKINGSCROLLLOCK	0x84
#define KP_COMMA				0x85
#define KP_EQUALSIGN			0x86
#define KB_INTERNATIONAL1		0x87
#define KB_INTERNATIONAL2		0x88
#define KB_INTERNATIONAL3		0x89
#define KB_INTERNATIONAL4		0x8A
#define KB_INTERNATIONAL5		0x8B
#define KB_INTERNATIONAL6		0x8C
#define KB_INTERNATIONAL7		0x8D
#define KB_INTERNATIONAL8		0x8E
#define KB_INTERNATIONAL9		0x8F
#define KB_LANG1				0x90
#define KB_LANG2				0x91
#define KB_LANG3				0x92
#define KB_LANG4				0x93
#define KB_LANG5				0x94
#define KB_LANG6				0x95
#define KB_LANG7				0x96
#define KB_LANG8				0x97
#define KB_LANG9				0x98
#define KB_ALTERNATE_ERASE		0x99
#define KB_SYSREQ				0x9A
#define KB_CANCEL				0x9B
#define KB_CLEAR				0x9C
#define KB_PRIOR				0x9D
#define KB_RETURN				0x9E
#define KB_SEPARATOR			0x9F
#define KB_OUT					0xA0
#define KB_OPER					0xA1
#define KB_CLEAR_AGAIN			0xA2
#define KB_CRSEL				0xA3
#define KB_EXSEL				0xA4

#define KB_RESERVED1			0xA5-0xCF	

#define KP_00					0xB0
#define KP_000					0xB1
#define KB_THOUSANDS_SEPARATOR	0xB2
#define KB_DECIMAL_SEPARATOR	0xB3
#define KB_CURRENCY_UNIT		0xB4
#define KB_CURRENCY_SUBUNIT		0xB5
#define KP_LEFT_PAREN			0xB6
#define KP_RIGHT_PAREN			0xB7
#define KP_LEFT_BRACE			0xB8
#define KP_RIGHT_BRACE			0xB9
#define KP_TAB					0xBA
#define KP_BACKSPACE			0xBB
#define KP_A					0xBC
#define KP_B					0xBD
#define KP_C					0xBE
#define KP_D					0xBF
#define KP_E					0xC0
#define KP_F					0xC1
#define KP_XOR					0xC2
#define KP_POWER				0xC3
#define KP_MOD					0xC4
#define KP_LESS					0xC5
#define KP_GREATER				0xC6
#define KP_BIT_AND				0xC7
#define KP_AND					0xC8
#define KP_BIT_OR				0xC9
#define KP_OR					0xCA
#define KP_COLON				0xCB
#define KP_NUMBER				0xCC
#define KP_SPACE				0xCD
#define KP_AT					0xCE
#define KP_NOT					0xCF
#define KP_MEMORY_STORE			0xD0
#define KP_MEMORY_RECALL		0xD1
#define KP_MEMORY_CLEAR			0xD2
#define KP_MEMORY_ADD			0xD3
#define KP_MEMORY_SUBTRACT		0xD4
#define KP_MEMORY_MULTIPLY		0xD5
#define KP_MEMORY_DIVIDE		0xD6
#define KP_PLUSMINUS			0xD7
#define KP_CLEAR				0xD8
#define KP_CLEAR_ENTRY			0xD9
#define KP_BINARY				0xDA
#define KP_OCTAL				0xDB
#define KP_DECIMAL				0xDC
#define KP_HEXADECIMAL			0xDD

#define KB_RESERVED2			0xDE-0xDF

/* Do not uncomment, included only */
/* for reference				   */
/*************************************
#define KB_LEFT_CTRL			0xE0 *
#define KB_LEFT_SHIFT			0xE1 *
#define KB_LEFT_ALT				0xE2 *
#define KB_LEFT_GUI				0xE3 *
#define KB_RIGHT_CTRL			0xE4 *
#define KB_RIGHT_SHIFT			0xE5 *
#define KB_RIGHT_ALT			0xE6 *
#define KB_RIGHT_GUI			0xE7 *
*************************************/

#define KB_RESERVED3			0xE8-FFFF