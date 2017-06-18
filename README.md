# Alarm Clock

## The aim of the project is conceive an Alarm clock respondig to the following specifications
1. Automatically get the time and keep it
1. stay on on battery for at least 1h
1. design / without any apparent controls
1. custom (if simple) ringtone
1. adaptative brightness

## Ideas of implementation:
1. 2 solutions, DCF77 to get the time via radio, or wifi

...I wanted to test the esp8266 so wifi will it be constant polling (a few hours) without RTC module should be enough, if the precision keeps into ~10s
1. Here the idea is to set a 18650 battery module  (from 2.4to 4V) and a 3.3V battery regulator. The rest of the time (and to feed the battery), a 5V USB charger will be used
1. I see it as a wood block of 10*10*10cm without apparent display or controls.

...The display, when lit, would appear through the wood (infact a thin layer)..
...The alarm will be activating by flipping the cube head down (and all the display will be inverted)..
...basic controls would be provided by vibration (patting it) or tilting it left or right
1. This have to be a DAC (not decided if it needs RAM or I2C/SPI yet), and the same goes for the amplifier
1. Input isn't complicated, but i'm afraid not beeing able to dim the LEDs or the screen enought.