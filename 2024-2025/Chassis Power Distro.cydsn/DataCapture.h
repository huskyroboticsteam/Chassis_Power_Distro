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
#ifndef DataCapture_H
    #define DataCapture_H
    #include <stdint.h>
    
    #define ALL_RAIL_OFF        0x00
    #define RAIL_3V3_ONLY       0x01
    #define RAIL_5V_ONLY        0x02
    #define RAIL_12V_ONLY       0x03
    #define RAIL_3V3_5V         0x04
    #define RAIL_3V3_12V        0x05
    #define RAIL_5V_12V         0x06
    #define ALL_RAIL_ON         0x07
    
    #define RAIL_NUMS           0X03
    #define RAILSET_3V3         0x00
    #define RAILSET_5V          0x01
    #define RAILSET_12V         0x02
    
    typedef struct
    {
        uint8_t railNumber;
        uint32_t currentLimit;
    } RAILSetting;
    
    void checkCurr();
    
    //Methods used to set the initial values of the board
    void railStateAssign(uint8_t packetData);
    void overCurrentLimitAssign(uint8_t railNumber, uint32_t currentLimit);
    RAILSetting constructRAILSetting(uint8_t railNumber, uint32_t currentLimit);
    
    //Method use to collect Current Data, need INA226 methods
    uint16_t analyzeCurrent(uint8_t railNumber);
    
    
#endif
/* [] END OF FILE */
