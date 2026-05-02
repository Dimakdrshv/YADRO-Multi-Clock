// <*!> This file contains the declaration of the UART driver
#pragma once

#include <stdbool.h>
#include <stdint.h>

#define RXDATA_BUF_SIZE 64

// [start bit][frame = 8][no parity][1 stop bit]
// message\n

typedef enum __uart_status {
    UART_DRIVER_OK = 0,
    UART_DRIVER_ERROR,
} UART_Status;

UART_Status UART_init(void);

void UART_Transmit(const char * src);

bool UART_isCommandReady(void);

bool UART_isOverflow(void);

bool UART_GetCommand(char * dst);

void UART_ClearCommandReady(void);

void UART_ClearOverflow(void);

void UART_IRQHandler(void);