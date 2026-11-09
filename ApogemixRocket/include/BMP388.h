#ifndef BMP388_H
#define BMP388_H

#include <Arduino.h>
#include <Wire.h>

// TOKMAS BMP388 pressure/temperature oversampling encodings.
enum : uint8_t {
    BMP388_OS_1X = 0,
    BMP388_OS_2X = 1,
    BMP388_OS_4X = 2,
    BMP388_OS_8X = 3,
    BMP388_OS_16X = 4,
    BMP388_OS_32X = 5,
    BMP388_OS_64X = 6,
    BMP388_OS_128X = 7
};

// TOKMAS rate encoding. Unlike the Bosch part, this device has no 12.5 Hz rate.
enum : uint8_t {
    BMP388_ODR_1_HZ = 0,
    BMP388_ODR_2_HZ = 1,
    BMP388_ODR_4_HZ = 2,
    BMP388_ODR_8_HZ = 3,
    BMP388_ODR_16_HZ = 4,
    BMP388_ODR_32_HZ = 5,
    BMP388_ODR_64_HZ = 6,
    BMP388_ODR_128_HZ = 7
};

class BMP388 {
public:
    bool begin(uint8_t address = 0x76, TwoWire *wire = &Wire);
    void setDebug(bool enabled) { debug_ = enabled; }

    bool setTemperatureOversampling(uint8_t value);
    bool setPressureOversampling(uint8_t value);
    bool setOutputDataRate(uint8_t value);

    float readTemperature(); // degrees Celsius
    float readPressure();    // Pascals

private:
    struct Calibration {
        int32_t c0, c1;
        int32_t c00, c10, c20, c30, c40;
        int32_t c01, c11, c21, c31;
    } calib_{};

    TwoWire *wire_ = nullptr;
    uint8_t address_ = 0x76;
    uint8_t temperatureOs_ = BMP388_OS_2X;
    uint8_t pressureOs_ = BMP388_OS_16X;
    uint8_t rate_ = BMP388_ODR_16_HZ;
    bool initialized_ = false;
    bool debug_ = false;

    bool readRegisters(uint8_t reg, uint8_t *data, size_t length);
    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRaw(int32_t &pressure, int32_t &temperature);
    bool applyConfiguration();
    double temperatureScale() const;
    double pressureScale() const;
    double compensateTemperature(int32_t rawTemperature) const;
    double compensatePressure(int32_t rawPressure, int32_t rawTemperature) const;
};

#endif
