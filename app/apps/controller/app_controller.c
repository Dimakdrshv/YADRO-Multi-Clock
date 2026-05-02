#include "app_controller.h"
#include "apps/event/app_events.h"
#include "apps/command/app_commands.h"
#include "services/timer/timer_service.h"
#include "services/lcd/lcd_service.h"
#include "services/rtc/rtc_service.h"
#include "services/command/command_service.h"
#include "services/buzzer/buzzer_service.h"
#include "services/dht11/dht11_service.h"

#define SCREEN_SHOW_STARTUP_M (1u << 0u)
#define SCREEN_SHOW_ALARM_M   (1u << 1u)
#define SCREEN_SHOW_TH_M      (1u << 2u)

#define STARTUP_SCREEN_TIMING (3u)
#define ALARM_SCREEN_TIMING   (7u)
#define TH_SCREEN_TIMING      (7u)
#define ZERO_SCREEN_TIMING    (0u)

static uint32_t app_flags = SCREEN_SHOW_STARTUP_M;
static uint32_t show_timing = STARTUP_SCREEN_TIMING;

static void App_Controller_SetAppFlag(uint32_t flag) {
    app_flags |= flag;
}

static void App_Controller_ClearAppFlag(uint32_t flag) {
    app_flags &= ~flag;
}

static void App_Controller_CheckTiming(uint32_t flag) {
    if (show_timing > 0u) {
        show_timing--;
        if (!show_timing) {
            App_Controller_ClearAppFlag(flag);
            LCD_Service_Clear();
        }
    }
}

static void App_Controller_ShowClock(void) {
    uint8_t hours = 0u;
    uint8_t minutes = 0u;
    uint8_t seconds = 0u;
    uint8_t day = 0u;
    uint8_t month = 0u;
    uint8_t year = 0u;
    uint8_t century = 0u;

    RTC_Service_GetTime(&hours, &minutes, &seconds);
    RTC_Service_GetDate(&day, &month, &year, &century);
    LCD_Service_ShowClock(hours, minutes, seconds, day, month, (uint16_t)(century * 100 + year));
}

static void App_Controller_ShowAlarm(void) {
    uint8_t hours = 0u;
    uint8_t minutes = 0u;

    RTC_Service_GetAlarmTime(&hours, &minutes);
    LCD_Service_ShowAlarm(hours, minutes);
}

static void App_Controller_ShowTemperatureHumidity(void) {
    uint8_t temperature = 0u;
    uint8_t humidity = 0u;

    DHT11_Service_GetTemperatureHumidity(&temperature, &humidity);
    LCD_Service_ShowTemperatureHumidity(temperature, humidity);
}

static void App_Controller_ShowStartup(void) {
    LCD_Service_ShowStartup();
}

static void App_Controller_HandleIRQEvents(uint32_t irq_events) {
    if (irq_events & APP_EVENT_RTC_IS_ALARM_M) {
        app_flags |= SCREEN_SHOW_ALARM_M;
        show_timing = UINT32_MAX;
        LCD_Service_Clear();
        BUZZER_Service_SetHigh();
    }
    
    if (irq_events & APP_EVENT_TIMER32_1_IS_OVERFLOW_M) {
        if (app_flags & SCREEN_SHOW_ALARM_M) {
            App_Controller_ShowAlarm();
            App_Controller_CheckTiming(SCREEN_SHOW_ALARM_M);
        }
        else if (app_flags & SCREEN_SHOW_TH_M) {
            App_Controller_ShowTemperatureHumidity();
            App_Controller_CheckTiming(SCREEN_SHOW_TH_M);
        }
        else if (app_flags & SCREEN_SHOW_STARTUP_M) {
            App_Controller_ShowStartup();
            App_Controller_CheckTiming(SCREEN_SHOW_STARTUP_M);
        }
        else {
            App_Controller_ShowClock();
        }
    }
}

