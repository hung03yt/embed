#include "mma8452.h"
#include "i2c.h"
static uint8_t _deviceAddress;

static void writeRegister(uint8_t reg, uint8_t data) {
	i2c1_MemoryWrite_Byte(MMA8452Q_ADDR, reg, data);
//    HAL_I2C_Mem_Write(hi2c, _deviceAddress, reg, 1, &data, 1, HAL_MAX_DELAY);
}

static uint8_t readRegister(uint8_t reg) {
	uint8_t data;
	i2c1_readMemoryByte(MMA8452Q_ADDR, reg, &data);
//    HAL_I2C_Mem_Read(hi2c, _deviceAddress, reg, 1, &data, 1, HAL_MAX_DELAY);
	return data;
}

void MMA8452Q_Init() {
    uint8_t c = readRegister(WHO_AM_I);
    if (c != 0x2A) {
        return; // Không the ket noi MMA8452Q
    }

    MMA8452Q_setScale(SCALE_2G);
    MMA8452Q_setDataRate(ODR_800);
}

uint8_t MMA8452Q_readID() {
    return readRegister(WHO_AM_I);
}

void MMA8452Q_Read_Accel(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
		i2c1_readMemoryMulti(MMA8452Q_ADDR, OUT_X_MSB, data, 6);

//    HAL_I2C_Mem_Read(_deviceAddress, OUT_X_MSB, 1, data, 6, HAL_MAX_DELAY);

    *x = ((int16_t)(data[0] << 8 | data[1])) >> 4;
    *y = ((int16_t)(data[2] << 8 | data[3])) >> 4;
    *z = ((int16_t)(data[4] << 8 | data[5])) >> 4;
}

void MMA8452Q_setScale(MMA8452Q_Scale scale) {
    MMA8452Q_standby();

    uint8_t cfg = readRegister(XYZ_DATA_CFG);
    cfg &= 0xFC;
    cfg |= (scale >> 2);
    writeRegister( XYZ_DATA_CFG, cfg);

    MMA8452Q_active();
}

void MMA8452Q_setDataRate(MMA8452Q_ODR odr) {
    MMA8452Q_standby();

    uint8_t ctrl = readRegister(CTRL_REG1);
    ctrl &= 0xC7;
    ctrl |= (odr << 3);
    writeRegister(CTRL_REG1, ctrl);

    MMA8452Q_active();
}

void MMA8452Q_standby(I2C_HandleTypeDef *hi2c) {
    uint8_t c = readRegister(CTRL_REG1);
    writeRegister(CTRL_REG1, c & ~(0x01));
}

void MMA8452Q_active() {
    uint8_t c = readRegister(CTRL_REG1);
    writeRegister(CTRL_REG1, c | 0x01);
}

uint8_t MMA8452Q_isActive() {
    uint8_t currentState = readRegister(SYSMOD);
    currentState &= 0b00000011;

    if (currentState == SYSMOD_STANDBY) {
        return 0;
    }
    return 1;
}
