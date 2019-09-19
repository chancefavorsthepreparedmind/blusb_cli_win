### Conversion Guide

If you know what you are doing you do not have to follow this guide. Since the controller board is compatible with a variety of M series keyboards, some points may not relate to your particular setup and can be ignored.

#### Unpacking

After unpacking the controller, remove the foam padding and adhesive tape. Make sure the wire that connects the metal plate and shield GND does not touch the PCB anywhere.

#### Checking for proper functioning

Before mounting or glueing anything anywhere, connect the LED PCB and check if the LEDs will light up upon throwing the mode select switch. Then check if the controller is correctly recognized as an HID device ('Model M') when plugging in a USB cable. Then check if you can successfully pair the Bluetooth module with your host device. *It is crucial that your host device supports BT 4.0/BLE or above.* If you are using a noname BT dongle as a BT adapter, there is a chance you will run into problems even though the module has a 'Bluetooth 4.0' sticker on it. I can recommend a BT adapter based on Broadcom's BCM20702 chipset as it seems to provide decent functionality even in difficult environments. Many adapters based on CSR's 8510A10 'BT 4.0' chipset may not recognize the module on older OSes without native Bt 4.0/BLE support.

#### HID proxy dongle

The included HID Proxy BT dongle will connect to the onboard BT module automatically. Use the corresponding CLI (btusbcmd.exe) to clear pairings if necessary. The CLI also allows to switch the mode of operation from HID proxy to HCI mode. In HCI mode, the dongle will operate as a regular BT dongle that requires the OS system to supply a BT stack and handle pairing and bonding.

*Note: in HID proxy mode, the dongle does not update the LEDs.*

#### Mounting

The replacement controller should fit inside the bottom shell just as the original IBM controller (however, there are slight differences between Model M revisions as far as the holders and seatings are concerned, which can influence fit). Once in place, the PCB should have a snug fit. When seating the PCB, a fair amount of downward force should make the mounting pins slide through the pre-drilled holes. If the mounting pins don't fit inside the pre-drilled holes, don't try to squeeze them in with excessive force. Grab a suitably sized round file and carefully extend the width of the holes. You would want to file off a bit all around the perimeter of the holes. Do a bit, then pause and check if the controller will fit. Do not overdo it or you will make the holes to big and end up with a loose fit.
If the pre-drilled holes turn out to be too big, use double-sided adhesive to hold the controller firmly in place. Put a 10x10mm strip right below the USB socket and a 5x5mm strip on the opposite side of the PCB, sort of in a straight line with the USB socket. Do not use big strips of tape or you will have a really hard time removing the controller, trust me.

I have pre-attached double sided adhesive tape for you on the bottom of the LED PCB. Before firmly attaching it, it has to be correctly positioned. I suggest to remove the protective foil and to put it back on leaving a small strip uncovered at the edge. Put the LED PCB inside of the white marking where the original LED PCB was attached, connect the ribbon cable, and put the cover on. Flip the mode select switch and check if the LEDs are correctly aligned with the openings in the top cover. If necessary, reposition the LED PCB and check again. Only remove the rest of the foil once you have found the right spot, then attach the LED PCB firmly onto the surface.

*Note: if your device features a yellow-wire connector cable to connect the LED daughterboard, make sure to swap pin 1 and 2 inside the JST housing ON THE SIDE OF LED PCB (direction of counting: left to right, direction of connector: holes pointing to floor). Make sure that all diodes connect to the Vcc pin on the controller board. Otherwise the LEDs will not light up. Let me know if you need help figuring this out.*

#### Changing the LED power source

If you have not bought a replacement LED daughterboard and would like to increase the maximum brightness level of the original status LEDs in BT mode, you can bypass the onboard LDO (output: 2.2V) and power the LEDs directly from the battery (output: 3.7V). This will also enable you to use LEDs that require a higher forward-bias voltage (s.a. blue LEDs).

#### Connecting the battery and NTC

The red wire connects to +/3.7V, black connects to -/GND. The battery terminal pins on the PCB are marked accordingly. It is also protected against reverse polarity by a high-side P-Channel MOSFET, just in case.

If there is a jumper cable included in your kit with a striped bead attached to one end (that is a temperature sensor, an NTC, i.e. a Negative Temperature Coefficient thermistor), connect it to the terminal marked 'NTC 10K', otherwise the charger IC will not work. Place the NTC as close as possible to the battery, preferably on top, and attach it with a strip of adhesive tape. The overtemperature protection will kick in at approximately 55°C to interrupt the recharge process.

#### Reassembly

After seating the controller PCB, put the keyboard assembly back on and hold the top up with one hand to that your other hand can slip through the open space between assembly and bottom panel, and connect the flexible foil cables. Before putting the top cover back on, flip the switch and check if all LEDs are lighting up properly ((1x USB, 2x BT). Then plug in a USB cable or start a Bluetooth connection and check if all cables have a good connection. I suggest using Aquas's key tester or Elitekeyboards' Switch Hitter for that. Sometimes minor adjustments have to be made to the FFC sockets if the spring contacts are not tight enough, but usually they will work just fine.
If everything is ok, put the top cover back on and fasten the screws at the bottom.


### Operating instructions

Putting the device into operation is not a big deal really. The mode of operation is selected with the big slide switch next to the USB socket. The status LEDs will light up 1x in USB mode and 2x in BT mode. There are also two additional status LEDs on top of the USB socket, a red and an amber one. The amber LED indicates the connection status of the BT module. On enabling the BT module, the amber LED will start blinking, slowly indicating that it is discoverable and advertising and ready to accept pairing requests. The BT module will stay in discovery mode for 1 minute, then the amber LED will extinguish and the module will go to sleep. During sleep, the whole circuit will only consume about 336 µA. So whenever the BT host drops the connection (e.g. after shutting down), the module will not advertize infinitely. That way the battery cannot get drained out and no extra precautions have to be taken to save battery life. Any key press will wake the device up again.
Once the module has been successfully paired with a host device, the LED will stop flashing and stay off until the connection is interrupted, in which case the module will start advertising again.
The red LED indicates the status of the battery charger circuit. On attaching a USB cable, the red LED will light up and the battery will automatically start charging. Once the battery has been fully recharged, the red LED will extinguish. After that, the charger circuit will wait for a voltage drop below a certain threshold before recharging the battery again.

A minor tweak to maximize battery life: the device can be 'shut down' during longer intervals of inactivity (typically at night or for holidays or whatever): when no USB cable is plugged in and USB mode is selected, the controller will go into deep sleep mode after 1 minute, turning off all clocks and thereby reducing current consumption to 305.6 µA. To wake the device up again when in deep sleep mode, a hard reset will be necessary (by flipping the switch or plugging in a USB cable).

#### Special functions

For convenience, the brightness level of the status LEDs can be adjusted by assigning separate keys for increasing/decreasing brightness, but also with the CLI. I recommend low values in the range from 0 - 20, everything else will most likely be too bright.

It is possible to configure macro keys at run time. In order to do that, define a macro key and a record-macro key (additionally a macro-delete key may be defined). On pressing the record-macro key, the green status LEDs will start blinking slowly, indicating that the device is now in record-mode. Use the following steps to record a macro:
1. press the macro key to be configured.
2. Enter a combination of up to 6 regular and up to 8 modifier keys. Note: it is not required to press and hold all the keys you want to add at once - one after the other will work just fine. To delete the keys you entered, just press the macro-delete key and start over.
3. If you're done, press the macro-record key again to confirm and save the new macro configuration. The slow blink loop of the status LEDs will be cut off by a short flicker to indicate that the device is no longer in record-mode.
