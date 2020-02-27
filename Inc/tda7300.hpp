#ifndef TDA7300_H
#define TDA7300_H

extern "C"
{
	#include "stm32f1xx_hal.h"
}

typedef enum
{
	// X = dont care; Ax = 2dB steps; Bx = 10dB steps; Cx = 2.5dB steps
	VOLUME_MASK = 0x00,				// 0  0 B2  B1  B0  A2  A1  A0
	SPEAKER_ATT_LR_MASK = 0xc0,		// 1  1  0  B1  B0  A2  A1  A0
	SPEAKER_ATT_RR_MASK = 0xe0,		// 1  1  1  B1  B0  A2  A1  A0
	SPEAKER_ATT_LF_MASK = 0x80,		// 1  0  0  B1  B0  A2  A1  A0
	SPEAKER_ATT_RF_MASK = 0xa0,		// 1  0  1  B1  B0  A2  A1  A0
	AUDIO_SWITCH_MASK = 0x40,	// 0  1  0   X   X  S2  S1  S0
	BASS_CONTROL_MASK = 0x60,	// 0  1  1   0  C3  C2  C1  C0
	TREBLE_CONTROL_MASK = 0x70,	// 0  1  1   1  C3  C2  C1  C0

} Data_Masks_t;

//~/~	Volume Control	~/~// 0  0 B2  B1  B0  A2  A1  A0
typedef enum
{
	VOL_0_0 = 0,
	VOL_MINUS_2 = 1,
	VOL_MINUS_4 = 2,
	VOL_MINUS_6 = 3,
	VOL_MINUS_8 = 4

} Volume_0_t;

typedef enum
{
	VOL_PLUS_10 = 0,
	VOL_10_0 = 1,
	VOL_MINUS_10 = 2,
	VOL_MINUS_20 = 3,
	VOL_MINUS_30 = 4,
	VOL_MINUS_40 = 5,
	VOL_MINUS_50 = 6,
	VOL_MINUS_60 = 7,

} Volume_10_t;

union Reg_Volume_t
{
	struct
	{
		Volume_0_t Low_Bits: 3;
		Volume_10_t High_Bits: 3;
	} Bits;
	unsigned char Data;
};

//~/~	Speaker Control	~/~// qn qn qn B1  B0  A2  A1  A0
typedef enum
{
	SPK_0_0 = 0,
	SPK_MINUS_2 = 1,
	SPK_MINUS_4 = 2,
	SPK_MINUS_6 = 3,
	SPK_MINUS_8 = 4

} Speaker_0_t;

typedef enum
{
	SPK_10_0 = 0,
	SPK_MINUS_10 = 1,
	SPK_MINUS_20 = 2,
	SPK_MINUS_30 = 3,

} Speaker_10_t;

union Reg_Speaker_t
{
	struct
	{
		Speaker_0_t Low_Bits: 3;
		Speaker_10_t High_Bits: 2;
	} Bits;
	unsigned char Data;
};

//~/~	Audio Input Control	~/~// 0  1  0   X   X  S2  S1  S0
typedef enum
{
	STEREO_1_IN = 0,
	STEREO_2_IN = 1,
	STEREO_3_IN = 2,
	STEREO_4_IN = 3,
	MONO_IN = 4

} Audio_Switch_t;

union Reg_Audio_Switch_t
{
	struct
	{
		Audio_Switch_t Low_Bits: 3;
	} Bits;
	unsigned char Data;
};

//~/~	Bass and Treble Control	~/~// qn 1  1   0  C3  C2  C1  C0
typedef enum
{
	BASS_TRE_MINUS_15 = 1,
	BASS_TRE_MINUS_12_5 = 2,
	BASS_TRE_MINUS_10 = 3,
	BASS_TRE_MINUS_7_5 = 4,
	BASS_TRE_MINUS_5 = 5,
	BASS_TRE_MINUS_2_5 = 6,
	BASS_TRE_0 = 7,
	BASS_TRE_PLUS_2_5 = 14,
	BASS_TRE_PLUS_5 = 13,
	BASS_TRE_PLUS_7_5 = 12,
	BASS_TRE_PLUS_10 = 11,
	BASS_TRE_PLUS_12_5 = 10,
	BASS_TRE_PLUS_15 = 9,

} Bass_Treble_t;

union Reg_Bass_Control_t
{
	struct
	{
		Bass_Treble_t Low_Bits: 4;
	} Bits;
	unsigned char Data;
};

union Reg_Treble_Control_t
{
	struct
	{
		Bass_Treble_t Low_Bits: 4;
	} Bits;
	unsigned char Data;
};

//~/~	class TDA7300	~/~//
class TDA7300
{
	public:
		TDA7300();
		void Set_Volume (uint8_t value);
		void Set_Audio_Switch (Audio_Switch_t input);
	private:
		unsigned char I2C_ADDR;
		Reg_Volume_t Volume;
		Reg_Speaker_t Speaker_LF;
		Reg_Speaker_t Speaker_RF;
		Reg_Speaker_t Speaker_LR;
		Reg_Speaker_t Speaker_RR;
		Reg_Audio_Switch_t Audio_Switch;
		Reg_Bass_Control_t Bass_Control;
		Reg_Treble_Control_t Treble_Control;
		I2C_HandleTypeDef hi2c2;
		void I2C_Init();
		void I2C_Write (uint8_t value);
};

#endif

