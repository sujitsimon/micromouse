#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\calibration.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "logger.h"
#include "stm32f4xx_hal.h"

#define FLASH_SECTOR7_START_ADDR 0x08060000 // Sector 7 start address

const int DEFAULT_AX = -861;
const int DEFAULT_AY = -6738;
const int DEFAULT_AZ = 1820;
const int DEFAULT_GX = 49;
const int DEFAULT_GY = 4;
const int DEFAULT_GZ = -33;

class MPU6050Calibration
{
public:
    MPU6050Calibration(MPU6050 accelgyro, Logger *loggerInstance, int bufferSize = 1000, int accelDeadzone = 8, int gyroDeadzone = 1)
        : mpu(accelgyro), m_logger(loggerInstance), buffersize(bufferSize), acel_deadzone(accelDeadzone), giro_deadzone(gyroDeadzone),
          state(0) {}

    // void CalibrationSetup() {
    //     Wire.begin();
    //     mpu.initialize();
    //     resetOffsets();
    // }

    void Calibrate()
    {
        resetOffsets();
        meanSensors();
        delay(1000);
        calibrate();
        delay(1000);
        meanSensors();
        printResults();
    }

    void updateOffset(int &ax_offset_var, int &ay_offset_var, int &az_offset_var,
                      int &gx_offset_var, int &gy_offset_var, int &gz_offset_var)
    {
        ax_offset_var = ax_offset;
        ay_offset_var = ay_offset;
        az_offset_var = az_offset;
        gx_offset_var = gx_offset;
        gy_offset_var = gy_offset;
        gz_offset_var = gz_offset;
    }

