/**
 ****************************************************************************************************
 * @file        lcd.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.2
 * @date        2023-05-31
 * @brief       2.8��/3.5��/4.3��/7�� TFTLCD(MCU��) ��������
 *              ֧������IC�ͺŰ���:ILI9341/NT35310/NT35510/SSD1963/ST7789/ST7796/ILI9806 ��
 *
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200530
 * ��һ�η���
 * V1.1 20200607
 * ����lcd_wr_xdata����, ����lcd_ex.c, ��ʱ�任�ռ併��lcd�����Ĵ�����
 * V1.2 20230531
 * 1��������ST7796��ILI9806 IC֧��
 *
 ****************************************************************************************************
 */

#ifndef __LCD_H
#define __LCD_H

#include "stdlib.h"
#include "main.h"
//#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* LCD RST/WR/RD/BL/CS/RS ���� ���� 
 * LCD_D0~D15,��������̫��,�Ͳ������ﶨ����,ֱ����lcd_init�����޸�.��������ֲ��ʱ��,���˸�
 * ��6��IO��, ���ø�LCD_Init�����D0~D15���ڵ�IO��.
 */

/* RESET ��ϵͳ��λ�Ź��� �������ﲻ�ö��� RESET���� */
//#define LCD_RST_GPIO_PORT               GPIOx
//#define LCD_RST_GPIO_PIN                GPIO_PIN_x
//#define LCD_RST_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOx_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */

#define LCD_BL_GPIO_PORT                GPIOC
#define LCD_BL_GPIO_PIN                 GPIO_PIN_10
#define LCD_BL_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */

#define LCD_WR_GPIO_PORT                GPIOC
#define LCD_WR_GPIO_PIN                 GPIO_PIN_7
#define LCD_WR_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */

#define LCD_RD_GPIO_PORT                GPIOC
#define LCD_RD_GPIO_PIN                 GPIO_PIN_6
#define LCD_RD_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */


#define LCD_CS_GPIO_PORT                GPIOC
#define LCD_CS_GPIO_PIN                 GPIO_PIN_9
#define LCD_CS_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */

#define LCD_RS_GPIO_PORT                GPIOC
#define LCD_RS_GPIO_PIN                 GPIO_PIN_8
#define LCD_RS_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */

#define LCD_DATA_GPIO_PORT              GPIOB
#define LCD_DATA_GPIO_PIN               GPIO_PIN_All                                  /* 16��IO���õ� */
#define LCD_DATA_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */

/* LCD �˿ڿ��ƺ������� */
/* ���ò����Ĵ����ķ�ʽ����IO���������Ļ��ˢ���� */
#define LCD_BL(x)       LCD_BL_GPIO_PORT->BSRR = LCD_BL_GPIO_PIN << (16 * (!x))     /* ����BL���� */

#define LCD_WR(x)       LCD_WR_GPIO_PORT->BSRR = LCD_WR_GPIO_PIN << (16 * (!x))     /* ����WR���� */
#define LCD_RD(x)       LCD_RD_GPIO_PORT->BSRR = LCD_RD_GPIO_PIN << (16 * (!x))     /* ����RD���� */
#define LCD_CS(x)       LCD_CS_GPIO_PORT->BSRR = LCD_CS_GPIO_PIN << (16 * (!x))     /* ����CS���� */
#define LCD_RS(x)       LCD_RS_GPIO_PORT->BSRR = LCD_RS_GPIO_PIN << (16 * (!x))     /* ����RS���� */


#define LCD_DATA_OUT(x) LCD_DATA_GPIO_PORT->ODR = x                                 /* дB0~B15���� */
#define LCD_DATA_IN     LCD_DATA_GPIO_PORT->IDR                                     /* ��B0~B15���� */

/******************************************************************************************/

/* LCD��Ҫ������ */
typedef struct
{
    uint16_t width;     /* LCD ��� */
    uint16_t height;    /* LCD �߶� */
    uint16_t id;        /* LCD ID */
    uint8_t dir;        /* ���������������ƣ�0��������1�������� */
    uint16_t wramcmd;   /* ��ʼдgramָ�� */
    uint16_t setxcmd;   /* ����x����ָ�� */
    uint16_t setycmd;   /* ����y����ָ�� */
} _lcd_dev;

/* LCD���� */
extern _lcd_dev lcddev; /* ����LCD��Ҫ���� */

/* LCD�Ļ�����ɫ�ͱ���ɫ */
extern uint32_t  g_point_color;     /* Ĭ�Ϻ�ɫ */
extern uint32_t  g_back_color;      /* ������ɫ.Ĭ��Ϊ��ɫ */

/******************************************************************************************/
/* LCDɨ�跽�����ɫ ���� */

/* ɨ�跽���� */
#define L2R_U2D         0           /* ������,���ϵ��� */
#define L2R_D2U         1           /* ������,���µ��� */
#define R2L_U2D         2           /* ���ҵ���,���ϵ��� */
#define R2L_D2U         3           /* ���ҵ���,���µ��� */

#define U2D_L2R         4           /* ���ϵ���,������ */
#define U2D_R2L         5           /* ���ϵ���,���ҵ��� */
#define D2U_L2R         6           /* ���µ���,������ */
#define D2U_R2L         7           /* ���µ���,���ҵ��� */

#define DFT_SCAN_DIR    L2R_U2D     /* Ĭ�ϵ�ɨ�跽�� */

