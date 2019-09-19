# Blusb Software Operating Instructions

#### Dependencies

Libusb is necessary to compile the source. The included static libray is based on Libusb v1.0.23.11397.
Reference: https://libusb.info/

#### Usage

This is the default usage statement displayed when no valid argument has been provided. The commands are mostly self-explanatory. I have added explanatory remarks for clarification.

`+++ Blusb configuration tool +++

Usage: blusb_cmd [-option] [-optional parameter] [filename]

Options:

-read_matrix  
-read_pwm  
-write_pwm [value_USB value_BT] (Valid range: 0-255)  
-read_debounce  
-write_debounce [value] (Valid range: 1-255)  
-read_macros  
-write_macros [filename]  
-read_layout [-no_print] [-names]  
-write_layout [filename]  
-configure_layout [-update filename]  
-read_version  
-update_firmware [filename]  
-enter_bootloader  
-exit_bootloader  
-layout_dec_to_hex [filename]  
-macros_dec_to_hex [filename]  
-h, --help, /?`

#### Explanation

`-read_matrix`  
Display row and column of a key pressed.


`-read_pwm`


#### Layout and macro configuration

The CLI allows configuration of the key map and macro key table. To change either, it will suffice to edit the example files and then upload them with the CLI.
*Note: By default the macro table is empty. Hence reading the (empty) macro table with the CLI will only return an error message.*

The layout files contain preconfigured key maps. The file type is plain text, the file ending may be chosen arbitrarily.

Editing the layout files works thus:

Unless you need to reconfigure all keys, it will suffice to edit the default key map files. You can use any text editor you like.

For every layer, there are 160 values that comprise the key matrix. The key matrix consists of a maximum of 20 columns and 8 rows, i.e. for each row there are 20 columns, which gives 8x20 positions in total.
We will start counting from 0, so the first row is row 0, the last row is row 7, and likewise the first column is column 0, the last column is column 19.
The first 20 positions belong to row 0, the second 20 positions belong to row 1, the next 20 belong to row 2, and so forth. If you need to change a couple of values, look up the given values in the layout header file (layout.h). This file contains the values representing all the keys that can be mapped. All you need do is change a value, save the file and transfer the contents of the file to the controller using the CLI. Like so:

> blusb_cmd -write_layout yourlayout.dat

#### Understanding the internal representation of special key codes (s.a. modifiers)

There are special key codes that are 16 bit in length, e.g. the modifier keys. The lower byte represents the key type (to determine the position of the key value within the HID key report to be sent by the microcontroller to the USB host).
For modifiers, the higher byte is always 0x01 (hexadecimal). The Alt_left key, for instance, is represented by the value 0x04, hence the complete key code generated internally by the microcontroller will be 0x0104.
This scheme is not intuitive and may seem confusing, mainly due to the following two aspects: the higher byte for character keys is always 0x00, i.e. zero, while it is non-zero for other key types, for one. Second, the representation of modifier keys in the key map file is not identical with the value given in the HUT table (which the layout header file, layout.h is based on). 
Macro keys are defined according to the same scheme as modifier keys, i.e. they are 16 bit in length (cp. layout.h).

Alternatively, there is also the possibility of loading an existing key map file into memory and using the CLI to change the given key map. For this method, a second keyboard is necessary or you will not be able to enter keys that have not been mapped yet.
Any changes made with the CLI are not rendered effective at runtime. Configuring a key map and transfering of a key map to the controller comprise two consecutive steps. This precludes the event of ending up with a garbled key map if things go awry.

Starting the CLI by typing "blusb_cmd -configure_layout [keymap_filename.bin]" will load the specified key map file into memory. After the desired changes have been made, the new configuration can be saved to a new file or the existing file may be overwritten.

The CLI also allows tweaking the debounce period. The default value is 7ms. You can probably get away with less, say, 3ms. Conversely, if you should encounter bouncing at 7ms, you can also crank it up. For instance, Unicomp recommends 15ms.
*Note: the ms value given does not represent the actual or total delay that occurs from when a key is pressed until a key press is registered. The actual delay depends on other factors besides the debounce period.
