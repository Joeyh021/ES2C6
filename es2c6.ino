#include <Servo.h>

class LDR {
   private:
    int val;
    uint8_t pin;

   public:
    LDR(int pin) {
        this->pin = pin;
    }

    int read() {
        return val = analogRead(pin);
    }
};

Servo base;
Servo arm;
// A | B
// - - -
// C | D

LDR A(A3);
LDR B(A2);
LDR C(A1);
LDR D(A0);


#define Kp 0.02
#define Ki 1
#define Kd 1

void LDRdbg() {
    Serial.print("LDR Values: A: ");
    Serial.print(A.read());
    Serial.print(" B: ");
    Serial.print(B.read());
    Serial.print(" C: ");
    Serial.print(C.read());
    Serial.print(" D: ");
    Serial.print(D.read());
    Serial.println();
}


void setup() {
    // init serial monitor
    Serial.begin(9600);
    arm.attach(9);
    arm.write(160);
    base.attach(11);
    base.write(90);
}

void loop() {
    // calculate the 4 light values
    int N = A.read() + B.read() / 2;
    int E = B.read() + D.read() / 2;
    int S = C.read() + D.read() / 2;
    int W = A.read() + C.read() / 2;
    int avg = A.read() + B.read() + C.read() + D.read() / 4;
    Serial.print(N);
    Serial.print(" ");
    Serial.print(E);
    Serial.print(" ");
    Serial.print(S);
    Serial.print(" ");
    Serial.print(W);
    Serial.print(" ");
    Serial.print(arm.read());
    Serial.print(" ");
    Serial.print(base.read());
    Serial.print(" ");
    Serial.println(" ");

    int baseLight = E - W;
    int armLight  = S - N;

    int d_base = (E-W) * Kp;
    int d_arm = (S-N) * Kp;

    //update values
    base.write(base.read() + d_base);
    if(arm.read() + d_arm < 80)
      arm.write(80);
    else
      arm.write(arm.read() + d_arm);
    
    delay(10);

  

}