    // void storeOffsets()
    // {
    //     uint32_t baseAddr = FLASH_SECTOR7_START_ADDR;
    //     Flash_Write(baseAddr + (sizeof(int) * 0), ax_offset);
    //     Flash_Write(baseAddr + (sizeof(int) * 1), ay_offset);
    //     Flash_Write(baseAddr + (sizeof(int) * 2), az_offset);
    //     Flash_Write(baseAddr + (sizeof(int) * 3), gx_offset);
    //     Flash_Write(baseAddr + (sizeof(int) * 4), gy_offset);
    //     Flash_Write(baseAddr + (sizeof(int) * 5), gz_offset);
    // }
    void storeOffsets()
    {
        uint32_t baseAddr = FLASH_SECTOR7_START_ADDR;
        HAL_FLASH_Unlock();

        FLASH_EraseInitTypeDef eraseInitStruct;
        eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        eraseInitStruct.Sector = FLASH_SECTOR_7;
        eraseInitStruct.NbSectors = 1;
        eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

        uint32_t sectorError = 0;
        if (HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return;
        }

        HAL_StatusTypeDef status;
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, baseAddr + (sizeof(int) * 0), ax_offset);
        if (status != HAL_OK)
            goto flash_write_error;

        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, baseAddr + (sizeof(int) * 1), ay_offset);
        if (status != HAL_OK)
            goto flash_write_error;

        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, baseAddr + (sizeof(int) * 2), az_offset);
        if (status != HAL_OK)
            goto flash_write_error;

        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, baseAddr + (sizeof(int) * 3), gx_offset);
        if (status != HAL_OK)
            goto flash_write_error;

        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, baseAddr + (sizeof(int) * 4), gy_offset);
        if (status != HAL_OK)
            goto flash_write_error;

        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, baseAddr + (sizeof(int) * 5), gz_offset);
        if (status != HAL_OK)
            goto flash_write_error;

        HAL_FLASH_Lock();
        return;

    flash_write_error:
        HAL_FLASH_Lock();
    }

    void readOffsetsFromFlash()
    {
        uint32_t baseAddr = FLASH_SECTOR7_START_ADDR;
        ax_offset = Flash_Read(baseAddr + (sizeof(int) * 0));
        ay_offset = Flash_Read(baseAddr + (sizeof(int) * 1));
        az_offset = Flash_Read(baseAddr + (sizeof(int) * 2));
        gx_offset = Flash_Read(baseAddr + (sizeof(int) * 3));
        gy_offset = Flash_Read(baseAddr + (sizeof(int) * 4));
        gz_offset = Flash_Read(baseAddr + (sizeof(int) * 5));
        print_offset();
    }

    void print_offset()
    {
        m_logger->print("Current offsets:\t");
        m_logger->print(ax_offset);
        m_logger->print("\t");
        m_logger->print(ay_offset);
        m_logger->print("\t");
        m_logger->print(az_offset);
        m_logger->print("\t");
        m_logger->print(gx_offset);
        m_logger->print("\t");
        m_logger->print(gy_offset);
        m_logger->print("\t");
        m_logger->println(gz_offset);
        m_logger->println("\nData format: acelX acelY acelZ giroX giroY giroZ");
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

    void resetOffsets()
    {
        mpu.setXAccelOffset(0);
        mpu.setYAccelOffset(0);
        mpu.setZAccelOffset(0);
        mpu.setXGyroOffset(0);
        mpu.setYGyroOffset(0);
        mpu.setZGyroOffset(0);
    }

    void meanSensors()
    {
        long i = 0;
        long buff_ax = 0, buff_ay = 0, buff_az = 0;
        long buff_gx = 0, buff_gy = 0, buff_gz = 0;

        while (i < (buffersize + 101))
        {
            mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

            if (i > 100)
            {
                buff_ax += ax;
                buff_ay += ay;
                buff_az += az;
                buff_gx += gx;
                buff_gy += gy;
                buff_gz += gz;
            }
            if (i == (buffersize + 100))
            {
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

    void calibrate()
    {
        ax_offset = -mean_ax / 8;
        ay_offset = -mean_ay / 8;
        az_offset = (16384 - mean_az) / 8;

        gx_offset = -mean_gx / 4;
        gy_offset = -mean_gy / 4;
        gz_offset = -mean_gz / 4;

        while (true)
        {
            int ready = 0;
            mpu.setXAccelOffset(ax_offset);
            mpu.setYAccelOffset(ay_offset);
            mpu.setZAccelOffset(az_offset);

            mpu.setXGyroOffset(gx_offset);
            mpu.setYGyroOffset(gy_offset);
            mpu.setZGyroOffset(gz_offset);

            meanSensors();
            m_logger->println("...");

            if (abs(mean_ax) <= acel_deadzone)
                ready++;
            else
                ax_offset -= mean_ax / acel_deadzone;

            if (abs(mean_ay) <= acel_deadzone)
                ready++;
            else
                ay_offset -= mean_ay / acel_deadzone;

            if (abs(16384 - mean_az) <= acel_deadzone)
                ready++;
            else
                az_offset += (16384 - mean_az) / acel_deadzone;

            if (abs(mean_gx) <= giro_deadzone)
                ready++;
            else
                gx_offset -= mean_gx / (giro_deadzone + 1);

            if (abs(mean_gy) <= giro_deadzone)
                ready++;
            else
                gy_offset -= mean_gy / (giro_deadzone + 1);

            if (abs(mean_gz) <= giro_deadzone)
                ready++;
            else
                gz_offset -= mean_gz / (giro_deadzone + 1);

            if (ready == 6)
                break;
        }
    }

    void printResults()
    {
        m_logger->println("\nFINISHED!");
        m_logger->print("\nSensor readings with offsets:\t");
        m_logger->print(mean_ax);
        m_logger->print("\t");
        m_logger->print(mean_ay);
        m_logger->print("\t");
        m_logger->print(mean_az);
        m_logger->print("\t");
        m_logger->print(mean_gx);
        m_logger->print("\t");
        m_logger->print(mean_gy);
        m_logger->print("\t");
        m_logger->println(mean_gz);
        m_logger->print("Your offsets:\t");
        m_logger->print(ax_offset);
        m_logger->print("\t");
        m_logger->print(ay_offset);
        m_logger->print("\t");
        m_logger->print(az_offset);
        m_logger->print("\t");
        m_logger->print(gx_offset);
        m_logger->print("\t");
        m_logger->print(gy_offset);
        m_logger->print("\t");
        m_logger->println(gz_offset);
        m_logger->println("\nData format: acelX acelY acelZ giroX giroY giroZ");
        m_logger->println("Check that readings are close to 0 0 16384 0 0 0.");
        m_logger->println("Record offsets for future use.");
    }

    void Flash_Write(uint32_t address, uint32_t data)
    {
        HAL_FLASH_Unlock();

        FLASH_EraseInitTypeDef eraseInitStruct;
        uint32_t pageError = 0;

        eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        eraseInitStruct.Sector = FLASH_SECTOR_2;
        eraseInitStruct.NbSectors = 1;
        eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

        if (HAL_FLASHEx_Erase(&eraseInitStruct, &pageError) != HAL_OK)
        {
            // Handle error
        }
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data) != HAL_OK)
        {
            // Handle error
        }

        // Lock the Flash
        HAL_FLASH_Lock();
    }

    int Flash_Read(uint32_t address)
    {
        return *(__IO uint32_t *)address;
    }
};