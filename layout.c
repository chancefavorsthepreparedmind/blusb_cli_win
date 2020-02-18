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

char string_buffers[NUMLAYERS_MAX][1024] = { 0 };

const uint8_t vk_code_array[] =
{
	VK_ESCAPE, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7,	VK_F8, VK_F9, VK_F10, VK_F11, VK_F12, VK_SNAPSHOT,
	VK_SCROLL, VK_OEM_5, VK_KEY_1, VK_KEY_2, VK_KEY_3, VK_KEY_4, VK_KEY_5, VK_KEY_6, VK_KEY_7, VK_KEY_8,
	VK_KEY_9, VK_KEY_0,	VK_OEM_4, VK_OEM_6, VK_BACK, VK_INSERT,	VK_HOME, VK_PRIOR, VK_TAB, VK_KEY_Q, VK_KEY_W,
	VK_KEY_E, VK_KEY_R,	VK_KEY_T, VK_KEY_Z,	VK_KEY_U, VK_KEY_I,	VK_KEY_O, VK_KEY_P,	VK_OEM_1, VK_OEM_PLUS, VK_RETURN,
	VK_DELETE, VK_END, VK_NEXT, VK_CAPITAL, VK_KEY_A, VK_KEY_S, VK_KEY_D, VK_KEY_F, VK_KEY_G, VK_KEY_H, VK_KEY_J,
	VK_KEY_K, VK_KEY_L,	VK_OEM_3, VK_OEM_7,	VK_OEM_2, VK_LSHIFT, VK_OEM_102, VK_KEY_Y, VK_KEY_X, VK_KEY_C, VK_KEY_V,
	VK_KEY_B, VK_KEY_N, VK_KEY_M, VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_MINUS, VK_RSHIFT, VK_UP, VK_LCONTROL, VK_LWIN,
	VK_LMENU, VK_SPACE,	VK_RMENU, VK_RWIN, VK_APPS, VK_RCONTROL, VK_LEFT, VK_DOWN, VK_RIGHT, VK_NUMLOCK, VK_DIVIDE,
	VK_MULTIPLY, VK_SUBTRACT, VK_ADD, VK_RETURN, VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5,
	VK_NUMPAD6,	VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9, VK_DECIMAL
};

const char vk_name_array[][16] =
{
	"Esc", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "Print Screen", "Scroll Lock",
	"OEM_5", "KB 1", "KB 2", "KB 3", "KB 4", "KB 5", "KB 6", "KB 7", "KB 8", "KB 9", "KB 0", "OEM_4", "OEM_6",
	"Backspace", "Insert", "Home", "Page up", "Tab", "Q", "W", "E", "R", "T", "Z", "U", "I", "O", "P", "OEM_1",
	"OEM_PLUS", "Enter", "Delete", "End", "Page down", "Caps Lock", "A", "S", "D", "F", "G", "H", "J", "K", "L",
	"OEM_3", "OEM_7", "OEM_2", "Left Shift", "OEM_102", "Y", "X", "C", "V", "B", "N", "M", "OEM_COMMA", "OEM_PERIOD",
	"OEM_MINUS", "Right Shift", "Up Arrow", "Left Ctrl", "Left Win", "Left Alt", "Space", "Right Alt",	"Right Win",
	"Win Menu", "Right Ctrl", "Left Arrow", "Down Arrow", "Right Arrow", "Num Lock", "NP_Divide", "NP_Multiply", "NP_Subtract",
	"NP_Add", "Enter", "NP_0", "NP_1", "NP_2", "NP_3", "NP_4", "NP_5", "NP_6", "NP_7", "NP_8", "NP_9",
	"NP_Dot"
};

