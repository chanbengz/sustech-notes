#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// ALIENTEK STM32������
// ����������(֧��ADS7843/7846/UH7843/7846/XPT2046/TSC2046/OTT2001A/GT9147/FT5206��)����
// ����ԭ��@ALIENTEK
// ������̳:www.openedv.com
// ��������:2015/1/15
// �汾��V1.0
// ��Ȩ���У�����ؾ���
// Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////

#define TP_PRES_DOWN 0x80 // ����������
#define TP_CATH_PRES 0x40 // �а���������
#define CT_MAX_TOUCH 5    // ������֧�ֵĵ���,�̶�Ϊ5��

// ������������
typedef struct {
    uint8_t (*init)(void);
    uint8_t (*scan)(uint8_t);
    void (*adjust)(void);
    uint16_t x[CT_MAX_TOUCH];
    uint16_t y[CT_MAX_TOUCH];
    uint8_t sta;
    float xfac;
    float yfac;
    short xoff;
    short yoff;
    uint8_t touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;

#define PEN PCin(1)   // PC1  INT
#define DOUT PCin(2)  // PC2  MISO
#define TDIN PCout(3) // PC3  MOSI
#define TCLK PCout(0) // PC0  SCLK
#define TCS PCout(13) // PC13 CS

void TP_Write_Byte(uint8_t num);
uint16_t TP_Read_AD(uint8_t CMD);
uint16_t TP_Read_XOY(uint8_t xy);
uint8_t TP_Read_XY(uint16_t *x, uint16_t *y);
uint8_t TP_Read_XY2(uint16_t *x, uint16_t *y);
void TP_Drow_Touch_Point(uint16_t x, uint16_t y, uint16_t color);
void TP_Draw_Big_Point(uint16_t x, uint16_t y, uint16_t color);
void TP_Save_Adjdata(void);
uint8_t TP_Get_Adjdata(void);
void TP_Adjust(void);
void TP_Adj_Info_Show(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t fac);

uint8_t TP_Scan(uint8_t tp);
uint8_t TP_Init(void);

#endif
