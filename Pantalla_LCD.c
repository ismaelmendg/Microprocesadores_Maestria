#include <stdint.h>
#include "stm32f4xx_hal.h"

#define RCC_BASE_ADDR 0x40023800UL
#define RCC_AHB1ENR_OFFSET 0x30UL
#define RCC_AHB1ENR (RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET)

#define GPIOD_BASE_ADDR 0x40020C00UL
#define GPIOD_MODER_OFFSET 0x00UL
#define GPIOD_MODER_ADDR (GPIOD_BASE_ADDR + GPIOD_MODER_OFFSET)

#define GPIOD_ODR_OFFSET 0x14UL
#define GPIOD_ODR_ADDR (GPIOD_BASE_ADDR + GPIOD_ODR_OFFSET)

#define PIN_RS 3
#define SET_DDRAM_ADDR 0x80
#define CLEAR_DISPLAY 0x01
void lcd_write(uint8_t data);
void delay(uint32_t ms);

void GPIO(uint8_t pin, uint8_t on_off)
{
	uint32_t *pGPIOD_ODR = (uint32_t*) GPIOD_ODR_ADDR;

	if(on_off == 1)
	{
		*pGPIOD_ODR |= (0x1 << pin);
	}
	else
	{
		*pGPIOD_ODR &= ~(0x1 << pin);
	}
}

void pin_config(void)
{
	uint32_t *pRccAHB1ENR = (uint32_t*) RCC_AHB1ENR;
	uint32_t *pGPIOD_MODER = (uint32_t*) GPIOD_MODER_ADDR;

	*pRccAHB1ENR &= ~(0x1 << 1);
	*pRccAHB1ENR |= (1 << 3);


	*pGPIOD_MODER &= ~( 0x3 << 2);//Clear moder1 - LCD_E
	*pGPIOD_MODER &= ~( 0x3 << 6);//Clear moder3 - LCD_RS
	*pGPIOD_MODER &= ~( 0x3 << 8);//Clear moder4 - LCD_D4
	*pGPIOD_MODER &= ~( 0x3 << 10);//Clear moder5 - LCD_D5
	*pGPIOD_MODER &= ~( 0x3 << 12);//Clear moder6 - LCD_D6
	*pGPIOD_MODER &= ~( 0x3 << 14);//Clear moder7 - LCD_D7

	*pGPIOD_MODER |= (1 << 2);
	*pGPIOD_MODER |= (1 << 6);
	*pGPIOD_MODER |= (1 << 8);
	*pGPIOD_MODER |= (1 << 10);
	*pGPIOD_MODER |= (1 << 12);
	*pGPIOD_MODER |= (1 << 14);
}

void lcd_command(uint8_t command)
{
	GPIO(PIN_RS, 0);
	lcd_write(command);
	//lcd_write(command & 0x0F);
}

void lcd_char(uint8_t char_data)
{
	GPIO(PIN_RS, 1);
	lcd_write(char_data);
	//lcd_write(char_data & 0x0F);
}

void lcd_string(char *string) {
    while (*string != '\0') {
    	lcd_char(*string);
        string++;
    }
}

void lcd_String_xy (char row, char pos, char *str)  /* Send string to LCD function */
{
	if (row == 0)
	lcd_command((pos & 0x0F)|0x80);
	else if (row == 1)
	lcd_command((pos & 0x0F)|0xC0);
	lcd_string(str);	/* Call LCD string function */
}

void lcd_init()
{
	delay(20);
	lcd_command(0x02);
	lcd_command(0x28);
	lcd_command(0x0C);
	lcd_command(0x06);
	lcd_command(0x01);
	lcd_command(0x80);
}

void delay(uint32_t ms)
{

	HAL_Delay(ms);
}

void lcd_write(uint8_t data)
{
    uint8_t high_nibble = (data >> 4) & 0x0F;
    uint8_t low_nibble = data & 0x0F;

    GPIO(4, (high_nibble & 0x01) ? 1 : 0);
    GPIO(5, (high_nibble & 0x02) ? 1 : 0);
    GPIO(6, (high_nibble & 0x04) ? 1 : 0);
    GPIO(7, (high_nibble & 0x08) ? 1 : 0);

    GPIO(1, 1);
    delay(5);
    GPIO(1, 0);

    //delay(20);

    GPIO(4, (low_nibble & 0x01) ? 1 : 0);
    GPIO(5, (low_nibble & 0x02) ? 1 : 0);
    GPIO(6, (low_nibble & 0x04) ? 1 : 0);
    GPIO(7, (low_nibble & 0x08) ? 1 : 0);

    GPIO(1, 1);
    delay(5);
    GPIO(1, 0);
}

void lcd_cursor(uint8_t row, uint8_t col)
{
    uint8_t position = 0;

    if (row == 0) {
        position = col;
    } else if (row == 1) {
        position = col + 0x40;
    }

    lcd_command(SET_DDRAM_ADDR | position);
}

void lcd_clear(void)
{
	lcd_command(CLEAR_DISPLAY);
}
int main(void)
{
	HAL_Init();
    pin_config();
    lcd_init();

    lcd_clear();
	lcd_string("Ismael Mendoza");  /* write string on 1st line of LCD*/
	lcd_command(0xC0);
	lcd_string("Tarea pantalla");  /*write string on 2nd line*/

	while(1);
}