const char vk_name_abbrev_array[][16] =
{
	"Esc", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "Prnt", "Scrl",
	"Oem_5", "Kb1", "Kb2", "Kb3", "Kb4", "Kb5", "Kb6", "Kb7", "Kb8", "Kb9", "Kb0", "Oem4", "Oem6",
	"Space", "Ins", "Home", "PgUp", "Tab", "Q", "W", "E", "R", "T", "Z", "U", "I", "O", "P", "Oem1",
	"OemPlus", "Enter", "Del", "End", "PgDwn", "Caps", "A", "S", "D", "F", "G", "H", "J", "K", "L",
	"Oem3", "Oem7", "Oem2", "LShft", "Oem102", "Y", "X", "C", "V", "B", "N", "M", "OemCmA", "OemPrd",
	"OemMin", "RShft", "UpArr", "LCtrl", "LWin", "LAlt", "Space", "RAlt",	"RWin",
	"WinMn", "RCtrl", "LArr", "DwnArr", "RArr", "Num", "NpDiv", "NpMul", "NpSub",
	"NpAdd", "Enter", "Np0", "Np1", "Np2", "Np3", "Np4", "Np5", "Np6", "Np7", "Np8", "Np9",
	"NpDot"
};

const uint16_t hid_code_array[] =
{
	KB_ESC, KB_F1, KB_F2, KB_F3, KB_F4, KB_F5, KB_F6, KB_F7, KB_F8, KB_F9, KB_F10, KB_F11, KB_F12, KB_PSCRN, KB_SCRLK,
	KB_TILDE, KB_1, KB_2, KB_3, KB_4, KB_5, KB_6, KB_7, KB_8, KB_9, KB_0, KB_MINUS, KB_EQUAL, KB_BKSPC, KB_INS,
	KB_HOME, KB_PGUP, KB_TAB, KB_Q, KB_W, KB_E, KB_R, KB_T, KB_Z, KB_U, KB_I, KB_O, KB_P, KB_LBRCE, KB_RBRCE,
	KB_ENTER, KB_DEL, KB_END, KB_PGDN, KB_CAPLK, KB_A, KB_S, KB_D, KB_F, KB_G, KB_H, KB_J,	KB_K, KB_L,	KB_SMCLN,
	KB_QUOTE, KB_BSLSH, KB_LSHFT, KB_PIPE, KB_Y, KB_X, KB_C, KB_V, KB_B, KB_N, KB_M, KB_COMMA, KB_DOT, KB_SLASH,
	KB_RSHFT, KB_UP, KB_LCTRL, KB_LGUI,	KB_LALT, KB_SPACE, KB_RALT, KB_RGUI, KB_APP, KB_RCTRL, KB_LEFT, KB_DOWN,
	KB_RIGHT, KB_NUMLK, KP_SLASH, KP_ASTRX, KP_MINUS, KP_PLUS, KB_RETURN, KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8,
	KP_9, KP_DOT
};

void print_keyfile(uint16_t *p_layout_array, uint8_t nlayers)
{
	FILE *keyfile;
	uint8_t char_buf[8] = { 0 }, scan_buf[256] = { 0 }, string_buf[256] = { 0 }, string_buf2[5192] = { 0 };

	printf("\n\nEnter filename: ");
	fgets(scan_buf, sizeof(scan_buf), stdin);
	sscanf(scan_buf, "%s", string_buf);

	keyfile = fopen(string_buf, "w");

	if (!keyfile)
		perror("Error creating/opening file");
	else
	{
		for (uint8_t i = 0; i < nlayers; i++)
		{
			for (uint8_t j = 0; j < NUMKEYS; j++)
			{
				sprintf(string_buf, "%X", *(p_layout_array + i*NUMKEYS + j));
				if (j < 1 && i < 1)
				{
					strcpy(string_buf2, string_buf);
					strcpy(char_buf, ", ");
					strcat(string_buf2, char_buf);
				}
				else
				{
					if (j < NUMKEYS - 1)
					{
						strcat(string_buf2, string_buf);
						strcpy(char_buf, ", ");
						strcat(string_buf2, char_buf);
					}
					else
						strcat(string_buf2, string_buf);

				}
			}

			if (i != nlayers - 1)
			{
				strcpy(char_buf, "\n\n");
				strcat(string_buf2, char_buf);
			}

		}

		fprintf(keyfile, "%s", string_buf2);

		fclose(keyfile);
	}
}

