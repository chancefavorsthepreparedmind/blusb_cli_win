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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "layout.h"
#include "libusb.h"
#include <conio.h>
#include <stdbool.h>

#include "hex.h"
#include "hid.h"


void print_usage(argv);


extern libusb_device_handle *handle;
extern uint8_t verbose;

/************************************************************************/
/*                         Type definitions                             */
/************************************************************************/

typedef unsigned char			uchar;
typedef unsigned int			uint;