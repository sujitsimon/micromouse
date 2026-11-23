#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\accleration.cpp"
#include "accleration.h"

Accleration::Accleration(PID_Control left_motor_pid, PID_Control right_motor_pid, int step_size, Logger *loggerInstance) 
    : left_motor_speed(0), right_motor_speed(0), step_size(step_size) {
        LeftMotor = new Encoder(LEFT_MOTOR_PWM, LEFT_MOTOR_P, LEFT_MOTOR_N, left_motor_pid, MAX_MOTOR_POWER, loggerInstance);
        RightMotor = new Encoder(RIGHT_MOTOR_PWM, RIGHT_MOTOR_P, RIGHT_MOTOR_N, right_motor_pid, MAX_MOTOR_POWER, loggerInstance);
    }

void Accleration::accelerateLeftMotor(int target_speed) {
    if (left_motor_speed < target_speed) {
        left_motor_speed += step_size;
        if (left_motor_speed > target_speed) {
            left_motor_speed = target_speed;
        }
    } else if (left_motor_speed > target_speed) {
        decelerateLeftMotor(target_speed);
    }
}

void Accleration::accelerateRightMotor(int target_speed) {
    if (right_motor_speed < target_speed) {
        right_motor_speed += step_size;
        if (right_motor_speed > target_speed) {
            right_motor_speed = target_speed;
        }
    } else if (right_motor_speed > target_speed) {
        decelerateRightMotor(target_speed);
    }
}

void Accleration::decelerateLeftMotor(int target_speed) {
    if (left_motor_speed > target_speed) {
        left_motor_speed -= step_size;
        if (left_motor_speed < target_speed) {
            left_motor_speed = target_speed;
        }
    }
}

void Accleration::decelerateRightMotor(int target_speed) {
    if (right_motor_speed > target_speed) {
        right_motor_speed -= step_size;
        if (right_motor_speed < target_speed) {
            right_motor_speed = target_speed;
        }
    }
}

void Accleration::acclerateMotors(int left_motor_target_speed, int right_motor_target_speed) {
    double speedRatio = left_motor_target_speed / right_motor_target_speed;
    if (left_motor_speed < left_motor_target_speed) {
        left_motor_speed += (step_size * speedRatio);
        if (left_motor_speed > left_motor_target_speed) {
            left_motor_speed = left_motor_target_speed;
        }
    }  
    if (right_motor_speed < right_motor_target_speed) {
        right_motor_speed += (step_size * (1 + (1 - speedRatio)));
        if (right_motor_speed > right_motor_target_speed) {
            right_motor_speed = right_motor_target_speed;
        }
    }
    if ((right_motor_speed < right_motor_target_speed) && (left_motor_speed < left_motor_target_speed)) {
        decelerateMotors(left_motor_target_speed, right_motor_target_speed);
    }
}

void Accleration::decelerateMotors(int left_motor_target_speed, int right_motor_target_speed) {
    double speedRatio = left_motor_target_speed / right_motor_target_speed;
    if (left_motor_speed > left_motor_target_speed) {
        left_motor_speed -= (step_size * speedRatio);
        if (left_motor_speed < left_motor_target_speed) {
            left_motor_speed = left_motor_target_speed;
        }
    }
    if (right_motor_speed > right_motor_target_speed) {
        right_motor_speed -= (step_size * (1 + (1 - speedRatio)));
        if (right_motor_speed < right_motor_target_speed) {
            right_motor_speed = right_motor_target_speed;
        }
    }

}

int Accleration::getLeftMotorSpeed() const {
    return left_motor_speed;
}

int Accleration::getRightMotorSpeed() const {
    return right_motor_speed;
}

Encoder* Accleration::getLeftMotor() const {
    return LeftMotor;
}

Encoder* Accleration::getRightMotor() const {
    return RightMotor;
}