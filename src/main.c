/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "diag/Trace.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__ALIGN_BEGIN USBD_HandleTypeDef hUSBDDevice __ALIGN_END;

/* Private function prototypes -----------------------------------------------*/
void configure_system_clock(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	/* STM32F4xx HAL library initialization:
	 - Configure the Flash prefetch, instruction and Data caches
	 - Configure the Systick to generate an interrupt each 1 msec
	 - Set NVIC Group Priority to 4
	 - Global MSP (MCU Support Package) initialization
	 */

	SystemInit();

	// Enable HSE Oscillator and activate PLL with HSE as source
	configure_system_clock();
	SystemCoreClockUpdate();

	// Initialise the HAL Library; it must be the first
	// instruction to be executed in the main program.
	HAL_Init();


	/* Configure LED1, LED2, LED3 and LED4 */
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Toggle(LED3);
	BSP_LED_Toggle(LED4);
	/* Init Device Library */
	USBD_Init(&hUSBDDevice, &VCP_Desc, 1);

	/* Add Supported Class */
	USBD_RegisterClass(&hUSBDDevice, &USBD_CDC);

	/* Add CDC Interface Class */
	USBD_CDC_RegisterInterface(&hUSBDDevice, &USBD_CDC_fops);

	/* Start Device Process */
	USBD_Start(&hUSBDDevice);

	while(ComPortClosed);
	printf("Hello ARM World!");

	// At this stage the system clock should have already been configured
	// at high speed.
	printf("System clock: %d Hz \r\n", SystemCoreClock);

	/* Run Application (Interrupt mode) */
	while (1)
	{
		//HAL_Delay(1000);
		BSP_LED_Toggle(LED3);
		BSP_LED_Toggle(LED4);
		int val = 0;
		printf("Enter a number: ");
		scanf("%d", &val);
		printf("%d = 0x%x\r\n", val, val);

	}
}

void configure_system_clock(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	// Enable Power Control clock
	__PWR_CLK_ENABLE();

	// The voltage scaling allows optimizing the power consumption when the
	// device is clocked below the maximum system frequency, to update the
	// voltage scaling value regarding system frequency refer to product
	// datasheet.
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	// Enable HSE Oscillator and activate PLL with HSE as source
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	// This assumes the HSE_VALUE is a multiple of 1MHz. If this is not
	// your case, you have to recompute these PLL constants.
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	// clocks dividers
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
