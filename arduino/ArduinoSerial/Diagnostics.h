#include "Arduino.h"

#define BEGIN_FLAG  0xBB  //Marks the beginning of a diagnostic packet
#define END_FLAG    0xEE  //Marks the end of a diagnostic packet

typedef struct Diagnostics_t {
  double InputCurrent;
  double InputVoltage;
  double OutputCurrent;
  double OutputVoltage;
  double Temperature;
  double Humidity;
} Diagnostics_t;

extern Diagnostics_t Diagnostics;

//DUMMY METHODS...
double GetInputCurrent(void) {
  return 160.2;
}

double GetInputVoltage(void) {
  return 3.704; 
}

double GetOutputCurrent(void) {
  return 102.4; 
}

double GetOutputVoltage(void) {
  return 5.012; 
}

double GetTemperature(void) {
  return 24.6; 
}

double GetHumidity(void) {
  return 48; 
}

void InitializeDiagnostics(Diagnostics_t * d) {
  d->InputCurrent = 0;
  d->InputVoltage = 0;
  d->OutputCurrent = 0;
  d->OutputVoltage = 0;
  d->Temperature = 0;
  d->Humidity = 0;
}

void GetDiagnostics(Diagnostics_t * d) {
  d->InputCurrent = GetInputCurrent();
  d->InputVoltage = GetInputVoltage();
  d->OutputCurrent = GetOutputCurrent();
  d->OutputVoltage = GetOutputVoltage();
  d->Temperature = GetTemperature();
  d->Humidity = GetHumidity();
}

void SendDiagnostics(Diagnostics_t d) {
  //@TODO: Convert all values to char * and compute CRC Checksum
  Serial.print((unsigned long)BEGIN_FLAG);
  Serial.print(",");
  Serial.print(d.InputCurrent);
  Serial.print(",");
  Serial.print(d.InputVoltage);
  Serial.print(",");
  Serial.print(d.OutputCurrent);
  Serial.print(",");
  Serial.print(d.OutputVoltage);
  Serial.print(",");
  Serial.print(d.Temperature);
  Serial.print(",");
  Serial.print(d.Humidity);
  Serial.print(",");
  Serial.print((unsigned long)END_FLAG);
  
  //FOR DEBUGGING
  //Serial.println("");
}
