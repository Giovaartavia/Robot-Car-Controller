# Robot Car Controller

Code for iOS bluetooth controller app for a rover that we built ourselves. This code was written to work specifically with the Arduino Uno and the Adafruit Bluefruit LE UART Friend bluetooth module (with our module's specific UUID).

## How to Run

Change the ViewController.swift ids struct to match your device's specific UUID's (if not known you can uncomment some of the commented print statements to look for devices). Download the .ino code into the Arduino and the XCode code into a physical iPhone. To check if device was properly connected, open the Arduino serial monitor and check for confirmation messages.

## Authors

* **Giovanni Artavia** - [Giovaartavia](https://github.com/Giovaartavia)
* **Brandon Lammey** - [brandonlammey](https://github.com/brandonlammey)
* **Anna Seib** - [a268s950](https://github.com/a268s950)