# Pico W T-Code (work in progress)
## and general serial over gatt starting point, I guess.

This project is (or at least will be) a decently-documented T-Code implementation for the pico w that works over bluetooth, with the intention to connect to intiface. The T code and Bluetooth sections will be separate, so that either one can be adapted into individual projects if desired. Serial support is also implemented and functional.

## Usage Instructions
With the state of the code now, the only way to use it is via a pwm-controlled vibration motor on pin 2 or 3. Deploy the code to a pico w, connect the motor, and power it up. To get intiface to connect to the device, open your buttplug-device-config-v3.json file. Find the tcode-v03 section, and paste the contents of the dev-config.json file in this repo in the place of the tcode block. Then boot up intiface, start a discovery scan, and the device should appear shortly.

## The Parts
The code for this firmware is split into a few main parts. The bluetooth manager, which initializes, advretises, and reads data from the bluetooth chip. The tcode processor takes in a string that contains a tcode command, like one from serial or bluetooth, and converts it into a custom type, containing all the info from a command. The vibe control manages not only your mood, but mostly the different vibration axes of whatever you're controlling. It's meant to be more of a demo for the code, but is separated to make it easier to mod in different types of toys.

## Things to note
If you decide to attempt to use this code for yourself, please keep in mind a few things. If a part of a command is not present, such as the time command for vibration controls, the values may be filled with jumbled data. Ideally, though, you wont' try to access the data in these cases anyways. When it comes to the magnitude trait, while this usually only ranges between 0 and 100 in intiface, it is returned as a float ranging from 0 to 1. This seemed the easiest to use to me, as you simply multiply the range of a value by the magnitude, and you get your new value.