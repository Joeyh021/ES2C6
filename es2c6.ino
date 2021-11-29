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
    arm.write(90);
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

    // if (N > S && arm.read() <= 180)
    //     arm.write(arm.read() - 1);
    // if (N < S && arm.read() >= 0)
    //     arm.write(arm.read() + 1);

    // if (E > W && base.read() <= 180)
    //     base.write(base.read() + 1);
    // if (E < W && base.read() >= 0)
    //     base.write(base.read() - 1);

    delay(5);

    LDRdbg();
}
