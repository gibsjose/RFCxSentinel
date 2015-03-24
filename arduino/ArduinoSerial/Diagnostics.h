#include "Arduino.h"

#define BEGIN_FLAG  0x7B  //Marks the beginning of a diagnostic packet  //0111 1011
#define END_FLAG    0x7E  //Marks the end of a diagnostic packet        //0111 1110

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
  long rand = random(16000, 18000);
  return (double)rand / 100.0;
}

double GetInputVoltage(void) {
  long rand = random(350, 370);
  return (double)rand / 100.0;
}

double GetOutputCurrent(void) {
  long rand = random(10000, 12000);
  return (double)rand / 100.0;
}

double GetOutputVoltage(void) {
  long rand = random(480, 510);
  return (double)rand / 100.0;
}

double GetTemperature(void) {
  long rand = random(2200, 2500);
  return (double)rand / 100.0;
}

double GetHumidity(void) {
  long rand = random(4400, 5200);
  return (double)rand / 100.0;
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

//Serialize the diagnostics
void Serialize(Diagnostics_t * d, char * buffer) {
    char tmp[16];
    memset(buffer, 0, 1024);

    //Use `dtostrf`: char *dtostrf(double val, signed char width, unsigned char prec, char *s) for floats

    //Begin flag
    sprintf(tmp, "%lu", (unsigned long)BEGIN_FLAG);
    strcat(buffer, tmp);
    strcat(buffer, ",");

    //Input Current
    dtostrf(d->InputCurrent, 4, 3, tmp);
    strcat(buffer, tmp);
    strcat(buffer, ",");

    //Input Voltage
    dtostrf(d->InputVoltage, 2, 3, tmp);
    strcat(buffer, tmp);
    strcat(buffer, ",");

    //Output Current
    dtostrf(d->OutputCurrent, 4, 3, tmp);
    strcat(buffer, tmp);
    strcat(buffer, ",");

    //Output Voltage
    dtostrf(d->OutputVoltage, 2, 3, tmp);
    strcat(buffer, tmp);
    strcat(buffer, ",");

    //Temperature
    dtostrf(d->Temperature, 2, 1, tmp);
    strcat(buffer, tmp);
    strcat(buffer, ",");

    //Humidity
    dtostrf(d->Humidity, 3, 0, tmp);
    strcat(buffer, tmp);
    strcat(buffer, ",");

    //End flag
    sprintf(tmp, "%lu", (unsigned long)END_FLAG);
    strcat(buffer, tmp);
}

void SerializeRaw(Diagnostics_t * d, char * buffer) {
    char * p = buffer;
    memset(buffer, 0, 1024);

    unsigned long begin_flag = (unsigned long)BEGIN_FLAG;
    unsigned long end_flag = (unsigned long)END_FLAG;

    //Begin flag
    memcpy(p, (char *)&begin_flag, sizeof(unsigned long));
    p += sizeof(unsigned long);

    //InputCurrent
    memcpy(p, (char *)&(d->InputCurrent), sizeof(double));
    p += sizeof(double);

    //InputVoltage
    memcpy(p, (char *)&(d->InputVoltage), sizeof(double));
    p += sizeof(double);

    //OutputCurrent
    memcpy(p, (char *)&(d->OutputCurrent), sizeof(double));
    p += sizeof(double);

    //OutputVoltage
    memcpy(p, (char *)&(d->OutputVoltage), sizeof(double));
    p += sizeof(double);

    //Temperature
    memcpy(p, (char *)&(d->Temperature), sizeof(double));
    p += sizeof(double);

    //Humidity
    memcpy(p, (char *)&(d->Humidity), sizeof(double));
    p += sizeof(double);

    //End flag
    memcpy(p, (char *)&end_flag, sizeof(unsigned long));
    p += sizeof(unsigned long);
}

void SendDiagnostics(Diagnostics_t * d, char * buffer) {
  Serialize(d, buffer);
  Serial.write(buffer, strlen(buffer));
  Serial.flush();
}

void SendDiagnosticsRaw(Diagnostics_t * d, char * buffer) {
  SerializeRaw(d, buffer);
  Serial.write(buffer, (2 * sizeof(unsigned long)) + sizeof(Diagnostics_t));
  Serial.flush();
}
