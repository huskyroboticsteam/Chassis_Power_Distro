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

#include <stdio.h>
#include "main.h"
#include "CAN_Stuff.h"
#include "FSM_Stuff.h"
#include "HindsightCAN/CANLibrary.h"
#include "DataCapture.h"
#include "CANPower.h"

extern char txData[TX_DATA_SIZE];
extern uint8 address;

//Reads from CAN FIFO and changes the state and mode accordingly
int ProcessCAN(CANPacket* receivedPacket, CANPacket* packetToSend) {
    uint16_t packageID = GetPacketID(receivedPacket);
    uint8_t sender_DG = GetSenderDeviceGroupCode(receivedPacket);
    uint8_t sender_SN = GetSenderDeviceSerialNumber(receivedPacket);
    int32_t data = 0;
    int err = 0;
    uint16 sensor1Current, sensor2Current, sensor3Current, sensor4Current;
    
    switch(packageID){
        case(ID_TELEMETRY_PULL):         
            switch(DecodeTelemetryType(receivedPacket))
            {
                case(ID_POWER_RAIL_SET):
                    railStateAssign(GetPowerRailsStateFromPacket(receivedPacket));
                    LED_CAN_Write(~LED_CAN_Read());
                    break;
                case(ID_POWER_CURRENT_LIM_SET):
                    /* TODO: Once INA226 Firmware is completed, will call necessary methods
                             to set the current limits for the respective rail
                    */
                    overCurrentLimitAssign(GetOverCurrentRailNumFromPacket(receivedPacket), 
                    GetOverCurrentLimitFromPacket(receivedPacket));
                    LED_CAN_Write(~LED_CAN_Read());
                    break;
                // 3.3V
                case(PACKET_TELEMETRY_SENSOR1):
                    sensor1Current = getCurrent(ADDR_3_3V);
                    data = sensor1Current;
                    Print("3V current: ");
                    PrintIntBuff(sensor1Current);
                    break;
                // 5V
                case(PACKET_TELEMETRY_SENSOR2):
                    sensor2Current = getCurrent(ADDR_5V);
                    data = sensor2Current;
                    Print("5V current: ");
                    PrintIntBuff(sensor2Current);
                    break;
                // 12V
                case(PACKET_TELEMETRY_SENSOR3):
                    sensor3Current = getCurrent(ADDR_12V);
                    data = sensor3Current;
                    Print("`12V current: ");
                    PrintIntBuff(sensor3Current);
                    break;
                // 24V
                case(PACKET_TELEMETRY_SENSOR4):
                    sensor4Current = getCurrent(ADDR_24V);
                    data = sensor4Current;
                    PrintIntBuff(sensor4Current);
                    Print("3V current: ");
                    PrintIntBuff(sensor4Current);
                    break;
                case(PACKET_TELEMETRY_PWR_RAIL_STATE): {
                    char str_buffer[32];
                    static int arr[3];
                    arr[0] = Pin_Ena12V_Read();
                    arr[1] =  Pin_Ena5V_Read();
                    arr[2] =  Pin_Ena3V3_Read();
                    PrintIntBuff(arr[0]);
                    PrintIntBuff(arr[1]);
                    PrintIntBuff(arr[2]);
                // other cases michael wanted
                }
                case(ID_ESTOP):
                    data = 0x00;
                    break;
                case(ID_FAIL_REPORT):
                    data = packageID;
                    break;
                case(ID_OVRD_PROTECTION):
                    break;
                case(ID_TELEMETRY_TIMING):
                    static uint32_t lastTelemetryTime = 0;
                    uint32_t currentTelemetryTime = GetSystemTime(); // Assuming a function that gets system time in ms
                    data = currentTelemetryTime - lastTelemetryTime;
                    lastTelemetryTime = currentTelemetryTime;
                    break;
                case(ID_TELEMETRY_REPORT):
                    data = DecodeTelemetryType(receivedPacket);
                    break;
                case(ID_LED_COLOR):
                    // uhh idk?
                    break;
                case(ID_CHIP_TYPE_PULL):
                    data = CHIP_TYPE_PSOC_CY8C4248AZI_L485; // can adjust this in Port.h in HindsightCAN
                    break;
                case(ID_CHIP_TYPE_REP):
                    data = CHIP_TYPE_PSOC_CY8C4248AZI_L485; // can adjust this in Port.h in HindsightCAN
                    break;
                default:
                    err = ERROR_INVALID_TTC;
                    break;
            }
        break;
        case(ID_HEARTBEAT):
            data = 0xFF;
            break;
        default:
            return ERROR_INVALID_PACKET;
    }
    AssembleTelemetryReportPacket(packetToSend, sender_DG, sender_SN, receivedPacket->data[3], data);
        
    if (err == 0) {
        SendCANPacket(packetToSend);
    }
    
    return err;
};

void PrintCanPacket(CANPacket packet){
    for(int i = 0; i < packet.dlc; i++ ) {
        sprintf(txData,"Byte%d %x   ", i+1, packet.data[i]);
        Print(txData);
    }

    sprintf(txData,"ID:%x %x %x\r\n",packet.id >> 10, 
        (packet.id >> 6) & 0xF , packet.id & 0x3F);
    Print(txData);
}


/* [] END OF FILE */