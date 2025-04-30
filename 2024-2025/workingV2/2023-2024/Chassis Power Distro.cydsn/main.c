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

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "cyapicallbacks.h"
#include "CAN_Stuff.h"
#include "FSM_Stuff.h"
#include "HindsightCAN/CANLibrary.h"
#include "DataCapture.h"

// LED stuff
volatile uint8_t CAN_time_LED = 0;
volatile uint8_t ERROR_time_LED = 0;
char buffer[64];

uint16 current, voltage;
uint16 theshold;

// UART stuff
char txData[TX_DATA_SIZE];

// CAN stuff
CANPacket can_recieve;
CANPacket can_send;
uint8 address = 2; // dip addr



CY_ISR(Period_Reset_Handler) {
    CAN_time_LED++;
    ERROR_time_LED++;

    if (ERROR_time_LED >= 3) {
        LED_ERR_Write(OFF);
    }
    if (CAN_time_LED >= 3) {
        LED_CAN_Write(OFF);
    }
}

CY_ISR(Button_1_Handler) {
    LED_DBG_Write(!LED_DBG_Read());
}

int main(void)
{ 
    Initialize();
    int err;
    CANPacket packetToSend;
    CANPacket recievedPacket;
   
    for(;;)
    {     
        // Check if recieved packet
        if(!PollAndReceiveCANPacket(&recievedPacket)) {      
            ProcessCAN(&recievedPacket, &packetToSend);
        }

        railStateAssign(GetState());
        
        uint16 sensor3Current = getCurrent(ADDR_3_3V);
        uint16 sensor5Current = getCurrent(ADDR_5V);
        uint16 sensor12Current = getCurrent(ADDR_12V);
        uint16 sensor24Current = getCurrent(ADDR_24V);
        uint8 sensor3Voltage = getShuntVoltage(ADDR_3_3V);
        uint8 sensor5Voltage = getShuntVoltage(ADDR_5V);
        uint8 sensor12Voltage = getShuntVoltage(ADDR_12V);
        uint8 sensor24Voltage = getShuntVoltage(ADDR_24V);
        
        // debugging
        Print("3V current: ");
        PrintIntBuff(sensor3Current);
        Print("\n\r");
        Print("5V current: ");
        PrintIntBuff(sensor5Current);
        Print("\n\r");
        Print("12V current: ");
        PrintIntBuff(sensor12Current);
        Print("\n\r");
        Print("24V current: ");
        PrintIntBuff(sensor24Current);
        Print("\n\r");
//        //Turns off converter if current is too high
//        if(data5v > theshold || data12v > theshold || data24v > theshold){
//            PTN78020W_INHIBIT_Write(0);
//        }
        CyDelay(999);
    }
}

void Initialize(void) {
    CyGlobalIntEnable; /* Enable global interrupts. LED arrays need this first */
    PTN78020W_INHIBIT_Write(1);
    
    UART_Start();
    Print("Dip addr\n\r");
    PrintIntBuff(address);
    
    LED_DBG_Write(0);
    Print("\r\nDBG LED WRite 0\n\r");
    
    InitCAN(0x3, (int)address);
    Print("\r\nCAN INIT\n\r");
    Timer_Period_Reset_Start();

    isr_Button_1_StartEx(Button_1_Handler);
    isr_Period_Reset_StartEx(Period_Reset_Handler);
    
    // start I2C communication
    I2C_Start();
    Print("\r\nI2C START\n\r");
    
    // 5V only test
    Print("\r\nINA's IDENTIFIED\n\r");

    // Write calibration values
    uint8 errCal1 =  writeReg16(ADDR_5V, CAL_REG, CAL_5V);
    uint8 errCal2 =  writeReg16(ADDR_3_3V, CAL_REG, CAL_3V);
    uint8 errCal3 =  writeReg16(ADDR_12V, CAL_REG, CAL_12V);
    uint8 errCal4 =  writeReg16(ADDR_24V, CAL_REG, CAL_24V);
    Print("\r\nCALIBRATION SET\n\r");
    PrintIntBuff(errCal1);
    PrintIntBuff(errCal2);
    PrintIntBuff(errCal3);
    PrintIntBuff(errCal4);
    
}

void DebugPrint(char input) {
    switch(input) {
        case 'f':
            sprintf(txData, "Mode: %x State:%x \r\n", GetMode(), GetState());
            break;
        case 'x':
            sprintf(txData, "bruh\r\n");
            break;
        default:
            sprintf(txData, "what\r\n");
            break;
    }
    Print(txData);
}

void PrintIntBuff(uint16_t num){
    sprintf(buffer, "0x%d\r\n", num);
    UART_PutString(buffer);
}
int getSerialAddress() {
    int address = 0;
    
    if (DIP1_Read()==0) address += 1;
    if (DIP2_Read()==0) address += 2;
    if (DIP3_Read()==0) address += 4;
    if (DIP4_Read()==0) address += 8;
    
    if (address == 0)
        address = DEVICE_SERIAL_TELEM_LOCALIZATION;

    return address;
}

void DisplayErrorCode(uint8_t code) {    
    ERROR_time_LED = 0;
    LED_ERR_Write(ON);
    
    sprintf(txData, "Error %X\r\n", code);
    Print(txData);

    switch(code)
    {
        case ERROR_INVALID_TTC:
            Print("Cannot Send That Data Type!\n\r");
            break;
        default:
            //some error
            break;
    }
}

/* [] END OF FILE */