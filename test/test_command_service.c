#include "unity.h"

#include "command_service.h"
#include "apps/event/app_events.h"
#include "apps/command/app_commands.h"

#include "mock_uart_driver.h"

void setUp(void) 
{
    Command_Service_GetCommands();
    Command_Service_GetEvents();
}

void tearDown(void) 
{

}

void test_SET_TIME_command(void) 
{
    UART_isOverflow_ExpectAndReturn(false);

    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "SET_TIME 12 30 45";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_COMMAND_SETTIME_M,
        APP_COMMAND_SETTIME_M,
        Command_Service_GetCommands()
    );

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        Command_Service_GetEvents()
    );

    TEST_ASSERT_EQUAL_UINT8(12, Command_Service_GetArg1());
    TEST_ASSERT_EQUAL_UINT8(30, Command_Service_GetArg2());
    TEST_ASSERT_EQUAL_UINT8(45, Command_Service_GetArg3());
}

void test_SET_DATE_command(void) 
{
    UART_isOverflow_ExpectAndReturn(false);

    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "SET_DATE 20 06 2005";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_COMMAND_SETDATE_M,
        APP_COMMAND_SETDATE_M,
        Command_Service_GetCommands()
    );

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        Command_Service_GetEvents()
    );

    TEST_ASSERT_EQUAL_UINT8(20, Command_Service_GetArg1());
    TEST_ASSERT_EQUAL_UINT8(6, Command_Service_GetArg2());
    TEST_ASSERT_EQUAL_UINT16(2005, Command_Service_GetArg4());
}

void test_DISPLAY_ON_command(void) 
{
    UART_isOverflow_ExpectAndReturn(false);

    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "DISPLAY_ON";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_COMMAND_DISPLAYON_M,
        APP_COMMAND_DISPLAYON_M,
        Command_Service_GetCommands()
    );

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        Command_Service_GetEvents()
    );
}

void test_DISPLAY_OFF_command(void) 
{
    UART_isOverflow_ExpectAndReturn(false);

    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "DISPLAY_OFF";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_COMMAND_DISPLAYOFF_M,
        APP_COMMAND_DISPLAYOFF_M,
        Command_Service_GetCommands()
    );

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        Command_Service_GetEvents()
    );
}

void test_SET_ALARM_TIME_command(void) 
{
    UART_isOverflow_ExpectAndReturn(false);

    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "SET_ALARM_TIME 22 33";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_COMMAND_SETALARMTIME_M,
        APP_COMMAND_SETALARMTIME_M,
        Command_Service_GetCommands()
    );

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        Command_Service_GetEvents()
    );

    TEST_ASSERT_EQUAL_UINT8(22, Command_Service_GetArg1());
    TEST_ASSERT_EQUAL_UINT8(33, Command_Service_GetArg2());
}

void test_SHOW_ALARM_TIME_command(void) 
{
    UART_isOverflow_ExpectAndReturn(false);

    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "SHOW_ALARM_TIME";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_COMMAND_SHOWALARMTIME_M,
        APP_COMMAND_SHOWALARMTIME_M,
        Command_Service_GetCommands()
    );

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        Command_Service_GetEvents()
    );
}

void test_ALARM_ON_command(void) 
{
    UART_isOverflow_ExpectAndReturn(false);

    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "ALARM_ON";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_COMMAND_ALARMON_M,
        APP_COMMAND_ALARMON_M,
        Command_Service_GetCommands()
    );

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        Command_Service_GetEvents()
    );
}

void test_ALARM_OFF_command(void) 
{
    UART_isOverflow_ExpectAndReturn(false);

    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "ALARM_OFF";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_COMMAND_ALARMOFF_M,
        APP_COMMAND_ALARMOFF_M,
        Command_Service_GetCommands()
    );

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        Command_Service_GetEvents()
    );
}

void test_SHOW_TH_command(void) 
{
    UART_isOverflow_ExpectAndReturn(false);

    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "SHOW_TH";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_COMMAND_SHOWTH_M,
        APP_COMMAND_SHOWTH_M,
        Command_Service_GetCommands()
    );

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        Command_Service_GetEvents()
    );
}

void test_UART_OVERFLOW_event(void)
{
    UART_isOverflow_ExpectAndReturn(true);
    UART_ClearOverflow_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_EVENT_UART_DRIVER_OVERFLOW_ERROR_M,
        APP_EVENT_UART_DRIVER_OVERFLOW_ERROR_M,
        Command_Service_GetEvents()
    );
}

void test_UART_GETCOMMAND_event(void)
{
    UART_isOverflow_ExpectAndReturn(false);
    UART_isCommandReady_ExpectAndReturn(true);

    UART_GetCommand_ExpectAndReturn(NULL, false);
    UART_GetCommand_IgnoreArg_dst();

    Command_Service_Process();

    TEST_ASSERT_BITS
    (
        APP_EVENT_UART_DRIVER_GETCOMMAND_ERROR_M,
        APP_EVENT_UART_DRIVER_GETCOMMAND_ERROR_M,
        Command_Service_GetEvents()
    );
}

void test_UART_EMPTY_COMMAND_event(void)
{
    UART_isOverflow_ExpectAndReturn(false);
    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS(
        APP_EVENT_UART_DRIVER_EMPTYCOMMAND_ERROR_M,
        APP_EVENT_UART_DRIVER_EMPTYCOMMAND_ERROR_M,
        Command_Service_GetEvents()
    );
}

static void check_invalid_arg_command(const char *command)
{
    UART_isOverflow_ExpectAndReturn(false);
    UART_isCommandReady_ExpectAndReturn(true);

    char buffer[64] = {0};
    strcpy(buffer, command);

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(buffer, sizeof(buffer));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS(
        APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M,
        APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M,
        Command_Service_GetEvents()
    );
}

void test_UART_INVALID_ARGUMETS_1_event(void)
{
    check_invalid_arg_command("SET_TIME 12 30");
}

void test_UART_INVALID_ARGUMETS_2_event(void)
{
    check_invalid_arg_command("SET_TIME am... privet...");
}

void test_UART_UNKNOWN_COMMAND_event(void)
{
    UART_isOverflow_ExpectAndReturn(false);
    UART_isCommandReady_ExpectAndReturn(true);

    char command[64] = "SET_TH";

    UART_GetCommand_ExpectAndReturn(NULL, true);
    UART_GetCommand_IgnoreArg_dst();
    UART_GetCommand_ReturnMemThruPtr_dst(command, sizeof(command));

    UART_ClearCommandReady_Expect();

    Command_Service_Process();

    TEST_ASSERT_BITS(
        APP_EVENT_UART_DRIVER_UNKCOMMAND_ERROR_M,
        APP_EVENT_UART_DRIVER_UNKCOMMAND_ERROR_M,
        Command_Service_GetEvents()
    );
}