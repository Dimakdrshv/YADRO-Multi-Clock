// <*!> This file contains the mapping of the IO registers for the mik32 microcontroller.
#pragma once

#include <mik32_memory_map.h>

//==============================================================================
//                             <*!> UART                                       =
//==============================================================================
#define UART_0_BASE              (UART_0)

#define UART_0_TX_PIN            (6u)
#define UART_0_RX_PIN            (5u)
#define UART_0_TX_PIN_MASK       (1u << UART_0_TX_PIN)
#define UART_0_RX_PIN_MASK       (1u << UART_0_RX_PIN)

#define UART_0_PAD_CFG_REG       (PAD_CONFIG->PORT_0_CFG)
#define UART_0_PAD_DS_REG        (PAD_CONFIG->PORT_0_DS)
#define UART_0_PAD_PUPD_REG      (PAD_CONFIG->PORT_0_PUPD)

#define UART_0_CLOCK_MASK        (PM_CLOCK_APB_P_UART_0_M)

#define UART_0_IRQ_INDEX         (EPIC_UART_0_INDEX)


//==============================================================================
//                             <*!> I2C                                        =
//==============================================================================
#define I2C_1_BASE               (I2C_1)

#define I2C_1_SDA_PIN            (12u)
#define I2C_1_SCL_PIN            (13u)
#define I2C_1_SDA_PIN_MASK       (1u << I2C_1_SDA_PIN)
#define I2C_1_SCL_PIN_MASK       (1u << I2C_1_SCL_PIN)

#define I2C_1_PAD_CFG_REG        (PAD_CONFIG->PORT_1_CFG)
#define I2C_1_PAD_DS_REG         (PAD_CONFIG->PORT_1_DS)
#define I2C_1_PAD_PUPD_REG       (PAD_CONFIG->PORT_1_PUPD)

#define I2C_1_CLOCK_MASK         (PM_CLOCK_APB_P_I2C_1_M)


//==============================================================================
//                            <*!> MTIMER                                      =
//==============================================================================
#define MTIMER_BASE              (SCR1_TIMER)

//==============================================================================
//                            <*!> RTC                                         =
//==============================================================================
#define RTC_BASE                 (RTC)

#define RTC_CLOCK_MASK           (PM_CLOCK_APB_M_RTC_M)

#define RTC_IRQ_INDEX            (EPIC_RTC_INDEX)

//==============================================================================
//                            <*!> TIMER32                                     =
//==============================================================================
#define TIMER32_1_BASE           (TIMER32_1)

#define TIMER32_1_CLOCK_MASK     (PM_CLOCK_APB_P_TIMER32_1_M)

#define TIMER32_1_IRQ_INDEX      (EPIC_TIMER32_1_INDEX)

//==============================================================================
//                            <*!> BUZZER                                      =
//==============================================================================
#define BUZZER_BASE              (GPIO_0)

#define BUZZER_PIN               (10u)
#define BUZZER_PIN_MASK          (1u << BUZZER_PIN)

#define BUZZER_PAD_CFG_REG       (PAD_CONFIG->PORT_0_CFG)
#define BUZZER_PAD_DS_REG        (PAD_CONFIG->PORT_0_DS)
#define BUZZER_PAD_PUPD_REG      (PAD_CONFIG->PORT_0_PUPD)

#define BUZZER_CLOCK_MASK        (PM_CLOCK_APB_P_GPIO_0_M)

//==============================================================================
//                            <*!> DHT11                                       =
//==============================================================================
#define DHT11_BASE               (GPIO_0)

#define DHT11_PIN                (8u)
#define DHT11_PIN_MASK           (1u << DHT11_PIN)

#define DHT11_PAD_CFG_REG        (PAD_CONFIG->PORT_0_CFG)
#define DHT11_PAD_DS_REG         (PAD_CONFIG->PORT_0_DS)
#define DHT11_PAD_PUPD_REG       (PAD_CONFIG->PORT_0_PUPD)

#define DHT11_CLOCK_MASK         (PM_CLOCK_APB_P_GPIO_0_M)