#include "24cxx.h"
#include "delay.h"
#include "sys.h"

void AT24CXX_Init(void) { IIC_Init(); }

uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
    uint8_t temp = 0;
    IIC_Start();
    if (EE_TYPE > AT24C16) {
        IIC_Send_Byte(0XA0);
        IIC_Wait_Ack();
        IIC_Send_Byte(ReadAddr >> 8);
    }
    else {
        IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1));
    }
    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr % 256);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(0XA1);
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop();
    return temp;
}

void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{
    IIC_Start();
    if (EE_TYPE > AT24C16) {
        IIC_Send_Byte(0XA0);
        IIC_Wait_Ack();
        IIC_Send_Byte(WriteAddr >> 8);
    }
    else
        IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1));
    IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr % 256);
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);
    IIC_Wait_Ack();
    IIC_Stop();
    HAL_Delay(10);
}

void AT24CXX_WriteLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len)
{
    uint8_t t;
    for (t = 0; t < Len; t++) {
        AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
    }
}

uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr, uint8_t Len)
{
    uint8_t t;
    uint32_t temp = 0;
    for (t = 0; t < Len; t++) {
        temp <<= 8;
        temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
    }
    return temp;
}

uint8_t AT24CXX_Check(void)
{
    uint8_t temp;
    temp = AT24CXX_ReadOneByte(255);
    if (temp == 0X55)
        return 0;
    else {
        AT24CXX_WriteOneByte(255, 0X55);
        temp = AT24CXX_ReadOneByte(255);
        if (temp == 0X55)
            return 0;
    }
    return 1;
}

void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    while (NumToRead) {
        *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
    while (NumToWrite--) {
        AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}
