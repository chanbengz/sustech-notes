#include "touch.h"
#include "24cxx.h"
#include "delay.h"
#include "lcd.h"
#include "math.h"
#include "stdlib.h"
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

_m_tp_dev tp_dev = {
    TP_Init, TP_Scan, TP_Adjust, 0, 0, 0, 0, 0, 0, 0, 0,
};
// Ĭ��Ϊtouchtype=0������.
uint8_t CMD_RDX = 0XD0;
uint8_t CMD_RDY = 0X90;

// SPIд����
// ������ICд��1byte����
// num:Ҫд�������
void TP_Write_Byte(uint8_t num)
{
    uint8_t count = 0;
    for (count = 0; count < 8; count++) {
        if (num & 0x80)
            TDIN = 1;
        else
            TDIN = 0;
        num <<= 1;
        TCLK = 0;
        HAL_Delay_us(1);
        TCLK = 1; // ��������Ч
    }
}
// SPI������
// �Ӵ�����IC��ȡadcֵ
// CMD:ָ��
// ����ֵ:����������
uint16_t TP_Read_AD(uint8_t CMD)
{
    uint8_t count = 0;
    uint16_t Num = 0;
    TCLK = 0;           // ������ʱ��
    TDIN = 0;           // ����������
    TCS = 0;            // ѡ�д�����IC
    TP_Write_Byte(CMD); // ����������
    HAL_Delay_us(6);        // ADS7846��ת��ʱ���Ϊ6us
    TCLK = 0;
    HAL_Delay_us(1);
    TCLK = 1; // ��1��ʱ�ӣ����BUSY
    HAL_Delay_us(1);
    TCLK = 0;
    for (count = 0; count < 16; count++) // ����16λ����,ֻ�и�12λ��Ч
    {
        Num <<= 1;
        TCLK = 0; // �½�����Ч
        HAL_Delay_us(1);
        TCLK = 1;
        if (DOUT)
            Num++;
    }
    Num >>= 4; // ֻ�и�12λ��Ч.
    TCS = 1;   // �ͷ�Ƭѡ
    return (Num);
}
// ��ȡһ������ֵ(x����y)
// ������ȡREAD_TIMES������,����Щ������������,
// Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ
// xy:ָ�CMD_RDX/CMD_RDY��
// ����ֵ:����������
#define READ_TIMES 5 // ��ȡ����//这里是读五次取平均值
#define LOST_VAL 1   // ����ֵ
uint16_t TP_Read_XOY(uint8_t xy)
{
    uint16_t i, j;
    uint16_t buf[READ_TIMES];
    uint16_t sum = 0;
    uint16_t temp;
    for (i = 0; i < READ_TIMES; i++)
        buf[i] = TP_Read_AD(xy);
    for (i = 0; i < READ_TIMES - 1; i++) // ����
    {
        for (j = i + 1; j < READ_TIMES; j++) {
            if (buf[i] > buf[j]) // ��������
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    sum = 0;
    for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++)
        sum += buf[i];
    temp = sum / (READ_TIMES - 2 * LOST_VAL);
    return temp;
}
// ��ȡx,y����
// ��Сֵ��������100.
// x,y:��ȡ��������ֵ
// ����ֵ:0,ʧ��;1,�ɹ���
uint8_t TP_Read_XY(uint16_t *x, uint16_t *y)
{
    uint16_t xtemp, ytemp;
    xtemp = TP_Read_XOY(CMD_RDX);
    ytemp = TP_Read_XOY(CMD_RDY);
    // if(xtemp<100||ytemp<100)return 0;//����ʧ��
    *x = xtemp;
    *y = ytemp;
    return 1; // �����ɹ�
}
// ����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���
// ERR_RANGE,��������,����Ϊ������ȷ,�����������.
// �ú����ܴ�����׼ȷ��
// x,y:��ȡ��������ֵ
// ����ֵ:0,ʧ��;1,�ɹ���
#define ERR_RANGE 50 // ��Χ
uint8_t TP_Read_XY2(uint16_t *x, uint16_t *y)
{
    uint16_t x1, y1;
    uint16_t x2, y2;
    uint8_t flag;
    flag = TP_Read_XY(&x1, &y1);
    if (flag == 0)
        return (0);
    flag = TP_Read_XY(&x2, &y2);
    if (flag == 0)
        return (0);
    if (((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) // ǰ�����β�����+-50��
        && ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE))) {
        *x = (x1 + x2) / 2;
        *y = (y1 + y2) / 2;
        return 1;
    }
    else
        return 0;
}
//////////////////////////////////////////////////////////////////////////////////
// ��LCD�����йصĺ���
// ��һ��������
// ����У׼�õ�
// x,y:����
// color:��ɫ
void TP_Drow_Touch_Point(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_draw_line(x - 12, y, x + 13, y, color); // ����
    lcd_draw_line(x, y - 12, x, y + 13, color); // ����
    lcd_draw_point(x + 1, y + 1, color);
    lcd_draw_point(x - 1, y + 1, color);
    lcd_draw_point(x + 1, y - 1, color);
    lcd_draw_point(x - 1, y - 1, color);
    lcd_draw_circle(x, y, 6, color); // ������Ȧ
}
// ��һ�����(2*2�ĵ�)
// x,y:����
// color:��ɫ
void TP_Draw_Big_Point(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_draw_point(x, y, color); // ���ĵ�
    lcd_draw_point(x + 1, y, color);
    lcd_draw_point(x, y + 1, color);
    lcd_draw_point(x + 1, y + 1, color);
}
//////////////////////////////////////////////////////////////////////////////////
// ��������ɨ��
// tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
// ����ֵ:��ǰ����״̬.
// 0,�����޴���;1,�����д���
uint8_t TP_Scan(uint8_t tp)
{
    if (PEN == 0) // �а�������
    {
        if (tp)
            TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]);      // ��ȡ��������
        else if (TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0])) // ��ȡ��Ļ����
        {
            tp_dev.x[0] = tp_dev.xfac * tp_dev.x[0] + tp_dev.xoff; // �����ת��Ϊ��Ļ����
            tp_dev.y[0] = tp_dev.yfac * tp_dev.y[0] + tp_dev.yoff;
        }
        if ((tp_dev.sta & TP_PRES_DOWN) == 0) // ֮ǰû�б�����
        {
            tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES; // ��������
            tp_dev.x[4] = tp_dev.x[0];                // ��¼��һ�ΰ���ʱ������
            tp_dev.y[4] = tp_dev.y[0];
        }
    }
    else {
        if (tp_dev.sta & TP_PRES_DOWN) // ֮ǰ�Ǳ����µ�
        {
            tp_dev.sta &= ~(1 << 7); // ��ǰ����ɿ�
        }
        else // ֮ǰ��û�б�����
        {
            tp_dev.x[4] = 0;
            tp_dev.y[4] = 0;
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
        }
    }
    return tp_dev.sta & TP_PRES_DOWN; // ���ص�ǰ�Ĵ���״̬
}
//////////////////////////////////////////////////////////////////////////
// ������EEPROM����ĵ�ַ�����ַ,ռ��14���ֽ�(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+13)
#define SAVE_ADDR_BASE 40
// ����У׼����
void TP_Save_Adjdata(void)
{
    AT24CXX_Write(
        SAVE_ADDR_BASE, (uint8_t *)&tp_dev.xfac,
        14); // ǿ�Ʊ���&tp_dev.xfac��ַ��ʼ��14���ֽ����ݣ������浽tp_dev.touchtype//把结构体中的xfac对应的地址写入芯片
    AT24CXX_WriteOneByte(SAVE_ADDR_BASE + 14, 0X0A); // �����д0X0A���У׼����
}
// �õ�������EEPROM�����У׼ֵ
// ����ֵ��1���ɹ���ȡ����
//         0����ȡʧ�ܣ�Ҫ����У׼
uint8_t TP_Get_Adjdata(void)
{
    uint8_t temp;
    temp = AT24CXX_ReadOneByte(SAVE_ADDR_BASE + 14); // ��ȡ�����,���Ƿ�У׼����
    if (temp == 0X0A)                                // �������Ѿ�У׼����	//如果已经校准了
    {
        AT24CXX_Read(SAVE_ADDR_BASE, (uint8_t *)&tp_dev.xfac, 14); // ��ȡ֮ǰ�����У׼����
        if (tp_dev.touchtype)                                 // X,Y��������Ļ�෴
        {
            CMD_RDX = 0X90;
            CMD_RDY = 0XD0;
        }
        else // X,Y��������Ļ��ͬ
        {
            CMD_RDX = 0XD0;
            CMD_RDY = 0X90;
        }
        return 1;
    }
    return 0;
}
// ��ʾ�ַ���
uint8_t *const TP_REMIND_MSG_TBL = "Please use the stylus click the cross on the screen.The cross will always move until "
                              "the screen adjustment is completed.";

