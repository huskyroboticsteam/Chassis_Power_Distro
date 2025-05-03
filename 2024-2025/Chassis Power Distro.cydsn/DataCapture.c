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
#include "DataCapture.h"
#include "project.h"
#include "INA226.h"
#include "main.h"

RAILSetting railSetting[RAIL_NUMS];


RAILSetting constructRAILSetting(uint8_t railNumber, uint32_t currentLimit){
    RAILSetting rs;
    rs.railNumber = railNumber;
    rs.currentLimit = currentLimit;
    return rs;
}

void checkCurr() {
    for (int i = 0; i < RAIL_NUMS; i++) {
        uint16_t current = analyzeCurrent(railSetting[i].railNumber);
//        Print("Current rail:");
//        PrintIntBuff(railSetting[i].railNumber);
//        Print("Current current (mA):");
//        PrintIntBuff(current);
//        Print("Current limit (mA):");
//        PrintIntBuff((unsigned int) railSetting[i].currentLimit);
//        int arr[3];
//        arr[0] = Pin_Ena12V_Read();
//        arr[1] =  Pin_Ena5V_Read();
//        arr[2] =  Pin_Ena3V3_Read();
//        Print("12V: ");
//        PrintIntBuff(arr[0]);
//        Print("5V: ");
//        PrintIntBuff(arr[1]);
//        Print("3V3: ");
//        PrintIntBuff(arr[2]);
        if (current > railSetting[i].currentLimit) {
           if (i == 0) {
               Pin_Ena3V3_Write(0);
           } else if(i == 1) {
               Pin_Ena5V_Write(0);
           } else {
               Pin_Ena12V_Write(0);
           }
        }
        if (current < railSetting[i].currentLimit) {
           if (i == 0) {
               Pin_Ena3V3_Write(1);
           } else if(i == 1) {
               Pin_Ena5V_Write(1);
           } else {
               Pin_Ena12V_Write(1);
           }
        }
    }
}

void railStateAssign(uint8_t packetData){
    /*
        TODO: Determine what values will represent each rail and create a loop to check
              the bits of packetData and determine which rails need to be on
            
              Add output for codes that do not correspond with possible rail states 
    */
    if (packetData ==  ALL_RAIL_ON){
        Pin_Ena3V3_Write(1);
        Pin_Ena5V_Write(1);
        Pin_Ena12V_Write(1);
        
    } else if (packetData == RAIL_3V3_ONLY){
        Pin_Ena3V3_Write(1);
        Pin_Ena5V_Write(0);
        Pin_Ena12V_Write(0);
        
    } else if (packetData == RAIL_5V_ONLY){
        Pin_Ena3V3_Write(0);
        Pin_Ena5V_Write(1);
        Pin_Ena12V_Write(0);
        
    } else if (packetData == RAIL_12V_ONLY){
        Pin_Ena3V3_Write(0);
        Pin_Ena5V_Write(0);
        Pin_Ena12V_Write(1);
        
    } else if (packetData == RAIL_3V3_5V){
        Pin_Ena3V3_Write(1);
        Pin_Ena5V_Write(1);
        Pin_Ena12V_Write(0);
        
    } else if (packetData == RAIL_3V3_12V){
        Pin_Ena3V3_Write(1);
        Pin_Ena5V_Write(0);
        Pin_Ena12V_Write(1);
        
    } else if (packetData == RAIL_5V_12V){
        Pin_Ena3V3_Write(0);
        Pin_Ena5V_Write(1);
        Pin_Ena12V_Write(1);
        
    } else if (packetData == ALL_RAIL_OFF){
        Pin_Ena3V3_Write(0);
        Pin_Ena5V_Write(0);
        Pin_Ena12V_Write(0);
        
    } else {
        Pin_Ena3V3_Write(1);
        Pin_Ena5V_Write(1);
        Pin_Ena12V_Write(1);
    }
    int arr[3];
    arr[0] = Pin_Ena12V_Read();
    arr[1] =  Pin_Ena5V_Read();
    arr[2] =  Pin_Ena3V3_Read();
    PrintIntBuff(arr[0]);
    PrintIntBuff(arr[1]);
    PrintIntBuff(arr[2]);
}

void overCurrentLimitAssign(uint8_t railNumber, uint32_t currentLimit){
    if (railNumber == ADDR_3_3V) {
        railSetting[0] = constructRAILSetting(railNumber, currentLimit);
    }
    else if (railNumber == ADDR_5V) {
        railSetting[1] = constructRAILSetting(railNumber, currentLimit);
    } else if (railNumber == ADDR_12V) {
        railSetting[2] = constructRAILSetting(railNumber, currentLimit);
    }
}

uint16_t analyzeCurrent(uint8_t railNumber){
    
    uint16_t recordedCurrent;
    recordedCurrent = getCurrent(railNumber);
    
    return recordedCurrent;
}

/* [] END OF FILE */
