#include "tda7300.hpp"

TDA7300::TDA7300 ()
{
	TDA7300::I2C_ADDR = 0x0F;
}

void TDA7300::I2C_Init (void)
{
	TDA7300::hi2c2.Instance = I2C2;
	TDA7300::hi2c2.Init.ClockSpeed = 100000;
	TDA7300::hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	TDA7300::hi2c2.Init.OwnAddress1 = 0;
	TDA7300::hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	TDA7300::hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	TDA7300::hi2c2.Init.OwnAddress2 = 0;
	TDA7300::hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	TDA7300::hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		//Error_Handler();
	}
}

void TDA7300::I2C_Write (uint8_t address, uint8_t reg, uint8_t data)
{
	uint8_t dt[2];
	dt[0] = reg;
	dt[1] = data;
	HAL_I2C_Master_Transmit(&(TDA7300::hi2c2), address, dt, 2, 10);
}
