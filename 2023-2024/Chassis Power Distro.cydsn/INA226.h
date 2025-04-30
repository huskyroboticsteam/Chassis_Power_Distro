#include "project.h"
#include "stdlib.h"
#include <stdint.h>

#define DEVICE_ADDR                 0x40
#define SENSOR_1_ADDR               0b0100000 // 7-bit address
#define SENSOR_2_ADDR               0b0100001
#define SENSOR_3_ADDR               0b0100010
#define SENSOR_4_ADDR               0b0100011


#define SENSOR_1_SHUNT              0x64
#define SENSOR_2_SHUNT              0x64
#define SENSOR_3_SHUNT              0x64
#define SENSOR_4_SHUNT              0x64

#define SENSOR_1_CURLSB             0x56CE
#define SENSOR_2_CURLSB             0x56CE
#define SENSOR_3_CURLSB             0x56CE
#define SENSOR_4_CURLSB             0x56CE

#define INA226_DEVICE_ID            (0x2260)
#define INA226_RESET                (0x8000)
#define INA226_CALIBRATION_REF      (0xA000)

// Registers
#define CONFIG_REG  (0x00)  // R/W
#define SHUNT_REG   (0x01)  // R
#define BUS_REG     (0x02)  // R
#define PWR_REG     (0x03)  // R
#define CUR_REG     (0x04)  // R
#define CAL_REG     (0x05)  // R/W
#define ENBL_REG    (0x06)  // R/W
#define ALRT_REG    (0x07)  // R/W
#define MANID_REG   (0xFE)  // R
#define ID_REG      (0xFF)  // R

#define INA226_BIT_SOL              (0x8000)
#define INA226_BIT_SUL              (0x4000)
#define INA226_BIT_BOL              (0x2000)
#define INA226_BIT_BUL              (0x1000)
#define INA226_BIT_POL              (0x0800)
#define INA226_BIT_CNVR             (0x0400)
#define INA226_BIT_AFF              (0x0010)
#define INA226_BIT_CVRF             (0x0008)
#define INA226_BIT_OVF              (0x0004)
#define INA226_BIT_APOL             (0x0002)
#define INA226_BIT_LEN              (0x0001)

char debugOutput[32];

// values
#define RESET       0b1000000000000000
#define TIMEOUT     20

// functions
uint8 whoAmI(uint8 slaveAddr);
uint8 writeReg16(uint8 slaveAddr, uint8 reg, uint16 val);
uint16 readReg16(uint8 slaveAddr, uint8 regAddr);
uint8 reset(uint8 slaveAddr);
uint16 getCurrent(uint8 slaveAddr);
uint8 getBusVoltage(uint8 slaveAddr);
uint8 setCalibration(uint8 slaveAddr, uint8 rShunt, uint16 currentLSB);
uint8 getShuntVoltage(uint8 slaveAddr);
uint8 setEnable(uint8 slaveAddr, uint8 val);

/* [] END OF FILE */

