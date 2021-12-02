#include <Servo.h>

// enable servo movement
#define ENABLE_ARM 1
#define ENABLE_BASE 1

// time step t_s
#define DELAY 5

// LDR class, just a wrapper around an analog pin
class LDR {
   private:
    const uint8_t pin;

   public:
    LDR(int pin) : pin{pin} {
    }

    int read() {
        return analogRead(pin);
    }
};

// PID controller class for PID control in a discrete time domain
class PID {
   private:
    // constants for controller params
    const double Kp;
    const double Ki;
    const double Kd;
    // time step
    const double t_s;

    // Integral value at time t-1
    double I_t_1;
    // error value at time t-1
    double e_t_1;

   public:
    // initalise constants, set t-1 vals to 0
    PID(double Kp, double Ki, double Kd, double t_s) : Kp{Kp}, Ki{Ki}, Kd{Kd}, t_s{t_s} {
        I_t_1 = 0;
        e_t_1 = 0;
    }

    // calculate control signal
    double calculate(double e_t) {
        // calculate integral and derivative terms
        double I_t = I_t_1 + e_t * t_s;
        double D_t = (e_t - e_t_1) / t_s;

        // update values for next call
        e_t_1 = e_t;
        I_t_1 = I_t;

        // return control output
        return Kp * e_t + Ki * I_t + Kd * D_t;
    }
};

// our two servos
Servo base;
Servo arm;

// the four LDRS
// A | B
// - - -
// C | D
LDR A(A3);
LDR B(A2);
LDR C(A1);
LDR D(A0);

// two PID controllers with same parameters
PID arm_controller(0.025, 0.0000, 0.017, DELAY);
PID base_controller(0.025, 0.0000, 0.017, DELAY);

void setup() {
    // attach servos to pins and centre them
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

    // calculate error values
    int e_base = E - W;
    int e_arm = S - N;

    // control signals for each servo
    double d_base = base_controller.calculate(e_base);
    double d_arm = arm_controller.calculate(e_arm);

    // update values
    if (ENABLE_BASE)
        base.write(base.read() + d_base);
    if (ENABLE_ARM) {
        // cap arm at 70 degrees so it doesnt flip backwards
        if (arm.read() + d_arm < 70)
            arm.write(70);
        else
            arm.write(arm.read() + d_arm);
    }
    // give the servos time to move
    delay(DELAY);
}
