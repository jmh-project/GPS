#ifndef _BSP_CAN
#define _BSP_CAN

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "TEECO_C_Define.h"

extern  CanRxMsg    RxMessage;
extern  volatile    uint8_t     Rx_Flag;     
extern  CanTxMsg    TxMessage;

#define     USE_CAN2

#ifdef  USE_CAN1
  #define CANx                       CAN1
  #define CAN_CLK                    RCC_APB1Periph_CAN1
  #define CAN_RX_PIN                 GPIO_Pin_9		//GPIO_Pin_0
  #define CAN_TX_PIN                 GPIO_Pin_13	//GPIO_Pin_1
  #define CAN_GPIO_RX_PORT           GPIOI
  #define CAN_GPIO_TX_PORT			 GPIOH
  #define CAN_GPIO_CLK               (RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOH)
  #define CAN_AF_PORT                GPIO_AF_CAN1
  #define CAN_RX_SOURCE              GPIO_PinSource9 //GPIO_PinSource0
  #define CAN_TX_SOURCE              GPIO_PinSource13 //GPIO_PinSource1       
#else /*USE_CAN2*/
  #define CANx                       CAN2
  #define CAN_CLK                    (RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2)
  #define CAN_RX_PIN                 GPIO_Pin_5
  #define CAN_TX_PIN                 GPIO_Pin_13
  #define CAN_GPIO_PORT             GPIOB

  #define CAN_GPIO_RX_PORT         GPIOB
  #define CAN_GPIO_TX_PORT		 GPIOB
  
  #define CAN_GPIO_CLK              RCC_AHB1Periph_GPIOB
  #define CAN_AF_PORT              GPIO_AF_CAN2
  #define CAN_RX_SOURCE             GPIO_PinSource5
  #define CAN_TX_SOURCE             GPIO_PinSource13    
#endif  /* USE_CAN1 */     
     
#ifdef __cplusplus
}
#endif

#endif
