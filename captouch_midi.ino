/************************************************************************
  MIT License

  Copyright (c) 2021 touchgadgetdev@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 *************************************************************************/

/*
 * 12 cap touch inputs send Note On/Off starting with note 60
 *
 * 8 GPIO pins also send Note On/Off starting with note 60
 * 
 * Use with this Arduino board package.
 *
 * https://github.com/earlephilhower/arduino-pico
 *
 * Pi Pico USB Stack must be set to Adafruit_TinyUSB for this to work.
 */

#include <Bounce2.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>             // FortySevenEffects MIDI Library
#include <Wire.h>
#include "Adafruit_MPR121.h"

#define BASE_NOTE 60

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

Adafruit_MPR121 touch = Adafruit_MPR121();

#define NUM_BUTTONS 8
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {4, 5, 6, 7, 8, 9, 10, 11};

Bounce * buttons = new Bounce[NUM_BUTTONS];

bool USE_TOUCH = true;

void setup()
{
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  
  Serial.begin(115200);

  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].attach( BUTTON_PINS[i], INPUT_PULLUP );
    buttons[i].interval(5);
  }

  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();
  if (!touch.begin(0x5A)) {
    Serial.println("MPR121 not found");
    USE_TOUCH = false;
  }

  // wait until device mounted
  while( !USBDevice.mounted() ) delay(1);
}

void loop()
{
  static uint16_t touchold;

  if (USE_TOUCH) {
    uint16_t touchnow = touch.touched();
    uint16_t changed = touchnow ^ touchold;

    if (changed) {
      uint16_t bitmask = 1;
      for (int i = 0; i < 12; i++) {
        if (changed & bitmask) {
          if (touchnow & bitmask) {
            MIDI.sendNoteOn(BASE_NOTE + i, 127, 1);
          }
          else {
            MIDI.sendNoteOff(BASE_NOTE + i, 0, 1);
          }
        }
        bitmask <<= 1;
      } // for each touch input
      touchold = touchnow;
    } // if at least one touch input changed
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].update();
    if ( buttons[i].fell() ) {
      MIDI.sendNoteOn(BASE_NOTE + i, 127, 1);
    }
    else if ( buttons[i].rose() ) {
      MIDI.sendNoteOff(BASE_NOTE + i, 0, 1);
    }
  }
  MIDI.read();
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  // Log when a note is pressed.
  Serial.printf("Note on: channel = %d, pitch = %d, velocity - %d", channel, pitch, velocity);
  Serial.println();
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  // Log when a note is released.
  Serial.printf("Note off: channel = %d, pitch = %d, velocity - %d", channel, pitch, velocity);
  Serial.println();
}
