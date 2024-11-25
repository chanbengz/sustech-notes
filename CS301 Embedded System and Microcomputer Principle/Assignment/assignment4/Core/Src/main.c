/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "touch.h"
#include "lcd.h"
#include "delay.h"

#include <stdio.h>
#include <string.h>
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
unsigned char DATA_TO_SEND[800];
int state_num = 0;
uint8_t STATE[30];
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

void Load_Drow_Dialog(void)
{
  lcd_clear(WHITE);                                         // 清屏
  lcd_show_string(lcddev.width - 24, 0, 200, 16, 16, "RST", BLUE); // 显示清屏区域
}

void screen_print()
{
  lcd_clear(WHITE);                                         // 清屏
  lcd_show_string(lcddev.width - 24, 0, 200, 16, 16, "RST", BLUE); // 显示清屏区域
  lcd_show_string(0, 0, 200, 24, 24, "SHOW PICTURE", BLUE);
  lcd_show_string(60, 60, 200, 24, 24, "SEND MESSAGE", BLUE);
  lcd_show_string(0, lcddev.height - 24, 200, 16, 16, STATE, BLUE);
  lcd_show_num(0, 250, lcddev.id, 10, 16, BLUE);
}

void screen_norm_print()
{
  //	LCD_Clear(WHITE);//清屏
  lcd_show_string(lcddev.width - 24, 0, 200, 16, 16, "RST", BLUE); // 显示清屏区域
  lcd_show_string(0, 0, 200, 24, 24, "SHOW PICTURE", BLUE);
  lcd_show_string(60, 60, 200, 24, 24, "SEND MESSAGE", BLUE);
  lcd_show_string(0, lcddev.height - 24, 200, 16, 16, STATE, BLUE);
  lcd_show_num(0, 250, lcddev.id, 10, 16, BLUE);
}

void change_state()
{
  if (state_num == 0)
  {
    state_num = 1;
    sprintf(STATE, "STATE: ON");
  }
  else
  {
    state_num = 0;
    sprintf(STATE, "STATE: OFF");
  }
}
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay_Init(72);
  lcd_init();
  tp_dev.init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  lcd_show_string(30, 50, 200, 16, 16, "Mini STM32", RED);
  lcd_show_string(30, 70, 200, 16, 16, "TOUCH TEST", RED);
  lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, "2019/11/15", RED);
  if (tp_dev.touchtype != 0XFF)
  {
    lcd_show_string(30, 130, 200, 16, 16, "Press KEY0 to Adjust", RED); // 电阻屏才显示
  }
  HAL_Delay(1500);
  Load_Drow_Dialog();

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    
    tp_dev.scan(0);
    screen_norm_print();
    if (tp_dev.sta & TP_PRES_DOWN) // 触摸屏被按下
    {
      if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height)
      {
        if (tp_dev.x[0] > (lcddev.width - 24) && tp_dev.y[0] < 16)
        {
          screen_print(); // 清除
        }
        else if (tp_dev.x[0] < 80 && tp_dev.y[0] < 24)
        {
          // DELETED IMAGE
          lcd_color_fill(70, 70, 169, 169, gImage_11);
        }
        else if (tp_dev.x[0] > 60 && tp_dev.y[0] > 60 && tp_dev.x[0] < 180 && tp_dev.y[0] < 100)
        {
          sprintf(DATA_TO_SEND, "SEND DATA");
          HAL_UART_Transmit(&huart1, (uint8_t *)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);
        }
        else if (tp_dev.x[0] > 0 && tp_dev.y[0] > lcddev.height - 24 && tp_dev.x[0] < 80 && tp_dev.y[0] < lcddev.height)
        {
          change_state();
        }
        else
        {
          TP_Draw_Big_Point(tp_dev.x[0], tp_dev.y[0], RED); // 画图
        }
      }
    }
    else
      HAL_Delay(10);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
