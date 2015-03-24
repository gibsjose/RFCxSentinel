#include "Diagnostics.h"

char buffer[1024];
Diagnostics_t Diagnostics;

void setup() {
  //Initialize Diagnostics structure
  InitializeDiagnostics(&Diagnostics);

  //Initialize serial port
  Serial.begin(115200);

  //Initialize Timers, etc...
  //...

  //Initialize ADC
  //...

  //Seed random number generator
  randomSeed(analogRead(0));
}

void loop() {
  //Take measurements
  GetDiagnostics(&Diagnostics);

  //Send measurements
  SendDiagnostics(&Diagnostics, buffer);

  //Delay 5 seconds
  delay(1000);
}