void print_macrosfile(uint8_t *p_layout_array)
{
	FILE *keyfile;
	char char_buf[8] = { 0 }, scan_buf[256] = { 0 }, string_buf[256] = { 0 }, string_buf2[3072] = { 0 };

	printf("\n\nEnter filename: ");
	fgets(scan_buf, sizeof(scan_buf), stdin);
	sscanf(scan_buf, "%s", string_buf);

	keyfile = fopen(string_buf, "w");

	if (!keyfile)
		perror("Error creating/opening file");
	else
	{

		for (uint8_t i = 0; i < NUM_MACROKEYS*LEN_MACRO; i++)
		{
			sprintf(string_buf, "%X", *(p_layout_array + i));

			// need to use strcpy first because string_buf2 is empty
			if (i == 0)
			{
				strcpy(string_buf2, string_buf);
				strcpy(char_buf, ", ");
				strcat(string_buf2, char_buf);
			}
			else
			{
				// append comma after all strings except the last
				if (i < NUM_MACROKEYS*LEN_MACRO - 1)
				{
					// append newline character after each macro key report (which is a multiple of 8)
					if ((i + 1) % 8 == 0)
					{
						strcat(string_buf2, string_buf);
						strcpy(char_buf, "\n");
						strcat(string_buf2, char_buf);
					}
					else
					{
						strcat(string_buf2, string_buf);
						strcpy(char_buf, ", ");
						strcat(string_buf2, char_buf);
					}
				}
				else
					strcat(string_buf2, string_buf);

			}
		}

		fprintf(keyfile, "%s", string_buf2);

		fclose(keyfile);
	}
}

uint8_t *fill_macros_array(bool convert_dec_hex)
{
	char chardump = 0;
	uint8_t n = 0;
	char  string_buf[8] = { 0 };
	static uint8_t macros_array[192];
	uint8_t *end_p = NULL;
	uint8_t token = 0;

	memset(macros_array, 0, sizeof(macros_array));

	for (uint16_t pos = 0; token < NUM_MACROKEYS*LEN_MACRO; pos++)
	{
		chardump = *((uint8_t*)string_buffers + pos);

		if (chardump == ',' || chardump == '\n' || chardump == EOF)
		{
			//printf("token %-2d\n", pos);	
			// we have another token, flush string buffer 	
			if(convert_dec_hex == true)
				macros_array[token++] = (uint8_t)strtol(string_buf, &end_p, 10);
			else
				macros_array[token++] = (uint8_t)strtol(string_buf, &end_p, 16);
			n = 0;
			continue;
		}
		else
		{
			string_buf[n] = chardump;
			string_buf[n + 1] = '\0';
			n++;
		}
	}

	return macros_array;
}

/* Converts formatted string to integer word array */
uint16_t *fill_layout_array(uint8_t nlayers, bool convert_dec_hex)
{
	char chardump = 0;
	uint8_t n = 0;
	char string_buf[8] = { 0 };
	uint8_t *end_p = NULL;
	uint16_t *p_layout_array_keyfile = NULL;

	p_layout_array_keyfile = (uint16_t *)calloc(NUMKEYS * nlayers, sizeof(uint16_t));

	for (uint8_t layer = 0; layer < nlayers; layer++)
	{

		uint8_t token = 0;

		for (uint16_t pos = 0; string_buffers[layer][pos]; pos++)
		{
			chardump = string_buffers[layer][pos];
			//printf("char: %-5c", chardump);
			if (chardump == ',' || chardump == '\n' || chardump == EOF)
			{
				//printf("token %-2d\n", pos);
				// we have another token, flush string buffer
				if(convert_dec_hex == true)
					*(p_layout_array_keyfile + layer*NUMKEYS + (token++)) = (uint16_t)strtol(string_buf, &end_p, 10);
				else
					*(p_layout_array_keyfile + layer*NUMKEYS + (token++)) = (uint16_t)strtol(string_buf, &end_p, 16);
				n = 0;
				continue;
			}
			else
			{
				//printf("notoken %-2d\n", pos);	
				string_buf[n] = chardump;
				string_buf[n + 1] = '\0';
				n++;
			}
		}
	}

	return p_layout_array_keyfile;
}



