#include "command_service.h"
#include "apps/event/app_events.h"
#include "apps/command/app_commands.h"
#include "drivers/uart/uart_driver.h"

#define COMMAND_BUF_SIZE RXDATA_BUF_SIZE
#define MAX_TOKENIZE (5u)

typedef struct __command_service_data {
    uint8_t arg1;
    uint8_t arg2;
    uint8_t arg3;
    uint16_t arg4;
} Command_Service_Data;

static Command_Service_Data csd = {0, 0, 0, 0};
static uint32_t events = APP_EVENT_NONE_M;
static uint32_t commands = APP_COMMANDS_NONE_M;

static inline void Command_Service_RaiseEvent(uint32_t event) {
    events |= event;
}

static inline void Command_Service_RaiseCommand(uint32_t command) {
    commands |= command;
}

static bool Command_Service_CheckOverflow(void) {
    if (!UART_isOverflow()) {
        return false;
    }
    UART_ClearOverflow();
    return true;
}

static bool Command_Service_GetCommand(char * command_buffer) {
    if (!UART_GetCommand(command_buffer)) {
        return false;
    }
    UART_ClearCommandReady();
    return true;
}

static char* Command_Service_DeleteStartSpace(char * buffer) {
    uint8_t i = 0u;
    
    if (buffer == 0) {
        return 0;
    }
    while (buffer[i] == ' ' && buffer[i] != '\0') {
        i++;
    }
    if (buffer[i] == '\0') {
        return 0;
    }
    return &buffer[i];
}

static char* Command_Service_FindNextSpace(char * text) {
    uint8_t i = 0u;
    if (text == 0) {
        return 0;
    }
    while (text[i] != '\0') {
        if (text[i] == ' ') {
            return &text[i];
        }
        ++i;
    }
    return 0;
}

static uint8_t Command_Service_Tokenize(char *command_buffer, char *command_tokens[]) {
    char *buffer = 0;
    char *space = 0;
    uint8_t argc = 0u;

    if ((command_buffer == 0) || (command_tokens == 0)) {
        return 0u;
    }

    buffer = Command_Service_DeleteStartSpace(command_buffer);
    if (buffer == 0) {
        return 0u;
    }

    while ((*buffer != '\0') && (argc < MAX_TOKENIZE)) {
        command_tokens[argc] = buffer;
        ++argc;

        space = Command_Service_FindNextSpace(buffer);
        if (space == 0) {
            break;
        }

        *space = '\0';
        buffer = space + 1;

        while ((*buffer == ' ') && (*buffer != '\0')) {
            ++buffer;
        }

        if (*buffer == '\0') {
            break;
        }
    }

    return argc;
}

static bool Command_Service_StringEqual(const char * string1, const char * string2) {
    if (string1 == 0 || string2 == 0) {
        return false;
    }

    for ( ; *string1 != '\0'; string1++, string2++) {
        if (*string1 != *string2) {
            return false;
        }
    }
    return *string1 == *string2;
}

static bool Command_Service_isDigit(char symbol) {
    return symbol >= '0' && symbol <= '9';
}

static bool Command_Service_ParseDigit8(const char * text, uint8_t* value) {
    uint8_t result = 0u;
    uint8_t digit = 0u;

    if (text == 0 || value == 0) {
        return false;
    }

    while (*text != '\0') {
        if (!Command_Service_isDigit(*text)) {
            return false;
        }
        digit = (uint8_t)((*text) - '0');
        if (result > (UINT8_MAX - digit) / 10u) {
            return false;
        }
        result = result * 10u + digit;
        ++text;
    }
    *value = result;
    return true;
}

static bool Command_Service_ParseDigit16(const char * text, uint16_t* value) {
    uint16_t result = 0u;
    uint16_t digit = 0u;

    if (text == 0 || value == 0) {
        return false;
    }

    while (*text != '\0') {
        if (!Command_Service_isDigit(*text)) {
            return false;
        }
        digit = (uint16_t)((*text) - '0');
        if (result > (UINT16_MAX - digit) / 10u) {
            return false;
        }
        result = result * 10u + digit;
        ++text;
    }
    *value = result;
    return true;
}

