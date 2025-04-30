
/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#pragma once
   
#include "cyapicallbacks.h"
#include <stdint.h>
#include "INA226.h"
        
#define ON  1
#define OFF 0

#define TX_DATA_SIZE            (100u)

#define Print(message) UART_PutString(message)
#define PrintChar(character) UART_PutChar(character)
#define PrintInt(integer) UART_PutString(itoa(integer, txData, 10))
#define PrintIntBin(integer) UART_PutString(itoa(integer, txData, 2))

void PrintIntBuff(uint16_t);
void Initialize(void);
int getSerialAddress();
void DebugPrint(char input);
void DisplayErrorCode(uint8_t code);
char debugOutput[32];
/* [] END OF FILE */
