/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735.h"
#include "fonts.h"
#include "testimg.h"
#define ARRIBA 1
#define ABAJO 2
#define DERECHA 8
#define IZQUIERDA 4
#define START 1
#define BOTON_O 8
#define BOTON_X 16
#define ESTADO_INICIAL 0
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
uint8_t uart_byte_buf[15];
uint8_t PadIzq;
uint8_t PadDer;
uint8_t r = 2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART6_UART_Init(void);
int menu_opcion(uint8_t izquierda, uint8_t boton_o, uint8_t boton_x, int (*accion_adicional)());

/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	PadIzq = uart_byte_buf[6];
	PadDer = uart_byte_buf[5];

    HAL_UART_Receive_IT(&huart6, uart_byte_buf, 16);
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int posicion_lista(uint8_t reng_inf, uint8_t reng_sup, uint8_t pad1, uint8_t pad2, uint8_t posicion)
{

	if (PadIzq == pad1)
	{
		posicion = (posicion == reng_inf-1 || posicion == reng_inf) ? reng_inf : posicion - 1;
	    return posicion;
	}
	else if (PadIzq == pad2)
	{
		posicion = (posicion == reng_sup || posicion == reng_sup+1) ? reng_sup : posicion + 1;
	    return posicion;
	}
	else if(PadDer == BOTON_O || PadDer == BOTON_X)
	{
		PadDer = ESTADO_INICIAL;
		return 0;
	}

	PadIzq = ESTADO_INICIAL;
	return posicion;
}

int imagenes(uint8_t posicion)
{

	while(1)
	{
		if(PadIzq != ESTADO_INICIAL)
		{
			break;
		}
		else if(PadDer == BOTON_X)
		{
			while(1)
			{

				if (PadIzq == IZQUIERDA)
				{
					posicion = (posicion == 0 || posicion == 1) ? 5 : posicion - 1;
				}
				else if (PadIzq == DERECHA)
				{
					posicion = (posicion == 6 || posicion == 5) ? 1 : posicion + 1;
				}
				else if (PadDer == BOTON_O)
				{
					PadDer = ESTADO_INICIAL;
					break;
				}
				PadIzq = ESTADO_INICIAL;
				switch (posicion)
				{
					case 1:
						ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)desierto);
						break;
					case 2:
						ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)flor);
						break;
					case 3:
						ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)lago);
						break;
					case 4:
						ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)espacio);
						break;
					case 5:
						ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)tigre);
						break;
				}
			}
			break;
		}
	}
	return 0;
}

void contador_binario(void)
{
    uint8_t contador = 0;

    while (1)
    {

        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, (contador & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, (contador & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (contador & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, (contador & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

        HAL_Delay(500);

        contador++;

        if (contador > 15)
        {
        	contador = 0;
        }
        if(PadDer == BOTON_X)
		{
        	PadDer = ESTADO_INICIAL;
			break;
		}
    }
}


void led_on_off(uint8_t on_off)
{
	if(on_off == 1)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
	}

}

int menu_led(void)
{
	uint8_t posicion = 1;

	while(1)
	{
		if (PadIzq == ARRIBA)
		{
			posicion = (posicion == 0 || posicion == 1) ? 1 : posicion - 1;
		}
		else if (PadIzq == ABAJO)
		{
			posicion = (posicion == 3 || posicion == 2) ? 2 : posicion + 1;
		}
		else if (PadDer == BOTON_O)
		{
			PadDer = ESTADO_INICIAL;
			break;
		}
		PadIzq = ESTADO_INICIAL;
		switch (posicion)
		{
			case 1:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)two_led);
				led_on_off(0);
				while(1)
				{
					if(PadIzq != ESTADO_INICIAL)
					{
						break;
					}
					else if(PadDer == BOTON_O)
					{
						break;
					}
					else if(PadDer == BOTON_X)
					{
						PadDer = ESTADO_INICIAL;
						while(1)
						{
							if(PadDer == BOTON_X)
							{
								PadDer = ESTADO_INICIAL;
								break;
							}
							ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)led_on);
							led_on_off(1);
						}
						break;
					}
				}
				break;
			case 2:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)two_counter);
				while(1)
				{
					if(PadIzq != ESTADO_INICIAL)
					{
						break;
					}
					else if(PadDer == BOTON_O)
					{
						break;
					}
					else if(PadDer == BOTON_X)
					{
						PadDer = ESTADO_INICIAL;
						while(1)
						{

							if(PadDer == BOTON_X)
							{
								PadDer = ESTADO_INICIAL;
								break;
							}
							ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)counter_on);
							contador_binario();
							led_on_off(0);
							break;
						}
						break;
					}
				}
				break;
			case 3:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)menu3);
				while(1)
				{
					if(PadIzq != ESTADO_INICIAL)
					{
						break;
					}
					else if(PadDer == BOTON_O)
					{
						break;
					}
					else if(PadDer == BOTON_X)
					{
						break;
					}
				}
				break;
		}
	}
	return 0;
}


