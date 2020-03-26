#include "ssd1306.hpp"
#include <string>

OLED::OLED (void)
{
	I2C_ADDR = _I2C_ADDR;
	WIDTH = _WIDTH;
	HEIGHT = _HEIGHT;
	COLOR_BLACK = _COLOR_BLACK;
	COLOR_WHITE = _COLOR_WHITE;
	
	I2C_TIMEOUT = 20000;
	Buffer = Buffer_All + 1;
}

uint8_t OLED::Init (void)
{
	I2C_Init();
	if (HAL_I2C_IsDeviceReady(&hi2c1, I2C_ADDR, 1, 20000) != HAL_OK)
	{
		return 0;
	}

	/* A little delay */
	uint32_t p = 2500;
	while (p>0)
		p--;

	/* Init LCD */
	I2C_Write_Command (0xAE); //display off
	I2C_Write_Command (0x20); //Set Memory Addressing Mode   
	I2C_Write_Command (0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	I2C_Write_Command (0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	I2C_Write_Command (0xC8); //Set COM Output Scan Direction
	I2C_Write_Command (0x00); //---set low column address
	I2C_Write_Command (0x10); //---set high column address
	I2C_Write_Command (0x40); //--set start line address
	I2C_Write_Command (0x81); //--set contrast control register
	I2C_Write_Command (0xFF);
	I2C_Write_Command (0xA1); //--set segment re-map 0 to 127
	I2C_Write_Command (0xA6); //--set normal display
	I2C_Write_Command (0xA8); //--set multiplex ratio(1 to 64)
	I2C_Write_Command (0x3F); //
	I2C_Write_Command (0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	I2C_Write_Command (0xD3); //-set display offset
	I2C_Write_Command (0x00); //-not offset
	I2C_Write_Command (0xD5); //--set display clock divide ratio/oscillator frequency
	I2C_Write_Command (0xF0); //--set divide ratio
	I2C_Write_Command (0xD9); //--set pre-charge period
	I2C_Write_Command (0x22); //
	I2C_Write_Command (0xDA); //--set com pins hardware configuration
	I2C_Write_Command (0x12);
	I2C_Write_Command (0xDB); //--set vcomh
	I2C_Write_Command (0x20); //0x20,0.77xVcc
	I2C_Write_Command (0x8D); //--set DC-DC enable
	I2C_Write_Command (0x14); //
	I2C_Write_Command (0xAF); //--turn on SSD1306 panel
	
	Fill(COLOR_BLACK);
	UpdateScreen();

	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	SSD1306.Initialized = 1;
	return 1;
}

void OLED::UpdateScreen (void)
{
	Buffer_All[0] = 0x40;
	HAL_I2C_Master_Transmit_DMA(&hi2c1, I2C_ADDR, Buffer_All, WIDTH * HEIGHT / 8 + 1);
	while(HAL_DMA_GetState(hi2c1.hdmatx) != HAL_DMA_STATE_READY)
	{
		HAL_Delay(1); //Change for your RTOS
	}
}

void OLED::ToggleInvert (void)
{
	uint16_t i;
	SSD1306.Inverted = !SSD1306.Inverted;
	for (i = 0; i < sizeof (Buffer); i++)
	{
		Buffer[i] = ~Buffer[i];
	}
}

void OLED::Fill (uint8_t color)
{
	memset(Buffer, (color == COLOR_BLACK) ? 0x00 : 0xFF, WIDTH * HEIGHT / 8);
}

void OLED::DrawPixel (uint16_t x, uint16_t y, uint8_t color)
{
	if (x >= WIDTH || y >= HEIGHT)
	{
		return;
	}
	if (SSD1306.Inverted)
	{
		color = (uint8_t)!color;
	}
	if (color == OLED::COLOR_WHITE)
	{
		Buffer[x + (y / 8) * WIDTH] |= 1 << (y % 8);
	}
	else
	{
		Buffer[x + (y / 8) * WIDTH] &= ~(1 << (y % 8));
	}
}

void OLED::GotoXY (uint16_t x, uint16_t y)
{
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

char OLED::Putc (char ch, FontDef_t* Font, uint8_t color)
{
	uint32_t i, b, j;
	b = 0;
	for (i = 0; i < Font->FontHeight; i++)
	{
		for (j = 0; j < Font->FontWidth; j++)
		{
			if ((Font->data[ch*Font->CharBytes + b/8] >> b%8) & 1)
			{
				OLED::DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (uint8_t) color);
			}
			else
			{
				OLED::DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (uint8_t)!color);
			}
			b++;
		}
	}	
	SSD1306.CurrentX += Font->FontWidth;
	return ch;
}

char OLED::Puts (char* str, FontDef_t* Font, uint8_t color)
{
	while (*str)
	{
		if (Putc(*str, Font, color) != *str)
		{
			return *str;
		}		
		str++;
	}
	return *str;
}

void OLED::DrawLine (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t c)
{
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= WIDTH) {
		x0 = WIDTH - 1;
	}
	if (x1 >= WIDTH) {
		x1 = WIDTH - 1;
	}
	if (y0 >= HEIGHT) {
		y0 = HEIGHT - 1;
	}
	if (y1 >= HEIGHT) {
		y1 = HEIGHT - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			DrawPixel(x0, i, c);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			DrawPixel(i, y0, c);
		}
		
		/* Return from function */
		return;
	}
	
	while (1) {
		DrawPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err; 
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		} 
	}
}

void OLED::DrawRectangle (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t c)
{
	if (x >= WIDTH || y >= HEIGHT)
	{
		return;
	}
	/* Check width and height */
	if ((x + w) >= WIDTH)
	{
		w = WIDTH - x;
	}
	if ((y + h) >= HEIGHT)
	{
		h = HEIGHT - y;
	}
	/* Draw 4 lines */
	DrawLine(x, y, x + w, y, c);         /* Top line */
	DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	DrawLine(x, y, x, y + h, c);         /* Left line */
	DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}


void OLED::I2C_Init (void)
{
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		//Error_Handler();
	}
}

void OLED::ON(void)
{
	I2C_Write_Command(0x8D);  
	I2C_Write_Command(0x14);  
	I2C_Write_Command(0xAF);  
}
void OLED::OFF(void)
{
	I2C_Write_Command(0x8D);  
	I2C_Write_Command(0x10);
	I2C_Write_Command(0xAE);  
}

void OLED::I2C_Write (uint8_t address, uint8_t reg, uint8_t data)
{
	uint8_t dt[2];
	dt[0] = reg;
	dt[1] = data;
	HAL_I2C_Master_Transmit(&(hi2c1), address, dt, 2, 10);
}

void OLED::I2C_Write_Command (uint8_t command)
{
	I2C_Write(I2C_ADDR, 0x00, command);
}

void OLED::I2C_Write_Data (uint8_t data)
{
	I2C_Write(I2C_ADDR, 0x40, data);
}

void OLED::I2C_WriteMulti (uint8_t address, uint8_t reg, uint8_t *data, uint16_t count)
{
	//uint8_t dt[count + 1];
	uint8_t dt[8];
	dt[0] = reg;
	uint8_t i;
	for(i = 1; i <= count; i++)
		dt[i] = data[i-1];
	HAL_I2C_Master_Transmit(&(hi2c1), address, dt, count, 10);
}

void OLED::I2C_WriteMulti_DMA (uint8_t address, uint8_t reg, uint8_t* data, uint16_t count)
{
	HAL_I2C_Master_Transmit(&(hi2c1), address, &reg, 1, 100);
	HAL_I2C_Master_Transmit_DMA(&(hi2c1), address, data, count);
}

