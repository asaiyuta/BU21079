#include <Wire.h>
#include "BU21079.h"

BU21079_RawDataPerther sens(10);

void waitSerial(){
    while(Serial.available() == 0){}
    while(Serial.available()){ Serial.read(); }
}

void setup(){
    Serial.begin(115200);
    sens.begin();
    waitSerial();
    while(!sens.isHWInted()){}
    Serial.println("HWInted!!");
    waitSerial();
    sens.UNIT_SENS_NUM(0).TH_SLEEP_H(0);
    sens.initializeEnd();
    while(!sens.isSWCaled()){}
    Serial.println("SWcaled!!");
    waitSerial();
    Serial.println("START SENS");
}

void loop(){
        Serial.print(sens.SIN_DATA0());
        Serial.print(sens.SIN_DATA1());
        Serial.print(sens.SIN_DATA2());
        Serial.print(sens.SIN_DATA3());
        Serial.print(sens.SIN_DATA4());
        Serial.print(sens.SIN_DATA5());
        Serial.print(sens.SIN_DATA6());
        Serial.println(sens.SIN_DATA7());
        delay(15);
}
