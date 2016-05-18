/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#define START_DELAY 5000
#define KTIR_THRESHHOLD_FRONT 180
#define KTIR_THRESHHOLD_BACK 3800
#define KTIR_DELAY 200
#define SHARP_THRESHOLD 1000
#include <errno.h>
#include <math.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
DMA_HandleTypeDef hdma_tim3_ch4_up;
DMA_HandleTypeDef hdma_tim4_up;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint16_t GPIO_Pin_tab[4]={GPIO_PIN_7,GPIO_PIN_9,GPIO_PIN_8,GPIO_PIN_6};
int pwm_div=0;
int time_i=0;
uint32_t sensor[7]={0,0,0,0,0,0,0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void Wait_Start();    //czekaj na sygnal start
void Kill_switch();   //Umrzyj
void Start_all();     //odpala adc i timery
void turn(int l_wheel, int r_wheel); //jedz/skrec
void seek(uint8_t dir); 	  //szukaj przeciwnika
uint8_t destroy();    //zniszcz
void escape();
void GPIO_Reset_State(); //wylacz diody
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
	  /* USER CODE BEGIN 1 */

	  /* USER CODE END 1 */

	  /* MCU Configuration----------------------------------------------------------*/

	  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	  HAL_Init();

	  /* Configure the system clock */
	  SystemClock_Config();

	  /* Initialize all configured peripherals */
	  MX_GPIO_Init();
	  MX_DMA_Init();
	  MX_ADC1_Init();
	  MX_TIM1_Init();
	  MX_TIM3_Init();
	  MX_TIM4_Init();

	  /* USER CODE BEGIN 2 */
	  Start_all();
	  Wait_Start();

	  uint8_t direction=0;
	  /* USER CODE END 2 */

	  /* Infinite loop */
	  /* USER CODE BEGIN WHILE */
	  while (1)
	  {
		  seek(direction);
		  direction=destroy();
	  /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */

	  }
	  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 7;
  HAL_ADC_Init(&hadc1);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 2;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 3;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 4;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 6;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 7;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}

/* TIM1 init function */
void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 3599;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 2499;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim1);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

}

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 359;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 99;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim3);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);

}

/* TIM4 init function */
void MX_TIM4_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 359;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 99;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim4);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);

}

/** 
  * Enable DMA controller clock
  */
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : Start_Pin Killswitch_Pin */
  GPIO_InitStruct.Pin = Start_Pin|Killswitch_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Diode4_Pin Diode3_Pin Diode2_Pin Diode1_Pin 
                           IN1L_Pin */
  GPIO_InitStruct.Pin = Diode4_Pin|Diode3_Pin|Diode2_Pin|Diode1_Pin 
                          |IN1L_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : IN2L_Pin */
  GPIO_InitStruct.Pin = IN2L_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(IN2L_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : IN1R_Pin IN2R_Pin */
  GPIO_InitStruct.Pin = IN1R_Pin|IN2R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */
void GPIO_Reset_State()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
}

void Wait_Start()
{
	if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	while(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)) {}
	GPIO_Reset_State();
	}
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_Delay(START_DELAY);
	HAL_TIM_Base_Stop_IT(&htim1);

}

void Kill_switch()
{/*
	if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14) && !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
	{
		turn(0,0);
		GPIO_Reset_State();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	}
	while(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14) && !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)) {}
*/
}

void Start_all()
{
	HAL_ADC_Start_DMA(&hadc1,sensor, 7);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); //generacja PWM
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1); //generacja PWM
}


void turn(int l_wheel, int r_wheel)
{
	if(l_wheel>=0) //CCW
	{
		TIM3->CCR2 = l_wheel/pwm_div;
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET); //IN1L
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET); //IN2L
	}
	if(l_wheel<0) //CW
	{
		TIM3->CCR2 = -l_wheel/pwm_div;
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET); //IN1L
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);    //IN2L
	}
	if(r_wheel>=0) //CW
	{
		TIM4->CCR1 = r_wheel/pwm_div;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);  //IN1R
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET); //IN2R
	}
	if(r_wheel<0) //CCW
	{
		TIM4->CCR1 = -r_wheel/pwm_div;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET); //IN1R
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);  //IN2R
	}
}

void seek(uint8_t dir)
{
	Kill_switch();
	escape();
	if(sensor[0] < SHARP_THRESHOLD && sensor[5] < SHARP_THRESHOLD+350 && sensor[6] < SHARP_THRESHOLD)
	{
		int left=pow(-1,1-dir);
		int right=pow(-1,dir);
		turn(left*50 ,right*50 );
		GPIO_Reset_State();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET); //odpal lewa
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET); //odpal prawa
		while(sensor[0] < SHARP_THRESHOLD &&
				sensor[5] < SHARP_THRESHOLD+350 &&
					sensor[6] < SHARP_THRESHOLD)
		{
			Kill_switch();
			escape();
		}
	}
}

uint8_t destroy()
{
	Kill_switch();
	escape();
	uint8_t StateS0= sensor[0]>=SHARP_THRESHOLD;
	uint8_t StateS5= sensor[5]>=SHARP_THRESHOLD+350;
	uint8_t StateS6= sensor[6]>=SHARP_THRESHOLD;
	uint8_t memo=0;
	while(StateS0 || StateS5 || StateS6)
	{
		Kill_switch();
		escape();
		memo=StateS6 < StateS5;
		StateS0= sensor[0]>=SHARP_THRESHOLD;
		StateS5= sensor[5]>=SHARP_THRESHOLD+350;
		StateS6= sensor[6]>=SHARP_THRESHOLD;
		turn(30*StateS0 + 70*StateS5, 30*StateS0 + 70*StateS6);
		GPIO_Reset_State();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,StateS0);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,StateS6);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,StateS5);
	}
	return memo;
}

void escape()
{
	if(sensor[1] <= KTIR_THRESHHOLD_FRONT || sensor[3] <= KTIR_THRESHHOLD_FRONT)
	{
		Kill_switch();
		turn(-100 , -100 );
		GPIO_Reset_State();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET); //odpal tyl
		HAL_Delay(KTIR_DELAY);
/*	while(sensor[1] <= KTIR_THRESHHOLD_FRONT || sensor[3] <= KTIR_THRESHHOLD_FRONT)
	{
		Kill_switch();
	}*/
		turn(-50 , 50 );
		GPIO_Reset_State();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET); //odpal lewa
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET); //odpal prawa
	}

	if(sensor[2] <= KTIR_THRESHHOLD_BACK || sensor[4] <= KTIR_THRESHHOLD_BACK)
	{
		Kill_switch();
		turn(100 , 100 );
		GPIO_Reset_State();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET); //odpal tyl
		HAL_Delay(KTIR_DELAY);
/*	while(sensor[2] <= KTIR_THRESHHOLD_BACK || sensor[4] <= KTIR_THRESHHOLD_BACK)
	{
		Kill_switch();
	}*/
		turn(-50 , 50 );
		GPIO_Reset_State();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET); //odpal lewa
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET); //odpal prawa
	}
	//seek(0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0) // Sprawdzenie czy przerwanie zostalo wywolane przez pin 0
		{
			pwm_div++;
			pwm_div%=5;
		}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim1) // Sprawdzenie, czy przerwanie zostalo wywolane przez timer 1
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_Pin_tab[time_i]); // Zmien stan na Porcie C
		time_i++; time_i%=4;
	}
}
/* USER CODE END 4 */


#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
