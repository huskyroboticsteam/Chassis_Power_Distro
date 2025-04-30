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
#include "HindsightCAN/CANLibrary.h"
#include <stdio.h>

int main(void)
{
    // LFG
    // definitions
    
    // setup can
    CANPacket can_rx;
    CANPacket can_tx;
    
    // initialize
    initialize();
    int err;
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

int getSerialAddress(void) {
    int address = 0;
    
    if (DIP3v_Read()==0) {
        address += 1;
    }
    if (DIP5v_Read()==0) {
        address += 1;
    }
    if (DIP12v_Read()==0){
        address += 1;
    }
    if (DIP24v_Read()==0) {
        address += 1;
    }
    if (address == 0)  {
        address = DEVICE_SERIAL_TELEM_LOCALIZATION;
    }
    
    return address;
}

void initialize(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */
    Inhibit_Write(1); // sets inhibit pin high
    int addr = getSerialAddress(); // -------------------------------
    UART_start(); // start uart
    
    LED_DBG_Write(0); // shut off dbg led
    
    // todo: ping ina addr correctly
    InitCAN(0x4, (int)address); // initialize can
    int err1 = init_INA226(SENSOR_1_ADDR);
    
}

/* [] END OF FILE */
