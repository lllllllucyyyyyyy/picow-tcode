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