# 4/3/25
This project has been underway for a week or so now, but I decided to keep a log to track progress and such for those who want to work on it. As it stands now, the code is able to show up in the intiface application as a lovense device, but given that there is little to no actual implementation of nay neccesary apis that lovense uses, it threw warnings, as expected. The hard part seems to be over with, onto adding actual functionality.

## later in the day
Turns out the I was editing the wrong file, I was editing the user file instead of the device file. oh well. Device config file, device name, and a couple other things are getting updated.

## even later
I was wondering why I couldn't get the program to detect writes... turns out it's the dynamic gatt trait? (idk what they're called, but its in the same spot as where youd put write and notify and so on) that lets the program actually use that data. That was a fun half hour of debugging. 
//TODO (done): get a utf-8 string out of whatever garbled nonsense the bluetooth writes send, and spew it into the console. baby steps :)

# 4/4/25
Turns out literally printing the incoming buffer let me see the strings coming in. Copying that should let me use the contents.
//TODO: separate bluetooth commands into their own file, start decoding incoming t code.

## later
The t-code decoder? is coming along well. I felt like spitting out a custom object with all the details would be the easiest to implement across various devices without overcomplicating things tooo much. I was considering callbacks, but an object is a bit cleaner to implement on the device code end.

# 4/6/25
Busy day yesterday, so no updates, but the functionality of both the t-code processor and the bluetooth callbacks are complete. All that is left is to implement some functionality to actually use the incoming data, for which I plan to implement a simple dual-vibration system. Hopefully I can demonstrate a way to add in serial support as well.

# 4/7/25
I added a quick pwm-based example for vibration motors, but it's untested for now, until I get around to putting together a circuit for it.
Separating the header files from the code is... tricky and annoying, and I'm debating if it's even worthwhile.

# 4/8/25
The code is finished and fully functional! I went and added serial support for shits and giggles as well, which is implemented in a way to use the same tcode processor as over bluetooth. Over the coming days, I'll work on documenting the code and possibly reformatting it a tad to make it less confusing. A servo example branch has also been added (born out of... ~personal matters~ but the source code might as well be public) which is almost the exact same as the vibration example, but with bluetooth disabled due to it being on a non-pico rp2040 board.

# 4/9/25
I decided to try and separate the commands used by the input in order to make maintaining forking a bit more simple.
Using the void pointer was giving me too many issues, so I just decided to (ab)use the way that compilers reference commands to call my processing command.