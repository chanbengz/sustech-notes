#ifndef _MYIIC_H
#define _MYIIC_H

#include "sys.h"
#include <stdint.h>

#define SDA_IN()                                                                                                       \
    {                                                                                                                  \
        GPIOC->CRH &= 0XFFFF0FFF;                                                                                      \
        GPIOC->CRH |= (uint32_t)8 << 12;                                                                               \
    }
#define SDA_OUT()                                                                                                      \
    {                                                                                                                  \
        GPIOC->CRH &= 0XFFFF0FFF;                                                                                      \
        GPIOC->CRH |= (uint32_t)3 << 12;                                                                               \
    }

#define IIC_SCL PCout(12) // SCL
#define IIC_SDA PCout(11) // SDA
#define READ_SDA PCin(11) // ����SDA

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

void IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr, uint8_t addr);
#endif
