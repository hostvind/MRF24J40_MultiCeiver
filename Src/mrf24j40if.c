/*
 * mrf24j40if.c
 *
 *  Created on: 16 окт. 2017 г.
 *      Author: Dumitru
 */


/**************Imports***********************************/

#include "mrf24j40if.h"
#include "stm32f1xx_hal.h"
//#include "SPI/spi.h"               /* Include the spi header */
//#include "Timer/Timer.h"           /* Include the timer header */


/**************Private Macro Definitions*****************/

/* Set the uC pins configuration */

#define ZB_RST_PORT            /* uC port which pin is connected to RST pin of MRF24J40 */
#define ZB_RST_PIN             /* uC pin connected to RST pin of MRF24J40 */
#define ZB_WAKE_PORT           /* uC port which pin is connected to WAKE pin of MRF24J40 */
#define ZB_WAKE_PIN            /* uC pin connected to WAKE pin of MRF24J40 */
#define ZB_INT_PORT            /* uC port which pin is connected to INT pin of MRF24J40 */
#define ZB_INT_PIN             /* uC pin connected to INT pin of MRF24J40 */

#define ZB_SDI_PORT            /* uC port which pin is connected to SDI pin of MRF24J40 */
#define ZB_SDI_PIN             /* uC pin connected to SDI pin of MRF24J40 */
#define ZB_SCK_PORT            /* uC port which pin is connected to SCK pin of MRF24J40 */
#define ZB_SCK_PIN             /* uC pin connected to SCK pin of MRF24J40 */
#define ZB_CS_PORT             /* uC port which pin is connected to CS pin of MRF24J40 */
#define ZB_CS_PIN              /* uC pin connected to CS pin of MRF24J40 */
#define ZB_SDO_PORT            /* uC port which pin is connected to SDO pin of MRF24J40 */
#define ZB_SDO_PIN             /* uC pin connected to SDO pin of MRF24J40 */

/*
 *               ___________________________
                |  _______________________  |
                | |                       | |
                | |                       | |
                | |                       | |
                | |   _____       _____   | |
                | |  |     |     |     |  | |
                | |  |     |     |     |  | |
                | |  |     |     |     |  | |
                | |  |     |     |     |  | |
                | |  |     |     |     |  | |
                | |__|     |_____|     |__| |
                |___________________________|
                |                           |
                >> GND (1)       (12) GND  <<
                |                           |
   ZB_RST_PIN   >> /RST(2)       (11) GND  <<
                |                           |
   ZB_WAKE_PIN  >> WAKE(3)       (10) Vin  <<
                |                           |
   ZB_INT_PIN   >> INT (4)        (9) NC   <<
                |                           |
   ZB_SDI_PIN   >> SDI (5)        (8) /CS  <<   ZB_CS_PIN
                |                           |
   ZB_SCK_PIN   >> SCK (6)        (7) SDO  <<   ZB_SDO_PIN
                |___________________________|
 *
*/


/**************Private Type Definitions******************/



/**************Private Variable Definitions**************/




/**************Public Variable Definitions***************/




/**************Private Function Definitions**************/
void MRF24J40_ChipSelectPullDown(mrf24j40_io* interface){

    /* Use the function to pull down the CS pin*/
    HAL_GPIO_WritePin(interface->MRF_CSN_GPIOx, interface->MRF_CSN_GPIO_PIN, GPIO_PIN_RESET);
}

void MRF24J40_ChipSelectPullUp(mrf24j40_io* interface){

    /* Use the function to pull up the CS pin*/
    HAL_GPIO_WritePin(interface->MRF_CSN_GPIOx, interface->MRF_CSN_GPIO_PIN, GPIO_PIN_SET);
}
uint8_t MRF24J40_SendCommand(mrf24j40_io* interface, uint8_t cmd, uint8_t* tx, uint8_t* rx, uint8_t len)
{
    uint8_t myTX[len + 1];
    uint8_t myRX[len + 1];
    myTX[0] = cmd;

    int i = 0;
    for (i = 0; i < len; i++) {
        myTX[1 + i] = tx[i];
        myRX[i] = 0;
    }
HAL_StatusTypeDef res;
    MRF24J40_ChipSelectPullDown(interface);
    res = HAL_SPI_TransmitReceive(interface->spi, myTX, myRX, 1 + len, MRF_SPI_TIMEOUT);
    if (res        != HAL_OK) {
        HAL_Delay (10);
        HAL_Delay (10);
        HAL_Delay (10);
        HAL_Delay (10);
        return 1;
    }

    for (i = 0; i < len; i++) {
        rx[i] = myRX[1 + i];
    }

    MRF24J40_ChipSelectPullUp(interface);

    return 0;
}


/**************Public Function Definitions***************/



void MRF24J40_WakePinPullUp(mrf24j40_io* interface){

    /* Use the function to pull up the Wake pin*/
    HAL_GPIO_WritePin(interface->MRF_WAKE_GPIOx, interface->MRF_WAKE_GPIO_PIN, GPIO_PIN_SET);
}

void MRF24J40_WakePinPullDown(mrf24j40_io* interface){

    /* Use the function to pull down the Wake pin*/
    HAL_GPIO_WritePin(interface->MRF_WAKE_GPIOx, interface->MRF_WAKE_GPIO_PIN, GPIO_PIN_RESET);
}

