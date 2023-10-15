#include <stdint.h>

#define RCC_BASE_ADDR 0x40023800UL
#define RCC_AHB1ENR_OFFSET 0x30UL
#define RCC_AHB1ENR (RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET)

#define GPIOB_BASE_ADDR 0x40020400UL
#define GPIOB_MODER_OFFSET 0x00UL
#define GPIOB_MODER_ADDR (GPIOB_BASE_ADDR + GPIOB_MODER_OFFSET)

#define GPIOB_ODR_OFFSET 0x14UL
#define GPIOB_ODR_ADDR (GPIOB_BASE_ADDR + GPIOB_ODR_OFFSET)

#define GPIOC_BASE_ADDR 0x40020800UL
#define GPIOC_MODER_ADDR (GPIOC_BASE_ADDR + GPIOB_MODER_OFFSET)

#define GPIOC_IDR_OFFSET 0x10UL
#define GPIOC_IDR_ADDR (GPIOC_BASE_ADDR + GPIOC_IDR_OFFSET)

void led(int no_led, int on_off)
{
	uint32_t *pGPIOB_ODR = (uint32_t*) GPIOB_ODR_ADDR;

	if(no_led == 1)
	{
		if(on_off == 1)
		{
			*pGPIOB_ODR |= (0x1 << 1);
		}
		else
		{
			*pGPIOB_ODR &= ~(0x1 << 1);
		}
	}
	else if(no_led == 2)
	{
		if(on_off == 1)
		{
			*pGPIOB_ODR |= (0x1 << 3);
		}
		else
		{
			*pGPIOB_ODR &= ~(0x1 << 3);
		}
	}
	else if(no_led == 3)
	{
		if(on_off == 1)
		{
			*pGPIOB_ODR |= (0x1 << 4);
		}
		else
		{
			*pGPIOB_ODR &= ~(0x1 << 4);
		}
	}
	else if(no_led == 4)
	{
		if(on_off == 1)
		{
			*pGPIOB_ODR |= (0x1 << 5);
		}
		else
		{
			*pGPIOB_ODR &= ~(0x1 << 5);
		}
	}
}

void decimalToBinary(int decimal) {
    int binary[4]; // Un arreglo de 4 bits para almacenar la representación binaria
    int i;

    // Convertir decimal a binario
    for (i = 3; i >= 0; i--) {
        binary[i] = decimal % 2;
        decimal /= 2;
    }

    // Imprimir el valor binario
    for (i = 0; i < 4; i++) {
        printf("%d", binary[i]);
    }
    printf("\n");
}


int main(void)
{
	uint32_t *pRccAHB1ENR = (uint32_t*) RCC_AHB1ENR;
	uint32_t *pGPIOB_MODER = (uint32_t*) GPIOB_MODER_ADDR;
	//uint32_t *pGPIOB_ODR = (uint32_t*) GPIOB_ODR_ADDR;

	*pRccAHB1ENR &= ~(0x1 << 1);
	*pRccAHB1ENR |= (1 << 1);


	*pGPIOB_MODER &= ~( 0x3 << 2);//Clear moder1
	*pGPIOB_MODER &= ~( 0x3 << 6);//Clear moder3
	*pGPIOB_MODER &= ~( 0x3 << 8);//Clear moder4
	*pGPIOB_MODER &= ~( 0x3 << 10);//Clear moder5

	*pGPIOB_MODER |= (1 << 2);
	*pGPIOB_MODER |= (1 << 6);
	*pGPIOB_MODER |= (1 << 8);
	*pGPIOB_MODER |= (1 << 10);



    // Habilitar el reloj para el puerto GPIOC
    volatile uint32_t *pRCC_AHB1ENR_GPIOC = (uint32_t*)(0x40023830);
    *pRCC_AHB1ENR_GPIOC |= (1 << 2);

    // Configurar el pin PC13 como entrada (puedes usar 00 para entrada)
    volatile uint32_t *pGPIOC_MODER = (uint32_t*)GPIOC_MODER_ADDR;
    *pGPIOC_MODER &= ~(0x3 << 26);
    volatile uint32_t *pGPIOC_IDR = (uint32_t*) GPIOC_IDR_ADDR;


    int decimal = 0;
    int buttonPressed = 0; // Para verificar si el botón está presionado
    int buttonState = 0;
    int lastButtonState = 0;   // Estado previo del botón

      while (1)
      {
          // Leer el estado actual del botón
          buttonState = (*pGPIOC_IDR & (1 << 13)) ? 1 : 0;

          // Detectar un cambio en el estado del botón
          if (buttonState != lastButtonState)
          {
              // Esperar a que el botón se suelte completamente
              if (buttonState == 0)
              {
                  // Incrementar el contador solo cuando se suelta el botón
                  decimal++;
                  if (decimal > 15)
                  {
                      decimal = 0;
                  }

                  // Convierte el decimal a binario y muestra el resultado
                  decimalToBinary(decimal);

                  // Enciende los LED correspondientes
                  led(1, (decimal & 0x8) ? 1 : 0);
                  led(2, (decimal & 0x4) ? 1 : 0);
                  led(3, (decimal & 0x2) ? 1 : 0);
                  led(4, (decimal & 0x1) ? 1 : 0);
              }
          }

          // Actualizar el estado previo del botón
          lastButtonState = buttonState;
      }
}