static void Command_Service_ParseCommand(char * command_tokens[], uint8_t tokens) {
    if (Command_Service_StringEqual(command_tokens[0], "SET_TIME")) {
        if (tokens != 4u) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }

        if (!Command_Service_ParseDigit8(command_tokens[1], &csd.arg1) ||
            !Command_Service_ParseDigit8(command_tokens[2], &csd.arg2) ||
            !Command_Service_ParseDigit8(command_tokens[3], &csd.arg3)) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }

        Command_Service_RaiseCommand(APP_COMMAND_SETTIME_M);
    }
    else if (Command_Service_StringEqual(command_tokens[0], "SET_DATE")) {
        if (tokens != 4u) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }

        if (!Command_Service_ParseDigit8(command_tokens[1], &csd.arg1) ||
            !Command_Service_ParseDigit8(command_tokens[2], &csd.arg2) ||
            !Command_Service_ParseDigit16(command_tokens[3], &csd.arg4)) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }

        Command_Service_RaiseCommand(APP_COMMAND_SETDATE_M);
    }
    else if (Command_Service_StringEqual(command_tokens[0], "DISPLAY_ON")) {
        if (tokens != 1) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }
        Command_Service_RaiseCommand(APP_COMMAND_DISPLAYON_M);
    }
    else if (Command_Service_StringEqual(command_tokens[0], "DISPLAY_OFF")) {
        if (tokens != 1) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }
        Command_Service_RaiseCommand(APP_COMMAND_DISPLAYOFF_M);
    }
    else if (Command_Service_StringEqual(command_tokens[0], "SET_ALARM_TIME")) {
        if (tokens != 3) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }
        if (!Command_Service_ParseDigit8(command_tokens[1], &csd.arg1) ||
            !Command_Service_ParseDigit8(command_tokens[2], &csd.arg2)) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }
        Command_Service_RaiseCommand(APP_COMMAND_SETALARMTIME_M);
    }
    else if (Command_Service_StringEqual(command_tokens[0], "SHOW_ALARM_TIME")) {
        if (tokens != 1) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }
        Command_Service_RaiseCommand(APP_COMMAND_SHOWALARMTIME_M);
    }
    else if (Command_Service_StringEqual(command_tokens[0], "ALARM_ON")) {
        if (tokens != 1) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }
        Command_Service_RaiseCommand(APP_COMMAND_ALARMON_M);
    }
    else if (Command_Service_StringEqual(command_tokens[0], "ALARM_OFF")) {
        if (tokens != 1) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }
        Command_Service_RaiseCommand(APP_COMMAND_ALARMOFF_M);
    }
    else if (Command_Service_StringEqual(command_tokens[0], "SHOW_TH")) {
        if (tokens != 1) {
            Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M);
            return;
        }
        Command_Service_RaiseCommand(APP_COMMAND_SHOWTH_M);
    }
    else {
        Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_UNKCOMMAND_ERROR_M);
        return;
    }
}

void Command_Service_Process(void) {
    char command_buffer[COMMAND_BUF_SIZE];
    char *command_tokens[MAX_TOKENIZE];
    uint8_t tokens = 0u;

    if (Command_Service_CheckOverflow()) {
        Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_OVERFLOW_ERROR_M);
        return;
    }

    if (!UART_isCommandReady()) {
        return;
    }

    if (!Command_Service_GetCommand(command_buffer)) {
        Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_GETCOMMAND_ERROR_M);
        return;
    }
    
    tokens = Command_Service_Tokenize(command_buffer, command_tokens);
    if (tokens == 0u) {
        Command_Service_RaiseEvent(APP_EVENT_UART_DRIVER_EMPTYCOMMAND_ERROR_M);
        return;
    }

    Command_Service_ParseCommand(command_tokens, tokens);
}

uint32_t Command_Service_GetEvents(void) {
    uint32_t current_events = events;
    events = APP_EVENT_NONE_M;
    return current_events;
}

uint32_t Command_Service_GetCommands(void) {
    uint32_t current_commands = commands;
    commands = APP_COMMANDS_NONE_M;
    return current_commands;
}

uint8_t Command_Service_GetArg1(void) {
    return csd.arg1;
}

uint8_t Command_Service_GetArg2(void) {
    return csd.arg2;
}

uint8_t Command_Service_GetArg3(void) {
    return csd.arg3;
}

uint16_t Command_Service_GetArg4(void) {
    return csd.arg4;
}

void Command_Service_SendErrorCode(uint8_t code) {
    char buf[8];
    
    buf[0] = 'E';
    buf[1] = ':';
    buf[2] = ' ';
    buf[3] = (code / 10) + '0';
    buf[4] = (code % 10) + '0';
    buf[5] = '\r';
    buf[6] = '\n';
    buf[7] = '\0';

    UART_Transmit(buf);
}