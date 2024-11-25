#include "myiic.h"
#include "delay.h"

#include "main.h"

void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_Initure.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    IIC_SDA = 1;
    IIC_SCL = 1;
}

void IIC_Start(void)
{
    SDA_OUT();
    IIC_SDA = 1;
    IIC_SCL = 1;
    HAL_Delay_us(4);
    IIC_SDA = 0;
    HAL_Delay_us(4);
    IIC_SCL = 0;
}

void IIC_Stop(void)
{
    SDA_OUT();
    IIC_SCL = 0;
    IIC_SDA = 0; // STOP:when CLK is high DATA change form low to high
    HAL_Delay_us(4);
    IIC_SCL = 1;
    IIC_SDA = 1;
    HAL_Delay_us(4);
}

uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN();
    IIC_SDA = 1;
    HAL_Delay_us(1);
    IIC_SCL = 1;
    HAL_Delay_us(1);
    while (READ_SDA) {
        ucErrTime++;
        if (ucErrTime > 250) {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL = 0;
    return 0;
}

void IIC_Ack(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 0;
    HAL_Delay_us(2);
    IIC_SCL = 1;
    HAL_Delay_us(2);
    IIC_SCL = 0;
}

void IIC_NAck(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 1;
    HAL_Delay_us(2);
    IIC_SCL = 1;
    HAL_Delay_us(2);
    IIC_SCL = 0;
}

void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL = 0;
    for (t = 0; t < 8; t++) {
        IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        HAL_Delay_us(2);
        IIC_SCL = 1;
        HAL_Delay_us(2);
        IIC_SCL = 0;
        HAL_Delay_us(2);
    }
}

uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();
    for (i = 0; i < 8; i++) {
        IIC_SCL = 0;
        HAL_Delay_us(2);
        IIC_SCL = 1;
        receive <<= 1;
        if (READ_SDA)
            receive++;
        HAL_Delay_us(1);
    }
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}
