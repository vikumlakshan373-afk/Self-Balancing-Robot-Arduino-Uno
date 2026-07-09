#include <Wire.h>

const int MPU = 0x68;

long AccX, AccY, AccZ;
long GyroX, GyroY, GyroZ;

float AccErrorX = 0;
float AccErrorY = 0;

float GyroErrorX = 0;
float GyroErrorY = 0;
float GyroErrorZ = 0;

void setup()
{
    Serial.begin(115200);

    Wire.begin();

    Wire.beginTransmission(MPU);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    Serial.println("Keep MPU6050 still...");
    delay(2000);

    calculateError();

    Serial.println("Calibration Finished");
}

void loop()
{
    readRaw();

    Serial.print("AccX:");
    Serial.print(AccX);

    Serial.print(" AccY:");
    Serial.print(AccY);

    Serial.print(" AccZ:");
    Serial.print(AccZ);

    Serial.print(" GyroX:");
    Serial.print(GyroX);

    Serial.print(" GyroY:");
    Serial.print(GyroY);

    Serial.print(" GyroZ:");
    Serial.println(GyroZ);

    delay(100);
}

void readRaw()
{
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU,14,true);

    AccX = Wire.read()<<8 | Wire.read();
    AccY = Wire.read()<<8 | Wire.read();
    AccZ = Wire.read()<<8 | Wire.read();

    Wire.read();
    Wire.read();

    GyroX = Wire.read()<<8 | Wire.read();
    GyroY = Wire.read()<<8 | Wire.read();
    GyroZ = Wire.read()<<8 | Wire.read();

    GyroX -= GyroErrorX;
    GyroY -= GyroErrorY;
    GyroZ -= GyroErrorZ;
}

void calculateError()
{
    for(int i=0;i<500;i++)
    {
        readRaw();

        GyroErrorX += GyroX;
        GyroErrorY += GyroY;
        GyroErrorZ += GyroZ;

        delay(5);
    }

    GyroErrorX /= 500;
    GyroErrorY /= 500;
    GyroErrorZ /= 500;

    Serial.print("Gyro Offset X = ");
    Serial.println(GyroErrorX);

    Serial.print("Gyro Offset Y = ");
    Serial.println(GyroErrorY);

    Serial.print("Gyro Offset Z = ");
    Serial.println(GyroErrorZ);
}