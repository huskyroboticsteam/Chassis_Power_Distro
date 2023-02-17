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
#ifndef FSM_H
    #define FSM_H
    #include "CANLibrary.h"
    #include <stdint.h>
    
    #define UINIT       0xFF
    #define IDLE        0x0
    #define CHECK_CURR  0x1
    #define CHECK_BAT   0x2
    #define CHECK_CAN   0x3
    #define WARN        0x4
    
    //Methods for controlling state behavior
    uint8_t getState();
    void setState(uint8_t newState);
    void boardReset();
    
#endif
/* [] END OF FILE */
