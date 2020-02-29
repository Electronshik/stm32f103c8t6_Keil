#pragma once
#ifndef __SETTINGS_H__
#define __SETTINGS_H__

extern "C"
{
	#include "stm32f1xx_hal.h"
}

typedef enum
{
	VOLUME,
	SPEAKER_LF,
	SPEAKER_RF,
	SPEAKER_LR,
	SPEAKER_RR,
	AUDIO_SWITCH,
	BASS_CONTROL,
	TREBLE_CONTROL,
} Settings_List_t;

//~ Main settings ~//
#pragma pack (push, 1)

typedef struct
{
	unsigned char Writing_Tag_Begin;
	int Data[8];
	unsigned char Writing_Tag_End;

} Settings_t;

#pragma pack (pop)
//~ Main settings ~//

class SETTINGS
{
	public:
		SETTINGS (I2C_HandleTypeDef *);
		void Init (bool Reset_To_Default);
		void Save ();
		template <typename T>
		T Get_Setting (Settings_List_t Setting);
		template <typename T>
		void Set_Setting (Settings_List_t Setting, T Value);
		/*
		void Set_Setting (Settings_List_UChar_t Setting, unsigned char Value);
		void Set_Setting (Settings_List_Char_t Setting, char Value);
		void Set_Setting (Settings_List_UInt_t Setting, unsigned int Value);
		void Set_Setting (Settings_List_Int_t Setting, int Value);
		void Set_Setting (Settings_List_Float_t Setting, float Value);*/
	private:
		Settings_t Settings;
		unsigned int BackUp_Address;
		unsigned char Writing_Labels[4];
		unsigned char Writing_Tag[1];
		I2C_HandleTypeDef *hi2c2;
		int I2c_Eeprom_Write (uint16_t MemAddress, uint8_t *pData, uint16_t Size);
		int I2c_Eeprom_Read (uint16_t MemAddress, uint8_t *pData, uint16_t Size);
};

#endif
