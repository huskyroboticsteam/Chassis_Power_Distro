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

/* [] END OF FILE */
#include "FSM.h"
#include "Port.h"
#include "project.h"

uint8_t boardState = UINIT;

/*
    Determines the current state of the board in the FSM used for periodic checking of current
    and battery life values
    
    Input/s: N/A
    Output/s: 
        - boardState: State of board
*/
uint8_t getState(){
    return boardState;   
}

/*
    Sets the state of the board to the newState provided, 
    
    Input/s:
        - newState: Desired state for board
    Output/s: N/A
*/
void setState(uint8_t newState){
    boardState = newState;
}

void boardReset(){
    //TODO: Determine what reset behavior would look like   
}