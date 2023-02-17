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
#include "project.h"
#include "CANLibrary.h"
#include "FSM.h"

#define Print(message) DBG_UART_UartPutString(message)

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    CANPacket send;
    CANPacket recieve;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    for(;;)
    {
        /* 
        
        switch(getState()){
            case 1: idle state where the board will return to after running current checks on CANrequest 
                    Bootloop check to make sure the board doesn't get stuck in a loop when checking current
            
            case 2: Check current state where the board will check that all rails have nominal current readings
                    Should flag and send a CAN packet notifying for any bad current readings
                    or when requested by CAN
            
            case 3: Same as case 2 but should also update LED indicators to reflect current battery life
                    Should flag and send a CAN packet notifying low battery (20% remaining)
                    or when requested by CAN
            
            case 4: Check for incoming CAN messages or send CAN responses when needed
        }
        */
        switch(getState()) {
            case(IDLE):
                CyDelay(5000);
                setState(CHECK_CURR);
                break;
            case(CHECK_CURR):
                // This will use the INA226 driver so will need to figure out what functions are needed
                // Update LEDS for current? Should be one for each board but need to ask hardware
                setState(CHECK_BAT);
                break;
            case(CHECK_BAT):
                // Will also use INA226 driver
                // Update LEDS for battery life
                setState(CHECK_CAN);
                break;
            case(CHECK_CAN):
                // Send or check CAN to see if we need to read something or send something
                break;
            default:
                break;
        }
    }
}

/* [] END OF FILE */