int menu_imagenes(void)
{
	uint8_t posicion = 1;
	uint8_t posicion_ant = posicion;

	while(1)
	{
		posicion = posicion_lista(1, 6, 1, 2,posicion_ant);

		if(posicion != posicion_ant)
		{
			posicion_ant = posicion;
			PadIzq = ESTADO_INICIAL;
		}

		if(posicion==0) break;


		switch (posicion)
		{
			case 1:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)mi1);
				imagenes(1);
				break;
			case 2:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)mi2);
				imagenes(2);
				break;
			case 3:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)mi3);
				imagenes(3);
				break;
			case 4:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)mi4);
				imagenes(4);
				break;
			case 5:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)mi5);
				imagenes(5);
				break;
			case 6:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)mi6);
				while(1)
				{
					if(PadIzq != ESTADO_INICIAL)
					{
						break;
					}
					else if(PadDer == BOTON_O)
					{
						break;
					}
					else if(PadDer == BOTON_X)
					{
						break;
					}
				}
				break;
		}
	}
	return 0;
}


int menu(void)
{

	uint8_t posicion = 1;
	uint8_t posicion_ant = posicion;

	while(1)
	{

		posicion = posicion_lista(1, 3, 1, 2,posicion_ant);

		if(posicion != posicion_ant)
		{
			posicion_ant = posicion;
			PadIzq = ESTADO_INICIAL;
		}

		if(posicion==0) break;

		switch (posicion)
		{
			case 1:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)menu1);
				while(1)
				{
					if(PadIzq != ESTADO_INICIAL)
					{
						break;
					}
					else if(PadDer == BOTON_O)
					{
						break;
					}
					else if(PadDer == BOTON_X)
					{
						PadDer = ESTADO_INICIAL;
						menu_imagenes();
						break;
					}
				}
				break;
			case 2:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)menu2);
				while(1)
				{
					if(PadIzq != ESTADO_INICIAL)
					{
						break;
					}
					else if(PadDer == BOTON_O)
					{
						break;
					}
					else if(PadDer == BOTON_X)
					{
						PadDer = ESTADO_INICIAL;
						menu_led();
						break;
					}
				}
				break;
			case 3:
				ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)menu3);
				while(1)
				{
					if(PadIzq != ESTADO_INICIAL)
					{
						break;
					}
					else if(PadDer == BOTON_O)
					{
						break;
					}
					else if(PadDer == BOTON_X)
					{

						break;
					}
				}
				break;
		}
	}
	return 0;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  ST7735_Init();
  ST7735_Backlight_On();
  HAL_UART_Receive_IT(&huart6, uart_byte_buf, 16);
  ST7735_SetRotation(r);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ST7735_DrawImage(0, 0, 128, 160, (uint16_t*)portada);

	  if(PadDer == START)
	  {
		  PadDer = 0;
		  while(1)
		  {
			  menu();
			  break;
		  }
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ST7735_BL_GPIO_Port, ST7735_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|ST7735_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ST7735_CS_Pin|ST7735_RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ST7735_BL_Pin */
  GPIO_InitStruct.Pin = ST7735_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ST7735_BL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 ST7735_DC_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|ST7735_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ST7735_CS_Pin ST7735_RES_Pin */
  GPIO_InitStruct.Pin = ST7735_CS_Pin|ST7735_RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
