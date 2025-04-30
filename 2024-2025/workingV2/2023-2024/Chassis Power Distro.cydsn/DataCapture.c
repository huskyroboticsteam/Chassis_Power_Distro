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
#include "DataCapture.h"
#include "project.h"
#include "INA226.h"



RAILSetting constructRAILSetting(uint8_t railNumber, uint32_t currentLimit){
    RAILSetting rs;
    rs.railNumber = railNumber;
    rs.currentLimit = currentLimit;
    return rs;
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
          
    }
}

void overCurrentLimitAssign(uint8_t railNumber, uint8_t currentLimit){
    constructRAILSetting(railNumber, currentLimit);
}

uint8_t analyzeCurrent(uint8_t railNumber){
    
    uint8_t recordedCurrent;
    recordedCurrent = getCurrent(railNumber);
    
    return recordedCurrent;
}

uint8_t remainingBatteryLife(uint8_t railNumber){
    
    uint8_t recordedLife = getBusVoltage(railNumber);
    
    
    return recordedLife;
    
}

/* [] END OF FILE */
