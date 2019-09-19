#Blusb Conversion Guide

I'll run you through the conversion steps below 'just in case', but if you know what you're doing you don't have to follow my advice. This guide also contains operating instructions, btw. Some points may not relate to your particular setup but I'll leave them in anyway, just to be complete.

Unpacking

After unpacking the controller, remove the foam padding and adhesive tape. Make sure the wire that connects the metal plate and shield GND does not touch the PCB anywhere.

Checking for proper functioning

Before mounting or glueing anything anywhere, connect the LED PCB and check if the LEDs will light up upon throwing the mode select switch. Then check if the controller is correctly recognized as an HID device ('Model M') when plugging in a USB cable. Then check if you can successfully pair the Bluetooth module with your host device. It is crucial that your host device supports BT 4.0/BLE or above. If you're using an el cheapo BT dongle that plugs into a USB port, there is a chance you will run into problems even though the module has a 'Bluetooth 4.0' sticker on it. I currently would recommend a BT adapter based on Broadcom's BCM20702 as it seems to provide decent functionality even in difficult environments. Any adapter based on CSR's 8510A10 'BT 4.0' chip will most likely not recognize the module, at least not on a system running Windows 7 - unless it's running a BLE compatible BT stack.

HID proxy dongle

The included HID Proxy BT dongle comes pre-paired and will (should) connect to the module automatically. Use the corresponding CLI (btusbcmd.exe) to clear pairings if necessary. The CLI also allows to switch the mode of operation from HID proxy to HCI mode. In HCI mode, the dongle will operate as a regular BT dongle that requires the OS system to supply a BT stack and handle pairing and bonding.

Note: in HID proxy mode, the dongle does not update the LEDs.

Mounting

The replacement controller should fit inside the bottom shell just as the original IBM controller (however, there are slight differences between Model M revisions as far as the holders and seatings are concerned, which can influence fit). Once in place, the PCB should have a snug fit. When seating the PCB, a fair amount of downward force should make the mounting pins slide through the pre-drilled holes. If the the mounting pins don't fit inside the pre-drilled holes, don't try to squeeze them in with excessive force. Grab a suitably sized round file and carefully extend the width of the holes. You would want to file off a bit all around the perimeter of the holes. Do a bit, then pause and check if the controller will fit. Don't overdo it or you will make the holes to big and end up with a loose fit.
If the pre-drilled holes turn out to be too big, use double-sided adhesive to hold the controller firmly in place. Put a 10x10mm strip right below the USB socket and a 5x5mm strip on the opposite side of the PCB, sort of in a straight line with the USB socket. Don't use big strips of tape or you will have a really hard time removing the controller, trust me.

I've pre-attached double sided adhesive tape for you on the bottom of the LED PCB. Before firmly attaching it, it has to be correctly positioned. I suggest removing the protective foil and putting it back on, leaving a small strip uncovered at the edge. Put the LED PCB inside of the white marking where the original LED PCB was attached, Connect the ribbon cable, and put the cover on. Flip the mode select switch on the controller and check if the LEDs are correctly aligned with the openings in the top cover. If necessary, reposition the LED PCB and check again. Only remove the rest of the foil once you have found the right spot, then attach the LED PCB firmly onto the surface.

Note: if your device features a yellow-wire connector cable to connect the LED daughterboard, make sure to swap pin 1 and 2 inside the JST housing ON THE SIDE OF LED PCB (direction of counting: left to right, direction of connector: holes pointing to floor). Make sure that all diodes connect to the Vcc pin on the controller board. Otherwise the LEDs will not light up. Let me know if you need help figuring this out.

Changing the LED power source

If you have not bought a replacement LED daughterboard and would like to increase the maximum brightness level of the original status LEDs in BT mode, you can bypass the onboard LDO (output: 2.2V) and power the LEDs directly from the battery (output: 3.7V). This will also enable you to use LEDs that require a higher forward-bias voltage (s.a. blue LEDs).

Connecting the battery and NTC

