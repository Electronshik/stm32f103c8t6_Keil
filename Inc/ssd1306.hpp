#ifndef SSD1306_H
#define SSD1306_H

extern "C"
{
	#include "stm32f1xx_hal.h"
	#include "fonts.h"
	#include "stdlib.h"
	#include "string.h"
}

const unsigned char _I2C_ADDR = 0x78;
//const unsigned char I2C_ADDR = 0x7A;
const unsigned char _WIDTH = 128;
const unsigned char _HEIGHT = 64;
const unsigned char _COLOR_BLACK = 0x00;
const unsigned char _COLOR_WHITE = 0x01;

/** Default pinout
SSD1306    |STM32F10x    |DESCRIPTION

VCC        |3.3V         |
GND        |GND          |
SCL        |PB6          |Serial clock line
SDA        |PB7          |Serial data line	*/

/* Private SSD1306 structure */
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

class OLED
{
	public:
		unsigned char I2C_ADDR;
		unsigned char WIDTH;
		unsigned char HEIGHT;
		unsigned char COLOR_BLACK;
		unsigned char COLOR_WHITE;

		OLED (void);
		// - > 0: LCD initialized OK and ready to use; - 0: LCD was not detected on I2C port
		uint8_t Init (void);
		// This function must be called each time you do some changes to LCD, to update buffer from RAM to LCD
		void UpdateScreen (void);
		// Toggles pixels invertion inside internal RAM; must be called after that in order to see updated LCD screen
		void ToggleInvert (void);
		// Fills entire LCD with desired color
		void Fill (uint8_t color);
		// x: X location. This parameter can be a value between 0 and WIDTH - 1
		// y: Y location. This parameter can be a value between 0 and HEIGHT - 1
		void DrawPixel (uint16_t, uint16_t, uint8_t);
		// Sets cursor pointer to desired location for strings
		void GotoXY (uint16_t, uint16_t);
		// Puts character to internal RAM
		char Putc (char, FontDef_t*, uint8_t);
		// Puts string to internal RAM
		char Puts (char*, FontDef_t*, uint8_t);
		// Draws line on LCD
		void DrawLine (uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
		// Draws rectangle on LCD
		void DrawRectangle (uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
		// c: Color to be used. This parameter can be a value of @ref COLOR_t enumeration
		void DrawFilledRectangle (uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
		// Draws triangle on LCD
		void DrawTriangle (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t color);
		// Draws circle to STM buffer
		void DrawCircle (int16_t x0, int16_t y0, int16_t r, uint8_t c);
		//  Draws filled circle to STM buffer
		void DrawFilledCircle (int16_t x0, int16_t y0, int16_t r, uint8_t c);
		// Writes multi bytes to slave
		void image (uint8_t *img, uint8_t frame, uint8_t x, uint8_t y);
	private:
		unsigned int I2C_TIMEOUT;
		I2C_HandleTypeDef hi2c1;
		SSD1306_t SSD1306;
		uint8_t Buffer_All[_WIDTH * _HEIGHT / 8 + 1];
		uint8_t *Buffer;

		void I2C_Init (void);
		void I2C_Write (uint8_t address, uint8_t reg, uint8_t data);
		void I2C_Write_Command (uint8_t command);
		void I2C_Write_Data (uint8_t data);
		void I2C_WriteMulti (uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);
		void I2C_WriteMulti_DMA (uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);
		void ON (void);
		void OFF (void);
		//Writes multi bytes to slave
		// *I2Cx: I2C used
		// address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
		// reg: register to write to
		// *data: pointer to data array to write it to slave
		// count: how many bytes will be written

};

#endif
