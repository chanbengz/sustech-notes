#ifndef _24CXX_H
#define _24CXX_H

#include <stdint.h>

#define AT24C01 127
#define AT24C02 255
#define AT24C04 511
#define AT24C08 1023
#define AT24C16 2047
#define AT24C32 4095
#define AT24C64 8191
#define AT24C128 16383
#define AT24C256 32767
#define EE_TYPE AT24C02

uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite);
void AT24CXX_WriteLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len);
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr, uint8_t Len);
void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);
void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);

uint8_t AT24CXX_Check(void);
void AT24CXX_Init(void);
#endif
