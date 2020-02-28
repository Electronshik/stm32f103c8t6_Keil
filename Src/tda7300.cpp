#include "tda7300.hpp"

TDA7300::TDA7300 (I2C_HandleTypeDef *hi2c2)
{
	I2C_ADDR = 0x44;
	hi2c2 = hi2c2;
	Volume.Data = VOLUME_MASK;
	Speaker_LF.Data = SPEAKER_ATT_LF_MASK;
	Speaker_RF.Data = SPEAKER_ATT_RF_MASK;
	Speaker_LR.Data = SPEAKER_ATT_LR_MASK;
	Speaker_RR.Data = SPEAKER_ATT_RR_MASK;
	Audio_Switch.Data = AUDIO_SWITCH_MASK;
	Bass_Control.Data = BASS_CONTROL_MASK;
	Treble_Control.Data = TREBLE_CONTROL_MASK;
}

//~	-68dB..10dB; 2dB step; 0..35 = value
void TDA7300::Set_Volume(uint8_t value)
{
	if (value <= 35)
	{
		value = 70 - value*2; //70..0
		Volume.Bits.High_Bits = (Volume_10_t)(value/10 + 1);
		Volume.Bits.Low_Bits = (Volume_0_t)((value % 10) / 2);
		I2C_Write(Volume.Data);
	}
}

void TDA7300::Set_Audio_Switch(Audio_Switch_t input)
{
	Audio_Switch.Bits.Low_Bits = input;
	I2C_Write(Audio_Switch.Data);
}
void TDA7300::I2C_Write (uint8_t value)
{
	HAL_I2C_Master_Transmit (hi2c2, I2C_ADDR, &value, 1, 10);
}
