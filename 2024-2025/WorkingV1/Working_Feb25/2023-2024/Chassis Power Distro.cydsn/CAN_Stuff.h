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

#pragma once

#include <project.h>
#include "HindsightCAN/CANLibrary.h"

// CAN Errors (0x10-0x1F)
#define ERROR_NO_NEW_PACKET 0xFFFF
#define ERROR_WRONG_MODE      0x10
#define ERROR_INVALID_MODE    0x11
#define ERROR_INVALID_TTC     0x12
#define ERROR_INVALID_PACKET  0x13

#define ID_POWER_RAIL_SET               (uint8_t) 0x00
#define ID_POWER_CURRENT_LIM_SET        (uint8_t) 0x01

// Packet DLCs
#define DLC_POWER_RAIL_SET              (uint8_t) 0x02
#define DLC_POWER_CURRENT_LIM_SET       (uint8_t) 0x05

//Packet priorities
#define PRIO_POWER_RAIL_SET             PACKET_PRIORITY_HIGH
#define PRIO_POWER_CURRENT_LIM_SET      PACKET_PRIORITY_NORMAL

int ReadCAN(CANPacket *receivedPacket);
int ProcessCAN(CANPacket* receivedPacket, CANPacket* packetToSend);
void PrintCanPacket(CANPacket packet);

/* [] END OF FILE */
    