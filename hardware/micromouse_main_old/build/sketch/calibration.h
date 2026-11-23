#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main_old\\calibration.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "logger.h"

const int DEFAULT_AX = -26;
const int DEFAULT_AY = -3939;
const int DEFAULT_AZ = 193;
const int DEFAULT_GX = 124;
const int DEFAULT_GY = -117;
const int DEFAULT_GZ = 27;

class MPU6050Calibration {
public:
    MPU6050Calibration(MPU6050 accelgyro, Logger *loggerInstance, int address = 0x68, int bufferSize = 1000, int accelDeadzone = 8, int gyroDeadzone = 1)
        : mpu(accelgyro), m_logger(loggerInstance), buffersize(bufferSize), acel_deadzone(accelDeadzone), giro_deadzone(gyroDeadzone),
          state(0) {}

    // void CalibrationSetup() {
    //     Wire.begin();
    //     mpu.initialize();
    //     resetOffsets();
    // }

    void Calibrate() {    
        resetOffsets();
        switch (state) {
            case 0:
                meanSensors();
                state++;
                delay(1000);
                break;

            case 1:
                calibrate();
                state++;
                delay(1000);
                break;

            case 2:
                meanSensors();
                printResults();
                while (1);
        }
    }

    void updateOffset(int& ax_offset_var, int& ay_offset_var, int& az_offset_var, 
                        int& gx_offset_var, int& gy_offset_var, int& gz_offset_var) {
        ax_offset_var = ax_offset;
        ay_offset_var = ay_offset;
        az_offset_var = az_offset;
        gx_offset_var = gx_offset;
        gy_offset_var = gy_offset;
        gz_offset_var = gz_offset;
    }


    void storeOffset() {
        //do something to store offset values
    }




private:
    MPU6050 mpu;
    int buffersize;
    int acel_deadzone;
    int giro_deadzone;

    int16_t ax, ay, az, gx, gy, gz;
    int mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz;
    int ax_offset = DEFAULT_AX, ay_offset = DEFAULT_AY, az_offset = DEFAULT_AZ, gx_offset = DEFAULT_GX, gy_offset = DEFAULT_GY, gz_offset = DEFAULT_GZ;
    int state;
    Logger *m_logger;

    void resetOffsets() {
        mpu.setXAccelOffset(0);
        mpu.setYAccelOffset(0);
        mpu.setZAccelOffset(0);
        mpu.setXGyroOffset(0);
        mpu.setYGyroOffset(0);
        mpu.setZGyroOffset(0);
    }

    void meanSensors() {
        long i = 0;
        long buff_ax = 0, buff_ay = 0, buff_az = 0;
        long buff_gx = 0, buff_gy = 0, buff_gz = 0;

        while (i < (buffersize + 101)) {
            mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

            if (i > 100) {
                buff_ax += ax;
                buff_ay += ay;
                buff_az += az;
                buff_gx += gx;
                buff_gy += gy;
                buff_gz += gz;
            }
            if (i == (buffersize + 100)) {
                mean_ax = buff_ax / buffersize;
                mean_ay = buff_ay / buffersize;
                mean_az = buff_az / buffersize;
                mean_gx = buff_gx / buffersize;
                mean_gy = buff_gy / buffersize;
                mean_gz = buff_gz / buffersize;
            }
            i++;
            delay(2);
        }
    }

    void calibrate() {
        ax_offset = -mean_ax / 8;
        ay_offset = -mean_ay / 8;
        az_offset = (16384 - mean_az) / 8;

        gx_offset = -mean_gx / 4;
        gy_offset = -mean_gy / 4;
        gz_offset = -mean_gz / 4;

        while (true) {
            int ready = 0;
            mpu.setXAccelOffset(ax_offset);
            mpu.setYAccelOffset(ay_offset);
            mpu.setZAccelOffset(az_offset);

            mpu.setXGyroOffset(gx_offset);
            mpu.setYGyroOffset(gy_offset);
            mpu.setZGyroOffset(gz_offset);

            meanSensors();
            Serial.println("...");

            if (abs(mean_ax) <= acel_deadzone) ready++;
            else ax_offset -= mean_ax / acel_deadzone;

            if (abs(mean_ay) <= acel_deadzone) ready++;
            else ay_offset -= mean_ay / acel_deadzone;

            if (abs(16384 - mean_az) <= acel_deadzone) ready++;
            else az_offset += (16384 - mean_az) / acel_deadzone;

            if (abs(mean_gx) <= giro_deadzone) ready++;
            else gx_offset -= mean_gx / (giro_deadzone + 1);

            if (abs(mean_gy) <= giro_deadzone) ready++;
            else gy_offset -= mean_gy / (giro_deadzone + 1);

            if (abs(mean_gz) <= giro_deadzone) ready++;
            else gz_offset -= mean_gz / (giro_deadzone + 1);

            if (ready == 6) break;
        }
    }

    void printResults() {
        Serial.println("\nFINISHED!");
        Serial.print("\nSensor readings with offsets:\t");
        Serial.print(mean_ax); Serial.print("\t");
        Serial.print(mean_ay); Serial.print("\t");
        Serial.print(mean_az); Serial.print("\t");
        Serial.print(mean_gx); Serial.print("\t");
        Serial.print(mean_gy); Serial.print("\t");
        Serial.println(mean_gz);
        Serial.print("Your offsets:\t");
        Serial.print(ax_offset); Serial.print("\t");
        Serial.print(ay_offset); Serial.print("\t");
        Serial.print(az_offset); Serial.print("\t");
        Serial.print(gx_offset); Serial.print("\t");
        Serial.print(gy_offset); Serial.print("\t");
        Serial.println(gz_offset);
        Serial.println("\nData format: acelX acelY acelZ giroX giroY giroZ");
        Serial.println("Check that readings are close to 0 0 16384 0 0 0.");
        Serial.println("Record offsets for future use.");
    }
};