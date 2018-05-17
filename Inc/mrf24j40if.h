/*
 * mrf24j40if.h
 *
 *  Created on: 16 окт. 2017 г.
 *      Author: Dumitru
 */

#ifndef MRF24J40IF_H_
#define MRF24J40IF_H_


/***************************Imports*************************************************/

#include <stdint.h>
#include "stm32f1xx_hal.h"

/***************************Public Macro Definitions********************************/

#define MILLISECONDS(x)        x
#define MRF24J40_TIMEOUT_TIME  MILLISECONDS(30)
#define MRF_SPI_TIMEOUT 100000

/***************************Public Type Definitions********************************/
        /*Pinout and SPI handle container*/
typedef struct {

    volatile uint32_t ERRLVL;

    /* STM32 SPI Peripheral HAL handle */
    SPI_HandleTypeDef* spi;

    /* STM32 GPIO/EXTI Peripheral Parameter */
    
    GPIO_TypeDef* MRF_WAKE_GPIOx; // WAKE pin
    uint16_t MRF_WAKE_GPIO_PIN; 
    
    GPIO_TypeDef* MRF_RESET_GPIOx; // RESET pin
    uint16_t MRF_RESET_GPIO_PIN;   
    
    GPIO_TypeDef* MRF_CSN_GPIOx; // CSN pin
    uint16_t MRF_CSN_GPIO_PIN;

    GPIO_TypeDef* MRF_CE_GPIOx; // CE pin
    uint16_t MRF_CE_GPIO_PIN;

    GPIO_TypeDef* MRF_IRQ_GPIOx; // IRQ pin
    uint16_t MRF_IRQ_GPIO_PIN;
    IRQn_Type MRF_IRQn;
    uint8_t MRF_IRQ_preempt_priority;
    uint8_t MRF_IRQ_sub_priority;

} mrf24j40_dev;


/***************************Public Data Definitions********************************/



/***************************Public Function Definitions****************************/
uint8_t MRF24J40_SendCommand(uint8_t cmd, uint8_t* tx, uint8_t* rx, uint8_t len);
void MRF24J40_ChipSelectPullDown(void);
void MRF24J40_ChipSelectPullUp(void);
void MRF24J40_WakePinPullUp(void);
void MRF24J40_WakePinPullDown(void);
void MRF24J40_RstPinPullUp(void);
void MRF24J40_RstPinPullDown(void);
void MRF24J40_PinOutInit(void);
void MRF24J40_InterruptEnable(void);
void MRF24J40_InterruptDisable(void);
void MRF24J40_ClearInterruptFlag(void);
void MRF24J40_Disable(void);
void     MRF24J40IF_SpiTransmit(uint8_t v);
uint8_t  MRF24J40IF_SpiReceive(void);

void     MRF24J40IF_Delay_ms(uint16_t milliseconds);
uint32_t MRF24J40IF_GetSysMilliseconds(void);
uint32_t MRF24J40IF_ElapsedTimeSince(uint32_t time);

#endif /* MRF24J40IF_H_ */
