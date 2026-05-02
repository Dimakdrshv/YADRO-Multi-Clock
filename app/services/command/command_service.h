#pragma once

#include <stdint.h>
#include <stdbool.h>

void Command_Service_Process(void);

uint32_t Command_Service_GetEvents(void);
uint32_t Command_Service_GetCommands(void);

uint8_t Command_Service_GetArg1(void);
uint8_t Command_Service_GetArg2(void);
uint8_t Command_Service_GetArg3(void);
uint16_t Command_Service_GetArg4(void);

void Command_Service_SendErrorCode(uint8_t code);