/* Opens a key file (.txt format) and copies the contents to a string buffer.
*  Space characters are skipped, only commas and a single new-line
*  character are copied to separate tokens
*/
uint8_t parse_keyfile(uint8_t *keyfile_namestring)
{
	uint8_t nlayers = 0;
	char chardump = 0;
	FILE *keyfile = NULL;

	for (uint16_t i = 0; i < sizeof(string_buffers); i++)
		*((uint8_t*)string_buffers + i) = 0;

	// open file in text mode to convert Windows '\r\n\' to '\n' (if present)
	keyfile = fopen(keyfile_namestring, "r");

	if (!keyfile)
	{
		printf("\n\n");
		perror("Error opening file");
		return 0;
	}
	else
		while (feof(keyfile) == false)
		{
			uint16_t pos = 0;
			do {
				chardump = fgetc(keyfile);
				// skip space character, keep '\n' as layer separator
				if (chardump != ' ')
					string_buffers[nlayers][pos++] = chardump;

			} while (chardump != '\n' && chardump != EOF);

			// eat second new-line character
			fgetc(keyfile);

			nlayers++;

			// sanity check
			if (nlayers > 8)
			{
				fclose(keyfile);
				printf("Layout file layer count: %u\n\r", nlayers);
				return 0;
			}
		}

	fclose(keyfile);

	return nlayers;
}

bool parse_macrosfile(uint8_t *keyfile_namestring)
{
	char chardump = 0;
	FILE *keyfile;
	uint16_t pos = 0;

	for (uint16_t i = 0; i < sizeof(string_buffers); i++)
		*((uint8_t*)string_buffers + i) = 0;

	// open file in text mode to convert Windows '\r\n\' to '\n' (if present)
	keyfile = fopen(keyfile_namestring, "r");

	if (!keyfile)
	{
		printf("\n\n");
		perror("Error opening file");
		return false;
	}
	else
		while (!feof(keyfile))
		{
			do {
				chardump = fgetc(keyfile);
				if (chardump != ' ')
					string_buffers[0][pos++] = chardump;
			} while (chardump != EOF);
		}

	fclose(keyfile);

	return true;
}

