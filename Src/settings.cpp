#include "settings.hpp"
#include "stdio.h"

#define I2C_EEPROM_ADDRESS			0x50
#define I2C_EEPROM_PAGE_SIZE		64
#define	I2C_EEPROM_TIMEOUT			10
#define	I2c_Eeprom_Write_DELAY		10
#define	MEMORY_ADDRESS				0x00

SETTINGS::SETTINGS (I2C_HandleTypeDef *hi2c2)
{
	hi2c2 = hi2c2;
	BackUp_Address = ((sizeof(Settings) / 64) + 1) * 64 + MEMORY_ADDRESS * 2;
}

bool SETTINGS::Get_Setting (Settings_List_Bool_t Setting)
{

}

unsigned char SETTINGS::Get_Setting (Settings_List_UChar_t Setting)
{

}

char SETTINGS::Get_Setting (Settings_List_Char_t Setting)
{

}

unsigned int SETTINGS::Get_Setting (Settings_List_UInt_t Setting)
{

}

int SETTINGS::Get_Setting (Settings_List_Int_t Setting)
{

}

float SETTINGS::Get_Setting (Settings_List_Float_t Setting)
{

}

void SETTINGS::Set_Setting (Settings_List_Bool_t Setting, bool Value)
{
	
}

void SETTINGS::Set_Setting (Settings_List_UChar_t Setting, unsigned char Value)
{
	
}

void SETTINGS::Set_Setting (Settings_List_Char_t Setting, char Value)
{
	
}

void SETTINGS::Set_Setting (Settings_List_UInt_t Setting, unsigned int Value)
{
	
}

void SETTINGS::Set_Setting (Settings_List_Int_t Setting, int Value)
{
	
}

void SETTINGS::Set_Setting (Settings_List_Float_t Setting, float Value)
{
	
}

void SETTINGS::Save (void)
{
	if ( HAL_OK != HAL_I2C_Mem_Read (hi2c2, (uint16_t)I2C_EEPROM_ADDRESS << 1, MEMORY_ADDRESS, I2C_MEMADD_SIZE_16BIT, Writing_Tag, 1, I2C_EEPROM_TIMEOUT) )
	{
		printf ("Can't read settings from EEPROM! \n\r");
		//Error_Handle(EEPROM_ERR, READ_ERROR);
	}
	Settings.Writing_Tag_Begin = ++Writing_Tag[0];
	Settings.Writing_Tag_End = Settings.Writing_Tag_Begin;
	if (I2c_Eeprom_Write(MEMORY_ADDRESS, (uint8_t *)&Settings, sizeof(Settings)))
	{
			HAL_I2C_Mem_Read (hi2c2, (uint16_t)I2C_EEPROM_ADDRESS << 1, MEMORY_ADDRESS, I2C_MEMADD_SIZE_16BIT, Writing_Tag, 1, I2C_EEPROM_TIMEOUT);
			Writing_Labels[0] = Writing_Tag[0];
			HAL_I2C_Mem_Read (hi2c2, (uint16_t)I2C_EEPROM_ADDRESS << 1, MEMORY_ADDRESS + sizeof(Settings) - 1, I2C_MEMADD_SIZE_16BIT, Writing_Tag, 1, I2C_EEPROM_TIMEOUT);
			Writing_Labels[1] = Writing_Tag[0];
		if (Writing_Labels[0] == Writing_Labels[1])
		{
			if (I2c_Eeprom_Write (BackUp_Address, (uint8_t *)&Settings, sizeof(Settings)))
			{
				HAL_I2C_Mem_Read (hi2c2, (uint16_t)I2C_EEPROM_ADDRESS << 1, MEMORY_ADDRESS, I2C_MEMADD_SIZE_16BIT, Writing_Tag, 1, I2C_EEPROM_TIMEOUT);
				Writing_Labels[0] = Writing_Tag[0];
				HAL_I2C_Mem_Read (hi2c2, (uint16_t)I2C_EEPROM_ADDRESS << 1, MEMORY_ADDRESS + sizeof(Settings) - 1, I2C_MEMADD_SIZE_16BIT, Writing_Tag, 1, I2C_EEPROM_TIMEOUT);
				Writing_Labels[1] = Writing_Tag[0];
				HAL_I2C_Mem_Read (hi2c2, (uint16_t)I2C_EEPROM_ADDRESS << 1, BackUp_Address, I2C_MEMADD_SIZE_16BIT, Writing_Tag, 1, I2C_EEPROM_TIMEOUT);
				Writing_Labels[2] = Writing_Tag[0];
				HAL_I2C_Mem_Read (hi2c2, (uint16_t)I2C_EEPROM_ADDRESS << 1, BackUp_Address + sizeof(Settings) - 1, I2C_MEMADD_SIZE_16BIT, Writing_Tag, 1, I2C_EEPROM_TIMEOUT);
				Writing_Labels[3] = Writing_Tag[0];
				if (Writing_Labels[0] == Writing_Labels[1] && Writing_Labels[1] == Writing_Labels[2] && Writing_Labels[2] == Writing_Labels[3])
				{
					printf ("Settings was successfully written to EEPROM \n\r");
				}
				else
				{
					printf ("Can't write settings to EEPROM! \n\r");
					//Error_Handle(EEPROM_ERR, WRITE_ERROR);
					//return -1;
				}
			}
		}
		else
		{
			printf ("Can't write settings to EEPROM! \n\r");
			//Error_Handle(EEPROM_ERR, WRITE_ERROR);
			//return -1;
		}
	}
}

int SETTINGS::I2c_Eeprom_Write (uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
  uint8_t Data[I2C_EEPROM_PAGE_SIZE];
  uint16_t i = 0;

  while (i < Size)
  {
		uint16_t j = 0;
		while ((j < I2C_EEPROM_PAGE_SIZE) && (i < Size))
		{
			Data[j] = pData[i];
			i++;
			j++;
		}
		if ( HAL_OK != HAL_I2C_Mem_Write(hi2c2, (uint16_t)I2C_EEPROM_ADDRESS << 1, MemAddress, I2C_MEMADD_SIZE_16BIT, Data, j, I2C_EEPROM_TIMEOUT) )
		{
			printf ("Can't write settings to EEPROM! \n\r");
			//Error_Handle(EEPROM_ERR, WRITE_ERROR);
			return -1;
		}
		MemAddress += I2C_EEPROM_PAGE_SIZE;
		HAL_Delay (I2c_Eeprom_Write_DELAY);
  }
	return 1;
}

int SETTINGS::I2c_Eeprom_Read (uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
  uint8_t Data[I2C_EEPROM_PAGE_SIZE];
  uint16_t i = 0;

  while(i < Size)
  {
		uint16_t j = I2C_EEPROM_PAGE_SIZE;
		if ( HAL_OK != HAL_I2C_Mem_Read (hi2c2, (uint16_t)I2C_EEPROM_ADDRESS << 1, MemAddress, I2C_MEMADD_SIZE_16BIT, Data, j, I2C_EEPROM_TIMEOUT))
		{
			printf ("Can't read settings from EEPROM! \n\r");
			//Error_Handle (EEPROM_ERR, READ_ERROR);
			return -1;
		}
		j = 0;
		while((j < I2C_EEPROM_PAGE_SIZE) && (i < Size))
		{
			pData[i] = Data[j];
			i++;
			j++;
		}
		MemAddress += I2C_EEPROM_PAGE_SIZE;
		HAL_Delay(I2c_Eeprom_Write_DELAY);
  }
	return 1;
}
