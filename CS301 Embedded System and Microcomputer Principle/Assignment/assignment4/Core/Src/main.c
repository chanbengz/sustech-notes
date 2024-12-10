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
#define SLIDE_THRESHOLD 5
#define MAP_START 13980
#define NUM_OBS 10
#define TIME_LIMIE 30
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
extern int game_state = 0;
extern int game_time = 0;
extern int theme = 0;
extern int speed = 0;

int player_state;
int sliding;

struct object {
	int x, y;
} car, obstacles[NUM_OBS];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

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
  HAL_TIM_Base_Start_IT(&htim3);

  // lcd_show_string(80, 100, 200, 24, 24, "Welcome", BLACK);
  int last_y = 0;
  int last_up_time = 0, last_down_time;
  int slide_cum = 0;
  int track_start = MAP_START;
  int jump = 1;

  lcd_show_string(80, 50, 200, 24, 24, "Welcome", BLACK);
  lcd_draw_rectangle(85, 95, 150, 130, BLACK);
  lcd_show_string(90, 100, 210, 24, 24, "Start", BLACK);
  lcd_draw_rectangle(35, 135, 190, 170, BLACK);
  lcd_show_string(40, 140, 200, 24, 24, "Change theme", BLACK);

  while (game_state != 1) {
	    tp_dev.scan(0);
	    if ((tp_dev.sta & TP_PRES_DOWN) && (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height)) // 触摸屏被按下
	    {
	    	if (tp_dev.x[0] < 150 && tp_dev.y[0] > 95 && tp_dev.x[0] > 85 && tp_dev.y[0] < 130) // start
	    	{
	    		game_state = 1;
	    		break;
	    	}
	    	else if (tp_dev.x[0] > 30 && tp_dev.y[0] > 135 && tp_dev.x[0] < 190 && tp_dev.y[0] < 170) // change theme
	    	{
	    		theme = theme ? 0 : 1;
	    	}
	    }
	    char s[20];
	    sprintf(s, "Current theme: %s", theme ? "Boat Racing" : "F1 Racing");
	    lcd_show_string(30, 200, 200, 16, 16, s, BLACK);
	    HAL_Delay(50);
  }

  // game start

  car.x = 105;
  car.y = 0; // 0: straight, 1: left, 2: right
  srand(game_time);
  for (int i = 0; i < 5; i++) {
	  obstacles[i].x = rand() % 210;
	  obstacles[i].y = rand() % 1000 + 200;
  }

  speed = 0; // [-3, 3]
  lcd_show_string(10, 0, 200, 16, 16, "Time: 0", BLACK);

  lcd_color_fill(10, 280, 75, 312, button);
  lcd_color_fill(85, 280, 150, 312, button);
  lcd_color_fill(160, 280, 225, 312, button);

  lcd_show_string_trans(20, 285, 200, 24, 24, "Left", BLACK);
  lcd_show_string_trans(90, 285, 200, 24, 24, "Jump", BLACK);
  lcd_show_string_trans(165, 285, 200, 24, 24, "Right", BLACK);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
    
    tp_dev.scan(0);
    if ((tp_dev.sta & TP_PRES_DOWN) && (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height)) // 触摸屏被按下
    {
        if (tp_dev.x[0] < 80 && tp_dev.y[0] > 270 && tp_dev.x[0] > 0 && tp_dev.y[0] < lcddev.height) // left
        {
        	car.y = 1;
        	if (car.x > 10) car.x -= 5;
        }
        else if (tp_dev.x[0] > 80 && tp_dev.y[0] > 270 && tp_dev.x[0] < 160 && tp_dev.y[0] < lcddev.height) // jump
        {
        	jump = 1;
        }
        else if (tp_dev.x[0] > 160 && tp_dev.y[0] > 270 && tp_dev.x[0] < 240 && tp_dev.y[0] < lcddev.height) // right
        {
        	car.y = 2;
        	if (car.x < 310) car.x += 5;
        }
        else // slide
        {
        	if (sliding++) slide_cum += tp_dev.y[0] - last_y;
        	last_y = tp_dev.y[0];
        }
    }
    else {
    	car.y = jump = 0; // reset car status

		if (sliding > 1) { // sliding
			int interval;
			if (slide_cum < -SLIDE_THRESHOLD) {
				lcd_show_string(200, 0, 200, 16, 16, "Up!!!", BLACK);
				interval = game_time - last_up_time;
				if (interval < 8) speed = 3 - interval / 2;
			}
			else if (slide_cum > SLIDE_THRESHOLD) {
				lcd_show_string(200, 0, 200, 16, 16, "Down!", BLACK);
				interval = game_time - last_down_time;
				if (interval < 8) speed = interval / 2 - 3;
			}
			last_up_time = last_down_time = game_time;
		}
		slide_cum = sliding = 0;
    }

    // detect collision
    if (!jump)
    for (int i = 0; i < NUM_OBS; i++) {
        int obstacle_y = obstacles[i].y - track_start / 15;
        if (obstacle_y >= 140 && obstacle_y <= 170) {
        	if ((car.x > obstacles[i].x && car.x < obstacles[i].x + 30) || (car.x < obstacles[i].x && car.x + 30 > obstacles[i].x)) {
        		if(speed > 0) speed = 0;
        		lcd_show_string(200, 0, 200, 16, 16, "Coll", BLACK);
        	}
        }
    }

    // update screen

    if (track_start > 240 && game_time < TIME_LIMIE * 4) { // game on
    	if (track_start < MAP_START || speed > 0) {
    		track_start -= speed * 120;
    	}

    	if (theme) {
    		lcd_fill(0, 20, 240, 320, 0x047b);
    		int window = 300 - track_start / 15;
    		if (window < 300 && window > 0)
    			lcd_color_fill_scale(0, window + 20, 240, window + 75, finish_line, 4, 0);
    	} else{
    		lcd_color_fill_scale(0, 20, 240, 320, background_1, 4, track_start);
    	}

    	// obstacle
    	for (int i = 0; i < NUM_OBS; i++) {
    		int window = obstacles[i].y - track_start / 15;
    		if (window < 300 && window > 0) {
    			lcd_color_fill_trans(
    				obstacles[i].x, window + 20, obstacles[i].x + 29, window + 49,
					theme ? obstacle2 : obstacle1, 1);
    		}
    	}

    	// boat or car
    	if (theme) {
    		if (!jump) lcd_color_fill_trans(car.x, 190, car.x + 24, 219, player_boat, 1);
    		else lcd_color_fill_trans(car.x - 5, 183, car.x + 29, 227, player_boat_jump, 1);
    	} else {
    		if (!jump) lcd_color_fill_trans(car.x, 190, car.x + 29, 219, player_car[car.y], 1);
    		else lcd_color_fill_trans(car.x - 7, 183, car.x + 37, 227, player_car_jump, 1);
    	}

        // button
        lcd_color_fill(10, 280, 75, 312, button);
        lcd_color_fill(85, 280, 150, 312, button);
        lcd_color_fill(160, 280, 225, 312, button);

        lcd_show_string_trans(20, 285, 200, 24, 24, "Left", BLACK);
        lcd_show_string_trans(90, 285, 200, 24, 24, "Jump", BLACK);
        lcd_show_string_trans(165, 285, 200, 24, 24, "Right", BLACK);
    } else if (track_start < 240) { // game over
    	lcd_show_string_trans(70, 50, 200, 24, 24, "FINISHED!", RED);
    	lcd_show_string_trans(70, 80, 200, 24, 24, "YOU WIN!", RED);
    	game_state = 2;
    } else if (game_time > TIME_LIMIE * 4) { // game fail
    	lcd_show_string_trans(70, 50, 200, 24, 24, "TIME OUT!", RED);
    	lcd_show_string_trans(70, 80, 200, 24, 24, "YOU LOSE!", RED);
    	game_state = 3;
    }
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
