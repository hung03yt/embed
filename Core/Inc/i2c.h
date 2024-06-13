#ifndef I2C_H_
#define I2C_H_
#include "stdint.h"
#include "stdio.h"

void i2c_init();

void i2c1_scan_bus(void);

void i2c1_MemoryWrite_Byte(uint8_t saddr,uint8_t maddr, uint8_t data);

void i2c1_Write_Byte(uint8_t saddr, uint8_t data);

void i2c1_readMemoryByte(uint8_t saddr,uint8_t maddr, uint8_t *data);

void i2c1_readByte(uint8_t saddr, uint8_t *data);

void i2c1_readMemoryMulti(uint8_t saddr,uint8_t maddr, uint8_t *data, uint8_t length);

void i2c1_writeMemoryMulti(uint8_t saddr,uint8_t maddr, uint8_t *data, uint8_t length);

#endif /* I2C_H_ */