static void App_Controller_HandleServiceEvents(uint32_t events) {
    if (events & APP_EVENT_TIMER32_1_IS_OVERFLOW_M) {
        Command_Service_SendErrorCode(APP_EVENT_TIMER32_1_IS_OVERFLOW_S);
    }
    if (events & APP_EVENT_LCD_DRIVER_CLEAR_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_LCD_DRIVER_CLEAR_ERROR_S);
    }
    if (events & APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_S);
    }
    if (events & APP_EVENT_LCD_DRIVER_WRITE_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_LCD_DRIVER_WRITE_ERROR_S);
    }
    if (events & APP_EVENT_LCD_DRIVER_DISPLAY_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_LCD_DRIVER_DISPLAY_ERROR_S);
    }
    if (events & APP_EVENT_RTC_DRIVER_SETTIME_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_RTC_DRIVER_SETTIME_ERROR_S);
    }
    if (events & APP_EVENT_RTC_DRIVER_SETDATE_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_RTC_DRIVER_SETDATE_ERROR_S);
    }
    if (events & APP_EVENT_RTC_DRIVER_SETALARM_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_RTC_DRIVER_SETALARM_ERROR_S);
    }
    if (events & APP_EVENT_RTC_DRIVER_GETTIME_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_RTC_DRIVER_GETTIME_ERROR_S);
    }
    if (events & APP_EVENT_RTC_DRIVER_GETDATE_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_RTC_DRIVER_GETDATE_ERROR_S);
    }
    if (events & APP_EVENT_RTC_DRIVER_GETALARM_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_RTC_DRIVER_GETALARM_ERROR_S);
    }
    if (events & APP_EVENT_UART_DRIVER_OVERFLOW_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_UART_DRIVER_OVERFLOW_ERROR_S);
    }
    if (events & APP_EVENT_UART_DRIVER_GETCOMMAND_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_UART_DRIVER_GETCOMMAND_ERROR_S);
    }
    if (events & APP_EVENT_UART_DRIVER_EMPTYCOMMAND_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_UART_DRIVER_EMPTYCOMMAND_ERROR_S);
    }
    if (events & APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_UART_DRIVER_INVALIDARG_ERROR_S);
    }
    if (events & APP_EVENT_UART_DRIVER_UNKCOMMAND_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_UART_DRIVER_UNKCOMMAND_ERROR_S);
    }
    if (events & APP_EVENT_DHT11_DRIVER_INVALIDARG_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_DHT11_DRIVER_INVALIDARG_ERROR_S);
    }
    if (events & APP_EVENT_DHT11_DRIVER_READ_ERROR_M) {
        Command_Service_SendErrorCode(APP_EVENT_DHT11_DRIVER_READ_ERROR_S);
    }
}

static void  App_Controller_HandleCommands(uint32_t commands) {
    if (commands & APP_COMMAND_SETTIME_M) {
        RTC_Service_SetTime(Command_Service_GetArg1(), Command_Service_GetArg2(), Command_Service_GetArg3());
    }
    else if (commands & APP_COMMAND_SETDATE_M) {
        RTC_Service_SetDate(Command_Service_GetArg1(), Command_Service_GetArg2(), (uint8_t)(Command_Service_GetArg4() % 100u), (uint8_t)(Command_Service_GetArg4() / 100u));
    }
    else if (commands & APP_COMMAND_DISPLAYON_M) {
        LCD_Service_DisplayOn();
    }
    else if (commands & APP_COMMAND_DISPLAYOFF_M) {
        LCD_Service_DisplayOff();
    }
    else if (commands & APP_COMMAND_SETALARMTIME_M) {
        RTC_Service_SetAlarmTime(Command_Service_GetArg1(), Command_Service_GetArg2());
    }
    else if (commands & APP_COMMAND_SHOWALARMTIME_M) {
        App_Controller_SetAppFlag(SCREEN_SHOW_ALARM_M);
        LCD_Service_Clear();
        show_timing = ALARM_SCREEN_TIMING;
    }
    else if (commands & APP_COMMAND_ALARMON_M) {
        RTC_Service_EnableAlarm();
    }
    else if (commands & APP_COMMAND_ALARMOFF_M) {
        App_Controller_ClearAppFlag(SCREEN_SHOW_ALARM_M);
        RTC_Service_DisableAlarm();
        BUZZER_Service_SetLow();
        LCD_Service_Clear();
        show_timing = ZERO_SCREEN_TIMING;
    }
    else if (commands & APP_COMMAND_SHOWTH_M) {
        App_Controller_SetAppFlag(SCREEN_SHOW_TH_M);
        LCD_Service_Clear();
        show_timing = TH_SCREEN_TIMING;
    }
}

void App_Controller_Process(void) {
    uint32_t irq_events = APP_EVENT_NONE_M;
    uint32_t service_events = APP_EVENT_NONE_M;
    uint32_t commands = APP_COMMANDS_NONE_M;

    irq_events |= TIMER_Service_GetIRQEvents();
    irq_events |= RTC_Service_GetIRQEvents();

    if (irq_events != APP_EVENT_NONE_M) {
        App_Controller_HandleIRQEvents(irq_events);
    }

    Command_Service_Process();
    commands |= Command_Service_GetCommands();
    if (commands != APP_COMMANDS_NONE_M) {
        App_Controller_HandleCommands(commands);
    }

    service_events |= Command_Service_GetEvents();
    service_events |= RTC_Service_GetEvents();
    service_events |= LCD_Service_GetEvents();
    service_events |= BUZZER_Service_GetEvents();
    service_events |= DHT11_Service_GetEvents();

    if (service_events != APP_EVENT_NONE_M) {
        App_Controller_HandleServiceEvents(service_events);
    }
}