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

extern char txData[TX_DATA_SIZE];
extern uint8 address;

//Reads from CAN FIFO and changes the state and mode accordingly
int ProcessCAN(CANPacket* receivedPacket, CANPacket* packetToSend) {
    uint16_t packageID = GetPacketID(receivedPacket);
    uint8_t sender_DG = GetSenderDeviceGroupCode(receivedPacket);
    uint8_t sender_SN = GetSenderDeviceSerialNumber(receivedPacket);
    int32_t data = 0;
    int err = 0;
    uint8 current1, current2, current3, current4;
    
    switch(packageID){
        // Board-specific packets
        case(ID_MOTOR_UNIT_MODE_SEL):
            data = GetModeFromPacket(receivedPacket);
            
            if(data == MODE1) {
                SetModeTo(MODE1);
                // initialize MODE1
            } else {
                err = ERROR_INVALID_MODE;
            }
            break;
            
        // Common Packets
        case(ID_ESTOP):
            Print("\r\n\r\nSTOP\r\n\r\n");
            // stop all movement
            GotoUninitState();
            err = ESTOP_ERR_GENERAL;
            break;
        
        case(ID_TELEMETRY_PULL):            
             switch(DecodeTelemetryType(receivedPacket))
            {
                // USE CONSTANTS FOR CASES
                case(PACKET_TELEMETRY_SENSOR1):
                    data = (int32)getCurrent(SENSOR_1_ADDR, &current1);
                    break;
                case(PACKET_TELEMETRY_SENSOR2):
                    data = (int32)getCurrent(SENSOR_2_ADDR, &current2);
                    break;
                case(PACKET_TELEMETRY_SENSOR3):
                    data = (int32)getCurrent(SENSOR_3_ADDR, &current3);
                    break;
                case(PACKET_TELEMETRY_SENSOR4):
                    data = (int32)getCurrent(SENSOR_4_ADDR, &current4);
                    break;
                default:
                    err = ERROR_INVALID_TTC;
                    break;
            }

            
            // Assemble and send packet
            AssembleTelemetryReportPacket(packetToSend, sender_DG, sender_SN, receivedPacket->data[3], data);
            
            if (err == 0)
                SendCANPacket(packetToSend);
            
            break;
            
        default: //recieved Packet with non-valid ID
            // could be due to corruption, don't uninit
            return ERROR_INVALID_PACKET;
    }
    
    return err;
}

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