#include <Servo.h>
#define ENABLE_ARM true
#define ENABLE_BASE true

#define DELAY 10

class LDR {
   private:
    int val;
    const uint8_t pin;

   public:
    LDR(int pin) : pin{pin} {
    }

    int read() {
        return val = analogRead(pin);
    }
};

class PID {
   private:
    const double Kp;
    const double Ki;
    const double Kd;
    const double t_s;

    double I_t_1;
    double e_t_1;

   public:
    PID(double Kp, double Ki, double Kd, double t_s) : Kp{Kp}, Ki{Ki}, Kd{Kd}, t_s{t_s} {
        I_t_1 = 0;
        e_t_1 = 0;
    }

    double calculate(double e_t) {
        double I_t = I_t_1 + e_t * t_s;
        double D_t = (e_t - e_t_1) / t_s;

        // update values for next call
        e_t_1 = e_t;
        I_t_1 = I_t;
        return Kp * e_t + Ki * I_t + Kd * D_t;
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

PID arm_controller(0.02, 0, 0, DELAY);
PID base_controller(0.02, 0, 0, DELAY);

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
    base.write(80);
}

void loop() {
    // calculate the 4 light values
    int N = (A.read() + B.read()) / 2;
    int E = (B.read() + D.read()) / 2;
    int S = (C.read() + D.read()) / 2;
    int W = (A.read() + C.read()) / 2;

    int e_base = E - W;
    int e_arm = S - N;

    double d_base = base_controller.calculate(e_base);
    double d_arm = arm_controller.calculate(e_arm);

    // update values
    if (ENABLE_BASE)
        base.write(base.read() + d_base);
    if (ENABLE_ARM) {
        if (arm.read() + d_arm < 70)
            arm.write(70);
        else
            arm.write(arm.read() + d_arm);
    }
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
    delay(DELAY);
}