/* ���û�����ɫ */
#define WHITE           0xFFFF      /* ��ɫ */
#define BLACK           0x0000      /* ��ɫ */
#define RED             0xF800      /* ��ɫ */
#define GREEN           0x07E0      /* ��ɫ */
#define BLUE            0x001F      /* ��ɫ */ 
#define MAGENTA         0XF81F      /* Ʒ��ɫ/�Ϻ�ɫ = BLUE + RED */
#define YELLOW          0XFFE0      /* ��ɫ = GREEN + RED */
#define CYAN            0X07FF      /* ��ɫ = GREEN + BLUE */  

/* �ǳ�����ɫ */
#define BROWN           0XBC40      /* ��ɫ */
#define BRRED           0XFC07      /* �غ�ɫ */
#define GRAY            0X8430      /* ��ɫ */ 
#define DARKBLUE        0X01CF      /* ����ɫ */
#define LIGHTBLUE       0X7D7C      /* ǳ��ɫ */ 
#define GRAYBLUE        0X5458      /* ����ɫ */ 
#define LIGHTGREEN      0X841F      /* ǳ��ɫ */  
#define LGRAY           0XC618      /* ǳ��ɫ(PANNEL),���屳��ɫ */ 
#define LGRAYBLUE       0XA651      /* ǳ����ɫ(�м����ɫ) */ 
#define LBBLUE          0X2B12      /* ǳ����ɫ(ѡ����Ŀ�ķ�ɫ) */ 

/******************************************************************************************/
/* SSD1963������ò���(һ�㲻�ø�) */

/* LCD�ֱ������� */ 
#define SSD_HOR_RESOLUTION      800     /* LCDˮƽ�ֱ��� */ 
#define SSD_VER_RESOLUTION      480     /* LCD��ֱ�ֱ��� */ 

/* LCD������������ */ 
#define SSD_HOR_PULSE_WIDTH     1       /* ˮƽ���� */ 
#define SSD_HOR_BACK_PORCH      46      /* ˮƽǰ�� */ 
#define SSD_HOR_FRONT_PORCH     210     /* ˮƽ���� */ 

#define SSD_VER_PULSE_WIDTH     1       /* ��ֱ���� */ 
#define SSD_VER_BACK_PORCH      23      /* ��ֱǰ�� */ 
#define SSD_VER_FRONT_PORCH     22      /* ��ֱǰ�� */ 

/* ���¼����������Զ����� */ 
#define SSD_HT          (SSD_HOR_RESOLUTION + SSD_HOR_BACK_PORCH + SSD_HOR_FRONT_PORCH)
#define SSD_HPS         (SSD_HOR_BACK_PORCH)
#define SSD_VT          (SSD_VER_RESOLUTION + SSD_VER_BACK_PORCH + SSD_VER_FRONT_PORCH)
#define SSD_VPS         (SSD_VER_BACK_PORCH)
   
/******************************************************************************************/
/* �������� */

/* LCDд����, �������ĳɺ궨�庯��, �Դﵽ����ٶ�
 * -O2�Ż�ʱ, ���lcd_wr_dataʹ����ͨ��������, ֻ�ܵ�15֡ˢ��
 * -O2�Ż�ʱ, ���lcd_wr_dataʹ��__forceinline��������, �ܵ�39֡ˢ��
 * -O2�Ż�ʱ, ���lcd_wr_dataʹ�ú궨�庯��, �ܵ�51֡ˢ��
 */
#define lcd_wr_data(data)\
    {\
        LCD_RS(1);\
        LCD_CS(0);\
        LCD_DATA_OUT(data);\
        LCD_WR(0);\
        LCD_WR(1);\
        LCD_CS(1);\
    }

void lcd_wr_xdata(uint16_t data);                   /* LCDд����, �ú���ͬ lcd_wr_data �����Ĺ���һģһ�� */
void lcd_wr_regno(volatile uint16_t regno);         /* LCDд�Ĵ������/��ַ */
void lcd_write_reg(uint16_t regno, uint16_t data);  /* LCDд�Ĵ�����ֵ */


void lcd_init(void);                        /* ��ʼ��LCD */ 
void lcd_display_on(void);                  /* ����ʾ */ 
void lcd_display_off(void);                 /* ����ʾ */
void lcd_scan_dir(uint8_t dir);             /* ������ɨ�跽�� */ 
void lcd_display_dir(uint8_t dir);          /* ������Ļ��ʾ���� */ 
void lcd_ssd_backlight_set(uint8_t pwm);    /* SSD1963 ������� */ 

void lcd_write_ram_prepare(void);                           /* ׼��ЩGRAM */ 
void lcd_set_cursor(uint16_t x, uint16_t y);                /* ���ù�� */ 
uint32_t lcd_read_point(uint16_t x, uint16_t y);            /* ����(32λ��ɫ,����LTDC)  */
void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color);/* ����(32λ��ɫ,����LTDC) */

void lcd_clear(uint16_t color);                                                             /* LCD���� */
void lcd_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);                   /* ���ʵ��Բ */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);                  /* ��Բ */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color);                  /* ��ˮƽ�� */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);             /* ���ô��� */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color);          /* ��ɫ������(32λ��ɫ,����LTDC) */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);   /* ��ɫ������ */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);     /* ��ֱ�� */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);/* ������ */


void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color);                       /* ��ʾһ���ַ� */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color);                     /* ��ʾ���� */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color);      /* ��չ��ʾ���� */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color);   /* ��ʾ�ַ��� */


#endif

