void MRF24J40_RstPinPullUp(mrf24j40_io* interface){

    /* Use the function to pull up the RST pin*/
    HAL_GPIO_WritePin(interface->MRF_RESET_GPIOx, interface->MRF_RESET_GPIO_PIN, GPIO_PIN_SET);
}

void MRF24J40_RstPinPullDown(mrf24j40_io* interface){

    /* Use the function to pull down the RST pin*/
    HAL_GPIO_WritePin(interface->MRF_RESET_GPIOx, interface->MRF_RESET_GPIO_PIN, GPIO_PIN_RESET);
}

void MRF24J40_InterruptEnable(mrf24j40_io* interface){

    /* Use the function to enable uC interrupt on pin connected to pin INT of MRF24J40 */
                    /*MOVE TO INIT ROUTINE*/ //HAL_NVIC_SetPriority(interface->MRF_IRQn, interface->MRF_IRQ_preempt_priority, interface->MRF_IRQ_sub_priority);
    HAL_NVIC_EnableIRQ(interface->MRF_IRQn);
}

void MRF24J40_InterruptDisable(mrf24j40_io* interface){

    /* Use the function to disable uC interrupt on pin connected to pin INT of MRF24J40 */
    HAL_NVIC_DisableIRQ(interface->MRF_IRQn);
}

void MRF24J40_ClearInterruptFlag(mrf24j40_io* interface){
    /* Use the function to clear uC interrupt flag on pin connected to pin INT of MRF24J40 */
    /*In HAL not needed - a standart built-in IRQ handle does it*/
}

void MRF24J40_PinOutInit(mrf24j40_io* interface){
    GPIO_InitTypeDef GPIO_InitStruct;

/*
 *  - Configure the PullUp resistor to pin connected to pin INT of MRF24J40 if uC requires separate configuration
 *    (ex: REN register on MSP430)
 */
    // CSN pin
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Pin = interface->MRF_CSN_GPIO_PIN;
    HAL_GPIO_Init(interface->MRF_CSN_GPIOx, &GPIO_InitStruct);
    
    // RESET pin
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Pin = interface->MRF_RESET_GPIO_PIN;
    HAL_GPIO_Init(interface->MRF_RESET_GPIOx, &GPIO_InitStruct);
    
    // WAKE pin
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Pin = interface->MRF_WAKE_GPIO_PIN;
    HAL_GPIO_Init(interface->MRF_WAKE_GPIOx, &GPIO_InitStruct);
 /*  - Configure the pins connected to CS, RST, WAKE as output HIGH ("1")
 */
    MRF24J40_ChipSelectPullUp(interface);
    MRF24J40_RstPinPullUp(interface);
    MRF24J40_WakePinPullUp(interface);    
 /*  - Set the interrupt event on falling edge at INT pin
 */
    // IRQ_Pin
    GPIO_InitStruct.Pin = interface->MRF_IRQ_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(interface->MRF_IRQ_GPIOx, &GPIO_InitStruct);
 /*  - Enable external interrupts
 */
    HAL_NVIC_SetPriority(interface->MRF_IRQn, 0, 0);
    //HAL_NVIC_EnableIRQ(interface->MRF_IRQn);

}

void MRF24J40_Disable(mrf24j40_io* interface){

    /*
     * Set CS and RST pins to LOW ("0")
     * */
    /*DEBUG
    maybe too early
    try at the end of init*/
    MRF24J40_ChipSelectPullDown(interface);
    MRF24J40_RstPinPullDown(interface);
//    MRF24J40_WakePinPullDown(interface); 

}

/* ---------------------------- SPI dependent code ---------------------------- */

void MRF24J40IF_SpiTransmit(mrf24j40_io* interface, uint8_t v) // write 1 byte to SPI
{
    /* Interface for transmit data through SPI */
    uint8_t *pTx = &v;
    uint8_t *pRx;
    HAL_SPI_TransmitReceive(interface->spi, pTx, pRx, 1, MRF_SPI_TIMEOUT);
}

uint8_t MRF24J40IF_SpiReceive(mrf24j40_io* interface) // read 1 byte from SPI
{
    /* Interface for receive data through SPI */
    uint8_t *pTx;
    uint8_t *pRx;
    HAL_SPI_TransmitReceive(interface->spi, pTx, pRx, 1, MRF_SPI_TIMEOUT);
    return *pRx;
}




/* ---------------------------- Timer dependent code ---------------------------- */

void MRF24J40IF_Delay_ms(uint16_t milliseconds){
    /* Delay function (simple delay based on onChip timer is not compatible with OS_Delay) */
    HAL_Delay (milliseconds);
}


uint32_t MRF24J40IF_ElapsedTimeSince(uint32_t time){
    /* "time" represent the saved Timer tick or SYS tick and returning the difference between actual value of
     * SYS tick (timer tick) and "time" parameter */
    return (HAL_GetTick()-time);
}

uint32_t MRF24J40IF_GetSysMilliseconds(void){
    /* This function returning (not)milliseconds (sysTicks) converted value of SYS tick */
    return HAL_GetTick();
}