// ��ʾУ׼���(��������)
void TP_Adj_Info_Show(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t fac)
{
    lcd_show_string(40, 160, lcddev.width, lcddev.height, 16, "x1:", RED);
    lcd_show_string(40 + 80, 160, lcddev.width, lcddev.height, 16, "y1:", RED);
    lcd_show_string(40, 180, lcddev.width, lcddev.height, 16, "x2:", RED);
    lcd_show_string(40 + 80, 180, lcddev.width, lcddev.height, 16, "y2:", RED);
    lcd_show_string(40, 200, lcddev.width, lcddev.height, 16, "x3:", RED);
    lcd_show_string(40 + 80, 200, lcddev.width, lcddev.height, 16, "y3:", RED);
    lcd_show_string(40, 220, lcddev.width, lcddev.height, 16, "x4:", RED);
    lcd_show_string(40 + 80, 220, lcddev.width, lcddev.height, 16, "y4:", RED);
    lcd_show_string(40, 240, lcddev.width, lcddev.height, 16, "fac is:", RED);
    lcd_show_num(40 + 24, 160, x0, 4, 16, RED);      // ��ʾ��ֵ
    lcd_show_num(40 + 24 + 80, 160, y0, 4, 16, RED); // ��ʾ��ֵ
    lcd_show_num(40 + 24, 180, x1, 4, 16, RED);      // ��ʾ��ֵ
    lcd_show_num(40 + 24 + 80, 180, y1, 4, 16, RED); // ��ʾ��ֵ
    lcd_show_num(40 + 24, 200, x2, 4, 16, RED);      // ��ʾ��ֵ
    lcd_show_num(40 + 24 + 80, 200, y2, 4, 16, RED); // ��ʾ��ֵ
    lcd_show_num(40 + 24, 220, x3, 4, 16, RED);      // ��ʾ��ֵ
    lcd_show_num(40 + 24 + 80, 220, y3, 4, 16, RED); // ��ʾ��ֵ
    lcd_show_num(40 + 56, 240, fac, 3, 16, RED);     // ��ʾ��ֵ,����ֵ������95~105��Χ֮��.
}

