# SimpleArduClock
Simple Arduino 256 Clock with an 5" LCD display

## Requirements
Last time installed I've used an Arduino studio 1.8.5 (and I needed to adjust some stuff in the library to make it compile)

## Compilation
Simply copy the UTFT library into the Libraries folder and build

## Installation / Adjusting the time
Adjust the starting time on  line just before setup()

Clock c(hh, mm, ss);
