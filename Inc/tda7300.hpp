#ifndef TDA7300_H
#define TDA7300_H

extern "C"
{
	#include "stm32f1xx_hal.h"
}

class TDA7300
{
	public:
		TDA7300();
	private:
		unsigned char I2C_ADDR;
		I2C_HandleTypeDef hi2c2;
		void I2C_Init();
		void I2C_Write (uint8_t address, uint8_t reg, uint8_t data);
};

#endif
