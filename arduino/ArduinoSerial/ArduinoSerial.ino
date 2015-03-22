#include "Diagnostics.h"

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
}

void loop() {
  //Serial.print("160.2,3.704,102.4,5.012,24.6,48");
  GetDiagnostics(&Diagnostics);
  SendDiagnostics(Diagnostics);
  delay(1000);
}

