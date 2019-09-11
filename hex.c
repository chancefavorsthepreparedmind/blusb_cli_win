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

int  parseUntilColon(FILE *fp)
{
	int c;

	do {
		c = getc(fp);
	} while (c != ':' && c != EOF);
	return c;
}

int  parseHex(FILE *fp, int numDigits)
{
	int     i;
	char    temp[9];

	for (i = 0; i < numDigits; i++)
		temp[i] = getc(fp);
	temp[i] = 0;
	return strtol(temp, NULL, 16);
}

int  parseIntelHex(char *hexfile, char *buffer, int *startAddr, int *endAddr)
{
	int     address, base, d, segment, i, lineLen, sum;
	FILE    *input;

	input = fopen(hexfile, "r");
	if (input == NULL) {
		fprintf(stderr, "error opening %s: %s\n", hexfile, strerror(errno));
		return 1;
	}
	while (parseUntilColon(input) == ':') {
		sum = 0;
		sum += lineLen = parseHex(input, 2);
		base = address = parseHex(input, 4);
		sum += address >> 8;
		sum += address;
		sum += segment = parseHex(input, 2);  /* segment value? */
		if (segment != 0)    /* ignore lines where this byte is not 0 */
			continue;
		for (i = 0; i < lineLen; i++) {
			d = parseHex(input, 2);
			buffer[address++] = d;
			sum += d;
		}
		sum += parseHex(input, 2);
		if ((sum & 0xff) != 0) {
			fprintf(stderr, "Warning: Checksum error between address 0x%x and 0x%x\n", base, address);
		}
		if (*startAddr > base)
			*startAddr = base;
		if (*endAddr < address)
			*endAddr = address;
	}
	fclose(input);
	return 0;
}