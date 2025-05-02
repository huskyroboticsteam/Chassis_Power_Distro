#include "project.h"
#include "stdlib.h"
#include <stdint.h>

#define ADDR_24V                    0b1000000 // 7 bit addrs
#define ADDR_5V                     0b1000001
#define ADDR_12V                    0b1000101 
#define ADDR_3_3V                   0b1000100 


#define SENSOR_1_SHUNT              0x09 // 5vin mOhm -> 10 mOhm
#define SENSOR_2_SHUNT              0xC     // 12 V
#define SENSOR_3_SHUNT              0xF     // 24V
#define SENSOR_4_SHUNT              0xC     // 3.3V

#define CURLSB_3V                   0x1 // in mA -> 1 mA/bit resolution
#define CURLSB_5V                   0x1
#define CURLSB_12V                  0x1
#define CURLSB_24V                  0x1

#define CAL_3V                      0x2A // .00512/ curr_lsb * rshunt          
#define CAL_5V                      0x3B // 200
#define CAL_12V                     0x2A
#define CAL_24V                     0x22

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
uint16 whoAmI(uint8 slaveAddr);
uint8 writeReg16(uint8 slaveAddr, uint8 reg, uint16 val);
uint16 readReg16(uint8 slaveAddr, uint8 regAddr);
uint8 reset(uint8 slaveAddr);
uint16 getCurrent(uint8 slaveAddr);
uint8 getBusVoltage(uint8 slaveAddr);
uint8 setCalibration(uint8 slaveAddr, uint16 rShunt, uint16 currentLSB);
uint16 getShuntVoltage(uint8 slaveAddr);
uint8 setEnable(uint8 slaveAddr, uint8 val);

/* [] END OF FILE */