void configure_layout(uint8_t nlayers, uint16_t *p_layout_array_keyfile)
{
	uint8_t last_keypress = 0xff, nlayers_ready = 0, ch = 0, cur_layer_ready = 0, cur_layer = 0,
		row = 0, col = 0, manual_input = 0;
	uint8_t *p_end_strtol = NULL;
	uint16_t *p_layout_array = NULL;
	uint16_t rowcol = 0, rowcol_last = 0, keycode = 0;
	uint8_t str_buf[8] = { 0 }, scan_buf[8] = { 0 };

	if (nlayers <= 6 && nlayers >= 1)
	{
		p_layout_array = p_layout_array_keyfile;
		goto SET_LAYER;
	}

	printf("\n\n");
	printf("Please enter number of layers (1 min, 6 max): ");
	while (1)
	{
		if (nlayers_ready)
		{
			nlayers_ready = 0;
			break;
		}
		*str_buf = _getch();
		nlayers = (uint8_t)strtol(str_buf, &p_end_strtol, 10);
		printf("\n\n");
		printf("Number of layers: %d", nlayers);
		if (nlayers > 6 || nlayers < 1)
		{
			printf("\n\n");
			printf("Too few/many layers, try again (or press Esc to quit): ");

			while (1)
			{
				ch = _getch();
				if (ch == ASCII_ESC)
					return;
				*str_buf = ch;
				nlayers = (uint8_t)strtol(str_buf, &p_end_strtol, 10);
				printf("\rToo few/many layers, try again (or press Esc to quit): %d", nlayers);
				if (nlayers <= 6 && nlayers >= 1)
				{
					nlayers_ready = 1;
					printf("\n\n");
					printf("Number of layers: %d", nlayers);
					break;
				}
			}
		}
		else
			break;
	}

	// create layout array dynamically with calloc()

	p_layout_array = (uint16_t*)calloc(NUMKEYS * nlayers, sizeof(uint16_t));



SET_LAYER:

	printf("\n\n");
	printf("Set current layer: ");
	while (1)
	{
		if (cur_layer_ready)
		{
			cur_layer_ready = 0;
			break;
		}
		*str_buf = _getch();
		cur_layer = (uint8_t)strtol(str_buf, &p_end_strtol, 10);
		printf("\n\n");
		printf("Current layer: %d", cur_layer);
		if (cur_layer > nlayers || cur_layer < 1)
		{
			printf("\n\n");
			printf("Not a valid layer, try again (or press Esc to quit): ");

			while (1)
			{
				ch = _getch();
				if (ch == ASCII_ESC)
					return;
				*str_buf = ch;
				cur_layer = (uint8_t)strtol(str_buf, &p_end_strtol, 10);
				printf("\rNot a valid layer, try again (or press Esc to quit): %d", cur_layer);
				if (cur_layer > 0 && cur_layer <= nlayers)
				{
					cur_layer_ready = 1;
					printf("\n\n");
					printf("Current layer: %d", cur_layer);
					break;
				}
			}
		}
		else
			break;
	}

	printf("\n\n");
	printf("Please press a key to be mapped.");
	printf("\n");
	printf(
		"ESC - save layer to file and exit"
		" | "
		"ENTER - continue"
		" | "
		"BACKSPACE - change current layer"
		" | "
		"\n"
		"TAB - switch auto/manual keycode input"
		);
	printf("\n\n");

	Sleep(1000);		// give the user some time to release Enter key, or else GetAsyncKeyState() will
						// immediately return last key pressed



GET_KEY:

	if (manual_input)
	{
		while (1)
		{
			rowcol = read_matrix_pos();

			if (rowcol != 0xffff && rowcol != rowcol_last)
			{
				// don't worry about the magic numbers, read_matrix_pos() just returns a short, lower byte contains number
				// of rows and higher byte contains number of columns
				row = rowcol & 0x0f;	// decipher with bitmask and assign value
				col = (rowcol & 0xff00) / 256;
				printf("\r%32c", '\0');
				printf("\rRow: %d Column: %d Keycode: ", row, col);
			}

			if (_kbhit())
			{
				fgets(str_buf, sizeof(str_buf), stdin);
				sscanf(str_buf, "%s", scan_buf);
				keycode = (uint16_t)strtol(scan_buf, &p_end_strtol, 10);
				p_layout_array[(row * NUMCOLS + col) + (cur_layer - 1) * NUMKEYS] = keycode;

				while (1)
				{
					ch = _getch();
					switch (ch)
					{
					case ASCII_ESC:
						print_keyfile(p_layout_array, nlayers);
						free(p_layout_array);
						return;
					case ASCII_ENTER:
						rowcol_last = rowcol;
						printf("\nRow: %d Column: %d Keycode: ", row, col);
						goto GET_KEY;
					case ASCII_BKSPC:
						rowcol_last = rowcol;
						goto SET_LAYER;
					case ASCII_TAB:
						rowcol_last = rowcol;
						last_keypress = 0xff;
						manual_input ^= 1;
						printf("\n\n");
						printf("+++ Manual key input disabled +++");
						printf("\n\n");
						keybd_event(VK_TAB, 0, 2, 0);
						GetAsyncKeyState(VK_TAB);
						goto GET_KEY;
					}
				}
			}
		}
	}

	while (1)
	{
		rowcol = read_matrix_pos();

		if (rowcol != 0xffff && rowcol != rowcol_last)
		{
			// don't worry about the magic numbers, read_matrix_pos() just returns a short, lower byte contains number
			// of rows and higher byte contains number of columns
			row = rowcol & 0x0f;	// decipher with bitmask and assign value
			col = (rowcol & 0xff00) / 256;
			printf("\rRow: %d Column: %d Keycode: ", row, col);
		}

		for (uint8_t n = 0; n < sizeof(vk_code_array); n++)
			if (GetAsyncKeyState(*(vk_code_array + n)) & 0x8000 && (last_keypress != n))
			{
				last_keypress = n;
				p_layout_array[(row * NUMCOLS + col) + (cur_layer - 1) * NUMKEYS] = *(hid_code_array + n);
				printf("\r%32c", '\0');
				printf("\rRow: %d Column: %d Keycode: ", row, col);
				printf("%s", *(vk_name_array + n));
				// make sure Esc/Enter/Backspace can't fall through the while-loop
				if (n == 0 || n == 45 || n == 28 || n == 32)
					_getch();

				while (1)
				{
					ch = _getch();
					switch (ch)
					{
					case ASCII_ESC:
						print_keyfile(p_layout_array, nlayers);
						free(p_layout_array);
						return;
					case ASCII_ENTER:
						keybd_event(VK_RETURN, 0, 2, 0);
						GetAsyncKeyState(VK_RETURN);
						rowcol_last = rowcol;
						printf("\nRow: %d Column: %d Keycode: ", row, col);
						goto GET_KEY;
					case ASCII_BKSPC:
						rowcol_last = rowcol;
						goto SET_LAYER;
					case ASCII_TAB:
						rowcol_last = rowcol;
						manual_input ^= 1;
						printf("\n\n");
						printf("+++ Manual key input enabled +++");
						printf("\n\n");
						goto GET_KEY;
					}

				}

			}
		Sleep(50);

		// we need to implement a cheap hack to read the Pause key, because it will freeze console output
		// when depressed until another key is depressed. We will call keybd_event() to send a non-defined 
		// key to the console to activate output again

		if (GetAsyncKeyState(VK_PAUSE) & 0x8000 && (last_keypress != 0xfe))
		{
			keybd_event(VK_OEM_8, 0, 0, 0);
			keybd_event(VK_OEM_8, 0, 2, 0);
			last_keypress = 0xfe;
			p_layout_array[(row * NUMCOLS + col) + (cur_layer - 1) * NUMKEYS] = KB_PAUSE;
			printf("\r%32c", '\0');
			printf("\rRow: %d Column: %d Keycode: ", row, col);
			printf("%s", "Pause");

			while (1)
			{
				ch = _getch();
				switch (ch)
				{
				case ASCII_ESC:
					print_keyfile(p_layout_array, nlayers);
					free(p_layout_array);
					return;
				case ASCII_ENTER:
					keybd_event(VK_RETURN, 0, 2, 0);
					GetAsyncKeyState(VK_RETURN);
					rowcol_last = rowcol;
					printf("\nRow: %d Column: %d Keycode: ", row, col);
					goto GET_KEY;
				case ASCII_BKSPC:
					rowcol_last = rowcol;
					goto SET_LAYER;
				case ASCII_TAB:
					rowcol_last = rowcol;
					manual_input ^= 1;
					printf("\n\n");
					printf("+++ Manual key input enabled +++");
					printf("\n\n");
					goto GET_KEY;
				}
			}
		}
		rowcol_last = rowcol;

	}
}