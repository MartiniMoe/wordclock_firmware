![Wordclock Image](/pic/wordclock.jpg)

A wild Wordclock.

# Wordclock Firmware

This is the firmware for a NodeMCU (ESP8266) based Wordclock.
It features time-synchronisation via NTP and a webinterface for configuration.

## Features
* Time synchronisation via NTP
* Hotspot + captive portal for first-time-configuration
* Web interface for customizations
* Custom LED colors
* Custom brightness
* (Auto brightness option, if photoresistor is connected) WIP UNTESTED; DUNNO...
* The progress of the current shown time is displayed by fading the letters (one by one) to a second color

Recommended use is with PlatformIO.

```
pio pkg install
pio run --target=upload
```

## Branches
Currently there are two branches. One for Wordclocks with APA102 LEDs and one for those with (cheaper) WS2812b LEDs. Current development happens on the WS2812 branch. Its somewhat stupid, I am planning on merging those and handling different LEDs via DEFINES...

## Credits

Thanks to all members of the Hackspace (https://ha.si) and the Fab Lab (http://fablab-siegen.de) for their contributions :)

![Wordclock Interface](/pic/interface.jpg)

This is the web interface.