The red wire connects to +/3.7V, black connects to -/GND. The battery terminal pins on the PCB are marked accordingly. It is also protected against reverse polarity by a high-side P-Channel MOSFET, just in case.

If there's a jumper cable included in your kit with a striped bead attached to one end (that's a temperature sensor, an NTC, i.e. a Negative Temperature Coefficient thermistor), connect it to the terminal marked 'NTC 10K', otherwise the charger IC will not work. Place the NTC as close as possible to the battery, preferably on top, and attach it with a strip of adhesive tape. The overtemperature protection will kick in at approximately 55°C to interrupt the recharge process.

Reassembly

After seating the controller PCB, put the keyboard assembly back on and hold the top up with one hand to that your other hand can slip through the open space between assembly and bottom shell, and connect the flexible foil cables. Before putting the top shell back on, flip the switch and check if all LEDs are lighting up properly ((1x USB, 2x BT). Then plug in a USB cable or start a Bluetooth connection and check if all cables have a good connection. I suggest using Aquas's key tester for that (download from link). Sometimes minor adjustments have to be made to the FFC sockets if the spring contacts are not tight enough, but usually they will work just fine.
If everything is ok, put the top shell back on and fasten the screws at the bottom.


Operating instructions

Putting the device into operation is not a big deal really. The mode of operation is selected with the big slide switch next to the USB socket. The status LEDs will light up 1x in USB mode and 2x in BT mode. There are also two additional status LEDs on top of the USB socket, a red and an amber one. The amber LED indicates the connection status of the BT module. On enabling the BT module, the amber LED will start blinking, slowly indicating that it's discoverable and advertising and ready to accept pairing requests. The BT module will stay in discovery mode for 1 minute, then the amber LED will extinguish and the module will go to sleep. During sleep, the whole circuit will only consume about 36 µA. So whenever the BT host drops the connection (after shutting down for instance), the module will not advertise infinitely. That way the battery can't get drained out and no extra precautions have to be taken to save battery life. Any key press will wake the device up again.
Once the module has been successfully paired with a host device, the LED will stop flashing and stay off until the connection is interrupted, in which case the module will start advertising again.
The red LED indicates the status of the battery charger circuit. On attaching a USB cable, the red LED will light up and the battery will automatically start recharging. Once the battery has been fully recharged, the red LED will extinguish. After that, the charging circuit will wait for a voltage drop below a certain threshold before recharging the battery again.

A minor tweak to maximize battery life: the device can be 'shut down' during longer intervals of inactivity (typically at night or for holidays or whatever): when no USB cable is plugged in and USB mode is selected, the controller will go into deep sleep mode after 1 minute, turning off all clocks and thereby reducing current consumption to 5.6 µA. To wake the device up again when in deep sleep mode, a hard reset will be necessary (by flipping the switch or plugging in a USB cable).

Special functions

For convenience, the brightness level of the status LEDs can be adjusted by assigning separate keys for increasing/decreasing brightness, but also with the CLI. I recommend low values in the range from 0 - 20, everything else will most likely be too bright.

It is also possible to configure macro keys at run time. In order to do that, define a macro key and a record-macro key (additionally a macro-delete key may be defined). On pressing the record-macro key, the green status LEDs will start blinking slowly, indicating that the device is now in record-mode. Use the following steps to record a macro:
1. press the macro key to be configured.
2. Enter a combination of up to 6 regular and up to 8 modifier keys. Note: it is not required to press and hold all the keys you want to add at once - one after the other will work just fine. To delete the keys you entered, just press the macro-delete key and start over.
3. If you're done, press the macro-record key again to confirm and save the new macro configuration. The slow blink loop of the status LEDs will be cut off by a short flicker to indicate that the device is no longer in record-mode.

Software tools

At the link location (https://www.dropbox.com/s/un4t22qy83dupdh/blusb_tools_2.0.rar?dl=0) you will also find a simple CLI (blusb_cmd_2.0.exe) that allows changing the key map at will (it is pretty rudimentary though, let me know if you run into problems). It requires installing a filter driver (use Zadig and install WinUSB for the device). After installing the filter driver, the device will no longer work as an HID device. You either have to uninstall the driver when you're done changing the key map or try a different USB port that is not associated with the filter driver (which works on Windows, other OSes might handle that differently).

The CLI allows configuration of the key map and macro key table. To change either, it will suffice to edit the example files and then upload them with the CLI.
Note: By default the macro table is empty. Hence reading the (empty) macro table with the CLI will only return an error message ('Bad eeprom value').
If you start the executable from a console window without any parameters, the correct syntax and a list of options will be displayed.

Changing the key map with the CLI / Editing the *.bin layout files

The .bin files contain preconfigured key maps. The file type is plain text, the file ending may be chosen arbitrarily.

Editing the .bin files works thus:

Unless you need to reconfigure all keys, it will suffice to edit the default key map files. The key map files are regular text files. On Windows, I suggest to use Wordpad as opposed to Notepad for editing, because Notepad won't safe certain special characters the CLI depends on to parse the layout file.

For every layer, there are 160 values that comprise the key matrix. The key matrix consists of a maximum of 20 columns and 8 rows, i.e. for each row there are 20 columns, which gives 8*20 positions in total. The first 20 positions belong to row 1, the second 20 positions belong to row 2, the next 20 belong to row 3, and so forth. If you need to change a couple of values, look up the given values in the layout header file (layout.h). This file contains all the HID code values available for mapping according to the HUT (HID Usage Table). All you need do is change a value, save the file and transfer the contents of the file to the controller using the CLI.

Note: there are special key codes that are 16 bit in length, e.g. the modifier keys. The lower byte represents the key type (to determine the position of the key value within the HID key report to be sent by the microcontroller to the USB host).
For modifiers, the higher byte is always 0x01 (hexadecimal). The Alt_left key, for instance, is represented by the value 0x04, hence the complete key code generated internally by the microcontroller will be 0x0104.
This scheme is not intuitive and may seem confusing, mainly due to the following two aspects: the higher byte for character keys is always 0x00, i.e. zero, while it is non-zero for other key types, for one. Second, the values contained in the layout header file (layout.h) are given in hexadecimal format while in the key map file (.bin) they are given in decimal format. Accordingly, in the key map file the key code for Alt_left, for example, is not given as 104 (hexadecimal, 0x104), but as 260 - even though the actual HUT value for Alt_left is 4. Why 260? Because hexadecimal 104 translates to decimal 260. Because Alt is a modifier key, it's representation in the key map file is not identical with the value given in the HUT table (which the layout header file, layout.h is based on). Conversely, all character key values are identical with the (decimal) values given in the HUT table, because the higher byte which identifies the key type is always 0. No character key value is bigger than 255.
Macro keys are defined according to the same scheme as modifier keys, i.e. they are 16 bit in length (cp. layout.h).

Alternatively, there is also the possibility of loading an existing key map file into memory and using the CLI to change the given key map. For this method, a second keyboard is necessary, however.
Any changes made with the CLI are not rendered effective at runtime. Configuring a key map and transfering of a key map to the controller comprise two consecutive steps. This precludes the event of ending up with a garbled key map if things go awry.

Starting the CLI by typing "blusb_cmd -configure_layout [keymap_filename.bin]" will load the specified key map file into memory. After the desired changes have been made, the new configuration can be saved to a new file or the existing file may be overwritten.

The CLI also allows tweaking the debounce period. The default value is 7ms. You can probably get away with less, say, 3 ms. Conversely, if you should encounter bouncing at 7ms, you can also crank it up. For instance, Unicomp recommends 15 ms.

There's now a (soon to be cross-platform) GUI thanks to the efforts of fellow Deskthority member arakula. Check it out: https://github.com/Arakula/blusb_gui
If you're experiencing any issues, please report here: https://deskthority.net/viewtopic.php?f=7&t=21469
