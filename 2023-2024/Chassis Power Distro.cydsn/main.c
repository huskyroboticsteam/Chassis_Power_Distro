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

// LED stuff
volatile uint8_t CAN_time_LED = 0;
volatile uint8_t ERROR_time_LED = 0;

uint16 current, voltage;
uint16 theshold;

// UART stuff
char txData[TX_DATA_SIZE];

// CAN stuff
CANPacket can_recieve;
CANPacket can_send;
uint8 address = 0;



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
        err = 0;
        
        //Try to recieve can packet
        int err = PollAndReceiveCANPacket(&recievedPacket);
        
        // Check if recieved packet
        if(!err){      
            //Get Packet ID
            uint16_t packetID = GetPacketID(&recievedPacket);
            if(packetID == 0xF5){
                //Populate can packet
                AssembleTelemetryReportPacket(&packetToSend, 0x02, 0x01, 0x01, current);
        
                //send Can Packet
                SendCANPacket(&packetToSend);
            }
        }
        
        uint16 val;
        uint16 sensor1Current = getCurrent(SENSOR_1_ADDR);
        Print("Sensor 1 Current: ");
        PrintInt(current);
        
        uint16 sensor1Voltage = getBusVoltage(SENSOR_1_ADDR);
        Print("Sensor 1 Voltage: ");
        PrintInt(voltage);
        Print("\n\r");
        int data = (int32)(getCurrent(SENSOR_1_ADDR) * readReg16(SENSOR_1_ADDR, CAL_REG));
        int data1 = (int32)(getCurrent(SENSOR_2_ADDR) * readReg16(SENSOR_2_ADDR, CAL_REG));
        int data2 = (int32)(getCurrent(SENSOR_3_ADDR) * readReg16(SENSOR_3_ADDR, CAL_REG));
        //Turns off converter if current is too high
        if(data > theshold || data1 > theshold || data2 > theshold){
            PTN78020W_INHIBIT_Write(0);
        }
        ProcessCAN(&recievedPacket, &packetToSend);
        CyDelay(999);
    }
}

void Initialize(void) {
    CyGlobalIntEnable; /* Enable global interrupts. LED arrays need this first */
    PTN78020W_INHIBIT_Write(1);
    address = getSerialAddress();
    
    UART_Start();
    sprintf(txData, "Dip Addr: %x \r\n", address);
    Print(txData);
    
    LED_DBG_Write(0);
    
    InitCAN(0x4, (int)address);
    Timer_Period_Reset_Start();

    isr_Button_1_StartEx(Button_1_Handler);
    isr_Period_Reset_StartEx(Period_Reset_Handler);
    
    // start I2C communication
    I2C_Start();
    
    int id1 = whoAmI(SENSOR_1_ADDR);
    int id2 = whoAmI(SENSOR_2_ADDR);
    int id3 = whoAmI(SENSOR_3_ADDR);
    int id4 = whoAmI(SENSOR_4_ADDR);
    PrintInt(id1);
    PrintInt(id2);
    PrintInt(id3);
    PrintInt(id4);
    Print("\r\nINA's IDENTIFIED\n\r");
    
    uint8 errCal1 = setCalibration(SENSOR_1_ADDR, SENSOR_1_SHUNT, SENSOR_1_CURLSB);
    uint8 errCal2 = setCalibration(SENSOR_2_ADDR, SENSOR_2_SHUNT, SENSOR_2_CURLSB);
    uint8 errCal3 = setCalibration(SENSOR_3_ADDR, SENSOR_3_SHUNT, SENSOR_3_CURLSB);
    uint8 errCal4 = setCalibration(SENSOR_4_ADDR, SENSOR_4_SHUNT, SENSOR_4_CURLSB);
    
    PrintInt(errCal1);
    PrintInt(errCal2);
    PrintInt(errCal3);
    PrintInt(errCal4);
    Print("\r\nCALIBRATION SET\n\r");
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