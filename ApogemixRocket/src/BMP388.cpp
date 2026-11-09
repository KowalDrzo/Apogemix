#include "BMP388.h"
#include <math.h>

namespace {
constexpr uint8_t REG_PRESSURE = 0x00;
constexpr uint8_t REG_PRESSURE_CONFIG = 0x06;
constexpr uint8_t REG_TEMPERATURE_CONFIG = 0x07;
constexpr uint8_t REG_MEAS_CONFIG = 0x08;
constexpr uint8_t REG_CONFIG = 0x09;
constexpr uint8_t REG_RESET = 0x0C;
constexpr uint8_t REG_ID = 0x0D;
constexpr uint8_t REG_COEFFICIENTS = 0x10;
constexpr uint8_t TOKMAS_PRODUCT_ID = 0x01;
constexpr uint8_t TOKMAS_SOFT_RESET = 0x09;
constexpr uint8_t TOKMAS_BACKGROUND_PT = 0x07;

int32_t signExtend(uint32_t value, uint8_t bits) {
    const uint32_t signBit = uint32_t(1) << (bits - 1);
    return int32_t((value ^ signBit) - signBit);
}

double scaleForOversampling(uint8_t oversampling) {
    // TOKMAS BMP388 datasheet, compensation scale-factor table.
    static constexpr double scales[] = {
        524288.0, 1572864.0, 3670016.0, 7864320.0,
        253952.0, 516096.0, 1040384.0, 2088960.0
    };
    return scales[oversampling & 0x07];
}
}

bool BMP388::readRegisters(uint8_t reg, uint8_t *data, size_t length) {
    if (!wire_ || !data || !length) {
        if (debug_) Serial.printf("TOKMAS BMP388: invalid read args (reg=0x%02X, len=%u)\n", reg, unsigned(length));
        return false;
    }

    wire_->beginTransmission(address_);
    wire_->write(reg);
    const uint8_t txStatus = wire_->endTransmission(false);
    if (txStatus != 0) {
        if (debug_) Serial.printf("TOKMAS BMP388: read setup failed reg=0x%02X, endTransmission=%u\n", reg, txStatus);
        return false;
    }

    const size_t received = wire_->requestFrom(address_, length);
    if (received != length) {
        if (debug_) Serial.printf("TOKMAS BMP388: short read reg=0x%02X, got=%u expected=%u\n",
                                  reg, unsigned(received), unsigned(length));
        while (wire_->available()) wire_->read();
        return false;
    }

    for (size_t i = 0; i < length; ++i) data[i] = uint8_t(wire_->read());
    return true;
}

bool BMP388::writeRegister(uint8_t reg, uint8_t value) {
    if (!wire_) {
        if (debug_) Serial.println("TOKMAS BMP388: write requested before Wire was set");
        return false;
    }
    wire_->beginTransmission(address_);
    wire_->write(reg);
    wire_->write(value);
    const uint8_t txStatus = wire_->endTransmission();
    if (txStatus != 0 && debug_)
        Serial.printf("TOKMAS BMP388: write failed reg=0x%02X value=0x%02X, endTransmission=%u\n",
                      reg, value, txStatus);
    return txStatus == 0;
}

bool BMP388::begin(uint8_t address, TwoWire *wire) {
    initialized_ = false;
    address_ = address;
    wire_ = wire;
    if (debug_) Serial.printf("TOKMAS BMP388: begin at I2C address 0x%02X\n", address_);
    if (!wire_) {
        if (debug_) Serial.println("TOKMAS BMP388: Wire pointer is null");
        return false;
    }

    uint8_t id = 0;
    if (!readRegisters(REG_ID, &id, 1)) {
        if (debug_) Serial.println("TOKMAS BMP388: ID read failed (ID register is 0x0D)");
        return false;
    }
    if (debug_) Serial.printf("TOKMAS BMP388: ID register 0x0D = 0x%02X\n", id);
    if ((id & 0x0F) != TOKMAS_PRODUCT_ID) {
        if (debug_) Serial.println("TOKMAS BMP388: unexpected product ID; expected low nibble 0x1");
        return false;
    }

    if (debug_) Serial.println("TOKMAS BMP388: sending soft reset at register 0x0C");
    if (!writeRegister(REG_RESET, TOKMAS_SOFT_RESET)) return false;
    delay(10);

    // Wait for the TOKMAS ASIC initialization and coefficient copy to finish.
    uint8_t measStatus = 0;
    bool ready = false;
    for (uint8_t i = 0; i < 100; ++i) {
        if (!readRegisters(REG_MEAS_CONFIG, &measStatus, 1)) {
            if (debug_) Serial.println("TOKMAS BMP388: failed to read MEAS_CFG after reset");
            return false;
        }
        if ((measStatus & 0xC0) == 0xC0) {
            ready = true;
            break;
        }
        delay(10);
    }
    if (debug_) Serial.printf("TOKMAS BMP388: MEAS_CFG after reset = 0x%02X\n", measStatus);
    if (!ready) {
        if (debug_) Serial.println("TOKMAS BMP388: sensor/coefficient initialization timed out");
        return false;
    }

    uint8_t c[21];
    if (!readRegisters(REG_COEFFICIENTS, c, sizeof(c))) {
        if (debug_) Serial.println("TOKMAS BMP388: coefficient read failed");
        return false;
    }

    const uint32_t c0u = (uint32_t(c[0]) << 4) | (c[1] >> 4);
    const uint32_t c1u = (uint32_t(c[1] & 0x0F) << 8) | c[2];
    const uint32_t c00u = (uint32_t(c[3]) << 12) | (uint32_t(c[4]) << 4) | (c[5] >> 4);
    const uint32_t c10u = (uint32_t(c[5] & 0x0F) << 16) | (uint32_t(c[6]) << 8) | c[7];

    calib_.c0 = signExtend(c0u, 12);
    calib_.c1 = signExtend(c1u, 12);
    calib_.c00 = signExtend(c00u, 20);
    calib_.c10 = signExtend(c10u, 20);
    calib_.c01 = signExtend((uint32_t(c[8]) << 8) | c[9], 16);
    calib_.c11 = signExtend((uint32_t(c[10]) << 8) | c[11], 16);
    calib_.c20 = signExtend((uint32_t(c[12]) << 8) | c[13], 16);
    calib_.c21 = signExtend((uint32_t(c[14]) << 8) | c[15], 16);
    calib_.c30 = signExtend((uint32_t(c[16]) << 8) | c[17], 16);
    const uint32_t c31u = (uint32_t(c[18]) << 4) | (c[19] >> 4);
    const uint32_t c40u = (uint32_t(c[19] & 0x0F) << 8) | c[20];
    calib_.c31 = signExtend(c31u, 12);
    calib_.c40 = signExtend(c40u, 12);

    if (debug_) Serial.printf("TOKMAS BMP388: coefficients loaded; c0=%ld c1=%ld c00=%ld c10=%ld\n",
                              long(calib_.c0), long(calib_.c1), long(calib_.c00), long(calib_.c10));

    initialized_ = true;
    if (!applyConfiguration()) {
        initialized_ = false;
        if (debug_) Serial.println("TOKMAS BMP388: configuration failed");
        return false;
    }
    if (debug_) Serial.println("TOKMAS BMP388: initialization successful");
    return true;
}

