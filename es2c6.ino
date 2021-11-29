#include <Servo.h>

Servo s1;
Servo s2;

int pos = 0;  // variable to store the servo position

int LDRpin[] = {A0,A1,A2,A3};
int LDRval[4];
void setup() {
    s1.attach(9);  // attaches the servo on pin 9 to the servo object
    s2.attach(11);
    Serial.begin(9600);
}

void loop() {
    for(int i=0; i <4 ; i++){
      LDRval[i] = analogRead(LDRpin[i]);
      Serial.print(LDRval[i]);
      Serial.print(" ");
    }
    Serial.print("\n");
    for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        s1.write(pos);  
        s2.write(pos);
        delay(5);         
    }
    for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
        s1.write(pos);
        s1.write(pos);                 
        delay(5);                          
    }
    
}
