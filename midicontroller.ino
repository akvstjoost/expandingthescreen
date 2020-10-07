//Arduino Leonardo midicontroller
//send A0-A5 to MIDI control changes on controller numbers 0-5
//respond to midi control changes on controller numbers 2-13 with digital pins, 9=servo
//by Thomas Rutgers (2020) - Techlab / St Joost School of Art & Design - ts.rutgers [at] avans.nl

#include <Servo.h>
#include "MIDIUSB.h"
Servo servo;

void setup() {
  //configute digital pins as output pins, except pin 9 as a servo
  for (int n = 2; n <= 13; n++) {
    if (n != 9)   pinMode(n, OUTPUT);
  }
  servo.attach(9);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  
  //read midi
  midiEventPacket_t rx;
  rx = MidiUSB.read();
  if (rx.header == 0x0B) { // control change
    if (rx.byte2 >= 2 && rx.byte2 <= 13) {
      if (rx.byte2 == 9) {
        servo.write(map(rx.byte3, 0, 127, 0, 180));
      }
      else {
        digitalWrite(rx.byte2, rx.byte3);
      }
    }
  }

  //write midi
  for (int n = 0; n <= 5; n++) {
    int value = map(analogRead(n + 18), 0, 1023, 0, 127); //A0-A5 are on pin 18-23. scale value to range 0-127
    controlChange(0, n, value);
  }
  
  //wait
  delay(10);
}
