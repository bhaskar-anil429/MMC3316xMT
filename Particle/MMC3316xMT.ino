// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MMC3316xMT
// This code is designed to work with the MMC3316xMT_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Magnetic-Sensor?sku=MMC3316xMT_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// MMC3316xMT I2C address is 0x30(48)
#define Addr 0x30

int xMag = 0, yMag = 0, zMag = 0;
void setup()
{
    // Set variable
    Particle.variable("i2cdevice", "MAG3110");
    Particle.variable("xMag", xMag);
    Particle.variable("yMag", yMag);
    Particle.variable("zMag", zMag);
    
    // Initialise I2C communication as MASTER 
    Wire.begin();
    // Initialise Serial Communication
    Serial.begin(9600);
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select control register
    Wire.write(0x07);
    // Initiate measurement, continuous mode, coil set
    Wire.write(0x23);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select control register
    Wire.write(0x07);
    // Coil Not set
    Wire.write(0x00);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select control register-0
    Wire.write(0x07);
    // Initiate measurement, continuous mode , Coil RESET
    Wire.write(0x43);
    // Stop I2C Transmission
    Wire.endTransmission();
    delay(300);
}

void loop() 
{ 
    unsigned int data[6];
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select magnetometer data register
    Wire.write(0x00);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Request 6 bytes of data
    Wire.requestFrom(Addr, 6);
    
    // Read 6 bytes of data
    // xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
    if(Wire.available() == 6)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
        data[4] = Wire.read();
        data[5] = Wire.read();
    }
    delay(300);
    
    // Convert the data
    xMag = (((data[1] & 0x3F) * 256) + data[0]);
    if (xMag > 8191)
    {
        xMag -= 16384;
    } 
    yMag = (((data[3] & 0x3F) * 256) + data[2]);
    if (yMag > 8191)
    {
        yMag -= 16384;
    }
    zMag = (((data[5] & 0x3F) * 256) + data[4]);
    if (zMag > 8191)
    {
        zMag -= 16384;
    }
  
    // Output data to dashboard
    Particle.publish("Magnetic field in X Axis : ", String(xMag));
    Particle.publish("Magnetic field in Y Axis : ", String(yMag));
    Particle.publish("Magnetic field in Z Axis : ", String(zMag));
    delay(1000);
}

