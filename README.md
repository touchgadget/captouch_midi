# USB MIDI with Capacitive Touch Inputs

The captouch_midi project turns a Raspberry Pi Pico into a USB MIDI device 12
capacitive touch inputs in addition to 8 button inputs. The hardware consists
of a Raspberry Pi Pico with an MPR121 break out board. The MPR121 provides 12
capacitive touch inputs. Eight buttons are optional.

## Hardware

* Raspberry Pi Pico
* Adafruit MPR121 Gator Breakout https://www.adafruit.com/product/4830
* STEMMA QT/Qwicc JST cable with header pins https://www.adafruit.com/product/4209
* buttons for button inputs, optional
* conductive material for touch inputs

## Software

This project is based on the Raspberry Pi Pico using the Arduino IDE with the
following board package. See the web site for installation instructions.

https://github.com/earlephilhower/arduino-pico

The following Arduino libraries may be installed using the IDE Library Manager.

* Adafruit BusIO
* Adafruit MPR121
* Bounce2
* Adafruit_TinyUSB
* MIDI Library by FortySevenEffect

## Hardware Hook Up

### Touch Inputs

The 12 capacitive touch inputs are provided by an MPR121 board. The MPR121
board connects to the Pi Pico via I2C bus. The Adafruit MPR121 Gator board is
easy to use but a generic MPR121 board will also work.

MPR121  |Cable  |Raspberry Pi Pico
--------|-------|-----------------
3.3V    |Red    |3V3_OUT (pin 36)
GND     |Black  |GND (pin 3)
SCL     |Blue   |GP1, I2C0_SCL (pin 2)
SDA     |Yellow |GP0, I2C0_SDA (pin 1)

Touch   |MIDI
--------|--------
0       |MIDI note 60
1       |MIDI note 61
2       |MIDI note 62
3       |MIDI note 63
4       |MIDI note 64
5       |MIDI note 65
6       |MIDI note 66
7       |MIDI note 67
8       |MIDI note 68
9       |MIDI note 69
10      |MIDI note 70
11      |MIDI note 71

### Button Inputs

The following GPIO pins are configured as button inputs. Connect one pin
of the button to ground and the other to one of the pins listed below.

Button  |Raspberry Pi Pico|MIDI
--------|-----------------|--------
1       |GP4 (pin 6)      |MIDI note 60
2       |GP5 (pin 7)      |MIDI note 61
3       |GP6 (pin 9)      |MIDI note 62
4       |GP7 (pin 10)     |MIDI note 63
5       |GP8 (pin 11)     |MIDI note 64
6       |GP9 (pin 12)     |MIDI note 65
7       |GP10 (pin 14)    |MIDI note 66
8       |GP11 (pin 15)    |MIDI note 67