// ������У׼����
// �õ��ĸ�У׼����
void TP_Adjust(void)
{
    uint16_t pos_temp[4][2]; // ���껺��ֵ
    uint8_t cnt = 0;
    uint16_t d1, d2;
    uint32_t tem1, tem2;
    double fac;
    uint16_t outtime = 0;
    cnt = 0;
    lcd_clear(WHITE);  // ����
    lcd_clear(WHITE);  // ����
    lcd_show_string(40, 40, 160, 100, 16, (uint8_t *)TP_REMIND_MSG_TBL, BLACK); // ��ʾ��ʾ��Ϣ
    TP_Drow_Touch_Point(20, 20, RED);                              // ����1
    tp_dev.sta = 0;                                                // ���������ź�
    tp_dev.xfac =
        0; // xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������
    while (1) // �������10����û�а���,���Զ��˳�
    {
        tp_dev.scan(1);                          // ɨ����������
        if ((tp_dev.sta & 0xc0) == TP_CATH_PRES) // ����������һ��(��ʱ�����ɿ���.)
        {
            outtime = 0;
            tp_dev.sta &= ~(1 << 6); // ��ǰ����Ѿ����������.

            pos_temp[cnt][0] = tp_dev.x[0];
            pos_temp[cnt][1] = tp_dev.y[0];
            cnt++;
            switch (cnt) {
            case 1:
                TP_Drow_Touch_Point(20, 20, WHITE);              // �����1
                TP_Drow_Touch_Point(lcddev.width - 20, 20, RED); // ����2
                break;
            case 2:
                TP_Drow_Touch_Point(lcddev.width - 20, 20, WHITE); // �����2
                TP_Drow_Touch_Point(20, lcddev.height - 20, RED);  // ����3
                break;
            case 3:
                TP_Drow_Touch_Point(20, lcddev.height - 20, WHITE);              // �����3
                TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, RED); // ����4
                break;
            case 4: // ȫ���ĸ����Ѿ��õ�
                // �Ա����
                tem1 = abs(pos_temp[0][0] - pos_temp[1][0]); // x1-x2
                tem2 = abs(pos_temp[0][1] - pos_temp[1][1]); // y1-y2
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = sqrt(tem1 + tem2); // �õ�1,2�ľ���

                tem1 = abs(pos_temp[2][0] - pos_temp[3][0]); // x3-x4
                tem2 = abs(pos_temp[2][1] - pos_temp[3][1]); // y3-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = sqrt(tem1 + tem2); // �õ�3,4�ľ���
                fac = (float)d1 / d2;
                if (fac < 0.95 || fac > 1.05 || d1 == 0 || d2 == 0) // ���ϸ�
                {
                    cnt = 0;
                    TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); // �����4
                    TP_Drow_Touch_Point(20, 20, RED);                                  // ����1
                    TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
                                     pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); // ��ʾ����
                    continue;
                }
                tem1 = abs(pos_temp[0][0] - pos_temp[2][0]); // x1-x3
                tem2 = abs(pos_temp[0][1] - pos_temp[2][1]); // y1-y3
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = sqrt(tem1 + tem2); // �õ�1,3�ľ���

                tem1 = abs(pos_temp[1][0] - pos_temp[3][0]); // x2-x4
                tem2 = abs(pos_temp[1][1] - pos_temp[3][1]); // y2-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = sqrt(tem1 + tem2); // �õ�2,4�ľ���
                fac = (float)d1 / d2;
                if (fac < 0.95 || fac > 1.05) // ���ϸ�
                {
                    cnt = 0;
                    TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); // �����4
                    TP_Drow_Touch_Point(20, 20, RED);                                  // ����1
                    TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
                                     pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); // ��ʾ����
                    continue;
                } // ��ȷ��

                // �Խ������
                tem1 = abs(pos_temp[1][0] - pos_temp[2][0]); // x1-x3
                tem2 = abs(pos_temp[1][1] - pos_temp[2][1]); // y1-y3
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = sqrt(tem1 + tem2); // �õ�1,4�ľ���

                tem1 = abs(pos_temp[0][0] - pos_temp[3][0]); // x2-x4
                tem2 = abs(pos_temp[0][1] - pos_temp[3][1]); // y2-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = sqrt(tem1 + tem2); // �õ�2,3�ľ���
                fac = (float)d1 / d2;
                if (fac < 0.95 || fac > 1.05) // ���ϸ�// 判断上下距离是不是近似相等的，近似相等才可以通过
                {
                    cnt = 0;
                    TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); // �����4
                    TP_Drow_Touch_Point(20, 20, RED);                                  // ����1
                    TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
                                     pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); // ��ʾ����
                    continue;
                } // ��ȷ��
                // ������
                tp_dev.xfac = (float)(lcddev.width - 40) /
                              (pos_temp[1][0] - pos_temp[0][0]); // �õ�xfac		//这里开始计算出参数
                tp_dev.xoff = (lcddev.width - tp_dev.xfac * (pos_temp[1][0] + pos_temp[0][0])) / 2; // �õ�xoff

                tp_dev.yfac = (float)(lcddev.height - 40) / (pos_temp[2][1] - pos_temp[0][1]);       // �õ�yfac
                tp_dev.yoff = (lcddev.height - tp_dev.yfac * (pos_temp[2][1] + pos_temp[0][1])) / 2; // �õ�yoff
                if (abs(tp_dev.xfac) > 2 || abs(tp_dev.yfac) > 2)                                    // ������Ԥ����෴��.
                {
                    cnt = 0;
                    TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); // �����4
                    TP_Drow_Touch_Point(20, 20, RED);                                  // ����1
                    lcd_show_string(40, 26, lcddev.width, lcddev.height, 16, "TP Need readjust!", BLACK);
                    tp_dev.touchtype = !tp_dev.touchtype; // �޸Ĵ�������.
                    if (tp_dev.touchtype)                 // X,Y��������Ļ�෴
                    {
                        CMD_RDX = 0X90;
                        CMD_RDY = 0XD0;
                    }
                    else // X,Y��������Ļ��ͬ
                    {
                        CMD_RDX = 0XD0;
                        CMD_RDY = 0X90;
                    }
                    continue;
                }
                lcd_clear(WHITE);                                                                    // ����
                lcd_show_string(35, 110, lcddev.width, lcddev.height, 16, "Touch Screen Adjust OK!", BLUE); // У�����
                HAL_Delay(1000);
                TP_Save_Adjdata();
                lcd_clear(WHITE); // ����
                return;           // У�����
            }
        }
        HAL_Delay(10);
        outtime++;
        if (outtime > 1000) {
            TP_Get_Adjdata();
            break;
        }
    }
}
// ��������ʼ��
// ����ֵ:0,û�н���У׼
//        1,���й�У׼
uint8_t TP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE(); // ����GPIOCʱ��

    // PC0,3,13
    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_13; // PC0,3,13
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;                  // �������
    GPIO_Initure.Pull = GPIO_PULLUP;                          // ����
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;                // ����
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    // PC1,2
    GPIO_Initure.Pin = GPIO_PIN_1 | GPIO_PIN_2; // PC1,2
    GPIO_Initure.Mode = GPIO_MODE_INPUT;        // ��������
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    TP_Read_XY(&tp_dev.x[0], &tp_dev.y[0]); // ��һ�ζ�ȡ��ʼ��
    AT24CXX_Init();                         // ��ʼ��24CXX// 保存校准信息到24C芯片里
    if (TP_Get_Adjdata())
        return 0; // �Ѿ�У׼//判断是不是需要校准
    else          // δУ׼?
    {
        lcd_clear(WHITE); // ����
        TP_Adjust();      // ��ĻУ׼
    }
    TP_Get_Adjdata();
    return 1;
}
