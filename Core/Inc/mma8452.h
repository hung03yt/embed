#ifndef MMA8452Q_H
#define MMA8452Q_H

#include "stm32f1xx_hal.h"

#define MMA8452Q_ADDR 0x1C //<< 1 // Ð?a ch? I2C c?a MMA8452Q

#define WHO_AM_I 0x0D
#define CTRL_REG1 0x2A
#define XYZ_DATA_CFG 0x0E
#define OUT_X_MSB 0x01
#define OUT_Y_MSB 0x03
#define OUT_Z_MSB 0x05
#define STATUS_MMA8452Q 0x00
#define PULSE_CFG 0x21
#define PULSE_THSX 0x23
#define PULSE_THSY 0x24
#define PULSE_THSZ 0x25
#define PULSE_TMLT 0x26
#define PULSE_LTCY 0x27
#define PULSE_WIND 0x28
#define PL_CFG 0x11
#define PL_COUNT 0x12
#define PL_STATUS 0x10
#define SYSMOD 0x0B
#define SYSMOD_STANDBY 0x00

typedef enum {
    SCALE_2G = 2,
    SCALE_4G = 4,
    SCALE_8G = 8
} MMA8452Q_Scale;

typedef enum {
    ODR_800 = 0,
    ODR_400,
    ODR_200,
    ODR_100,
    ODR_50,
    ODR_12,
    ODR_6,
    ODR_1
} MMA8452Q_ODR;

void MMA8452Q_Init(
	);
uint8_t MMA8452Q_readID();
void MMA8452Q_Read_Accel(int16_t *x, int16_t *y, int16_t *z);
void MMA8452Q_setScale(MMA8452Q_Scale scale);
void MMA8452Q_setDataRate(MMA8452Q_ODR odr);
void MMA8452Q_standby();
void MMA8452Q_active();
uint8_t MMA8452Q_isActive();

#endif // MMA8452Q_H