bool BMP388::applyConfiguration() {
    if (!writeRegister(REG_MEAS_CONFIG, 0x00)) {
        if (debug_) Serial.println("TOKMAS BMP388: failed to stop measurements");
        return false;
    }

    const uint8_t pressureConfig = uint8_t((rate_ << 4) | pressureOs_);
    const uint8_t temperatureConfig = uint8_t((rate_ << 4) | temperatureOs_);
    // TOKMAS requires data shifting when oversampling exceeds 8x.
    const uint8_t shiftConfig = uint8_t((temperatureOs_ > BMP388_OS_8X ? 0x08 : 0) |
                                        (pressureOs_ > BMP388_OS_8X ? 0x04 : 0));

    if (!writeRegister(REG_PRESSURE_CONFIG, pressureConfig) ||
        !writeRegister(REG_TEMPERATURE_CONFIG, temperatureConfig) ||
        !writeRegister(REG_CONFIG, shiftConfig)) {
        if (debug_) Serial.println("TOKMAS BMP388: failed to write measurement configuration");
        return false;
    }

    if (!writeRegister(REG_MEAS_CONFIG, TOKMAS_BACKGROUND_PT)) {
        if (debug_) Serial.println("TOKMAS BMP388: failed to start background pressure/temperature measurements");
        return false;
    }
    delay(30);
    return true;
}

bool BMP388::setTemperatureOversampling(uint8_t value) {
    if (value > BMP388_OS_128X) return false;
    temperatureOs_ = value;
    return !initialized_ || applyConfiguration();
}

bool BMP388::setPressureOversampling(uint8_t value) {
    if (value > BMP388_OS_128X) return false;
    pressureOs_ = value;
    return !initialized_ || applyConfiguration();
}

bool BMP388::setOutputDataRate(uint8_t value) {
    if (value > BMP388_ODR_128_HZ) return false;
    rate_ = value;
    return !initialized_ || applyConfiguration();
}

double BMP388::temperatureScale() const {
    return scaleForOversampling(temperatureOs_);
}

double BMP388::pressureScale() const {
    return scaleForOversampling(pressureOs_);
}

bool BMP388::readRaw(int32_t &pressure, int32_t &temperature) {
    uint8_t d[6];
    if (!initialized_) {
        if (debug_) Serial.println("TOKMAS BMP388: read requested before successful initialization");
        return false;
    }
    if (!readRegisters(REG_PRESSURE, d, sizeof(d))) {
        if (debug_) Serial.println("TOKMAS BMP388: raw pressure/temperature read failed");
        return false;
    }

    pressure = signExtend((uint32_t(d[0]) << 16) | (uint32_t(d[1]) << 8) | d[2], 24);
    temperature = signExtend((uint32_t(d[3]) << 16) | (uint32_t(d[4]) << 8) | d[5], 24);
    return true;
}

double BMP388::compensateTemperature(int32_t rawTemperature) const {
    const double scaledTemperature = double(rawTemperature) / temperatureScale();
    return double(calib_.c0) * 0.5 + double(calib_.c1) * scaledTemperature;
}

double BMP388::compensatePressure(int32_t rawPressure, int32_t rawTemperature) const {
    const double p = double(rawPressure) / pressureScale();
    const double t = double(rawTemperature) / temperatureScale();
    const double p2 = p * p;
    const double p3 = p2 * p;
    const double p4 = p3 * p;

    return double(calib_.c00) + double(calib_.c10) * p + double(calib_.c20) * p2 +
           double(calib_.c30) * p3 + double(calib_.c40) * p4 +
           t * (double(calib_.c01) + double(calib_.c11) * p +
                double(calib_.c21) * p2 + double(calib_.c31) * p3);
}

float BMP388::readTemperature() {
    int32_t rawPressure, rawTemperature;
    if (!readRaw(rawPressure, rawTemperature)) return NAN;
    return float(compensateTemperature(rawTemperature));
}

float BMP388::readPressure() {
    int32_t rawPressure, rawTemperature;
    if (!readRaw(rawPressure, rawTemperature)) return NAN;
    return float(compensatePressure(rawPressure, rawTemperature));
}
