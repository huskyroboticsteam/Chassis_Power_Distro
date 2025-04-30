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
                case(PACKET_TELEMETRY_SENSOR1): {
                    uint16 sensor1Current = getCurrent(SENSOR_1_ADDR);
                    uint16 sensor1Resolution = readReg16(SENSOR_1_ADDR, CAL_REG);
                    uint16 converted_current = sensor1Current * sensor1Resolution;
                    data = (int32)converted_current;
                    break;
                }
                case(PACKET_TELEMETRY_SENSOR2): {
                    uint16 sensor2Current = getCurrent(SENSOR_2_ADDR);
                    uint16 sensor2Resolution = readReg16(SENSOR_2_ADDR, CAL_REG);
                    uint16 converted_current = sensor2Current * sensor2Resolution;
                    data = (int32)converted_current;
                    break;
                }
                case(PACKET_TELEMETRY_SENSOR3): {
                    uint16 sensor3Current = getCurrent(SENSOR_3_ADDR);
                    uint16 sensor3Resolution = readReg16(SENSOR_3_ADDR, CAL_REG);
                    uint16 converted_current = sensor3Current * sensor3Resolution;
                    data = (int32)converted_current;
                    break;
                }
                case(PACKET_TELEMETRY_SENSOR4): {
                    uint16 sensor4Current = getCurrent(SENSOR_4_ADDR);
                    uint16 sensor4Resolution = readReg16(SENSOR_4_ADDR, CAL_REG);
                    uint16 converted_current = sensor4Current * sensor4Resolution;
                    data = (int32)converted_current;
                    break;
                }
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