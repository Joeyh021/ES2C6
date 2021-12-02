#include <Servo.h>
#define ENABLE_ARM 1
#define ENABLE_BASE 1

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

    double I_t_1;
    double e_t_1;
    unsigned long t_1;

   public:
    PID(double Kp, double Ki, double Kd) : Kp{Kp}, Ki{Ki}, Kd{Kd} {
        I_t_1 = 0;
        e_t_1 = 0;
        t_1 = 0;
    }

    double calculate(double e_t, unsigned long t) {
        unsigned long t_s = t - t_1;
        double I_t = I_t_1 + e_t * t_s;
        double D_t = (e_t - e_t_1) / t_s;

        // update values for next call
        e_t_1 = e_t;
        I_t_1 = I_t;
        t_1 = t;
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

PID arm_controller(0.025, 0, 0.017);
PID base_controller(0.025, 0, 0.017);

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

    double d_base = base_controller.calculate(e_base, micros());
    double d_arm = arm_controller.calculate(e_arm, micros());

    // update values
    if (ENABLE_BASE)
        base.write(base.read() + d_base);
    if (ENABLE_ARM) {
        if (arm.read() + d_arm < 70)
            arm.write(70);
        else
            arm.write(arm.read() + d_arm);
    }
    LDRdbg();
}
