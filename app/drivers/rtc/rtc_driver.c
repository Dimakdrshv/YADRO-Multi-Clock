#include "rtc_driver.h"
#include <rtc.h>
#include "platform/io_mapping.h"
#include "platform/clock_control.h"
#include "platform/epic_irq.h"

static uint32_t alarm_flag = 0u;

#define RTC_MONDAY    (0b001u)
#define RTC_TUESDAY   (0b010u)
#define RTC_WEDNESDAY (0b011u)
#define RTC_THURSDAY  (0b100u)
#define RTC_FRIDAY    (0b101u)
#define RTC_SATURDAY  (0b110u)
#define RTC_SUNDAY    (0b111u)

#define RTC_FIELD_SET(VALUE, SHIFT, MASK) \
            ((((uint32_t)(VALUE)) << SHIFT) & MASK)

#define RTC_FIELD_GET(REG, SHIFT, MASK) \
            ((((uint32_t)(REG) & MASK)) >> SHIFT)

static void RTC_enable(void) {
    RTC_BASE->CTRL |= RTC_CTRL_EN_M;
}

static void RTC_disable(void) {
    RTC_BASE->CTRL &= ~RTC_CTRL_EN_M;
}

static void RTC_set_default_time(void) {
    RTC_BASE->TIME = RTC_FIELD_SET(0, RTC_TIME_S_S, RTC_TIME_S_M) |
                     RTC_FIELD_SET(0, RTC_TIME_TS_S, RTC_TIME_TS_M) |
                     RTC_FIELD_SET(0, RTC_TIME_M_S, RTC_TIME_M_M) |
                     RTC_FIELD_SET(0, RTC_TIME_TM_S, RTC_TIME_TM_M) |
                     RTC_FIELD_SET(0, RTC_TIME_H_S, RTC_TIME_H_M) |
                     RTC_FIELD_SET(0, RTC_TIME_TH_S, RTC_TIME_TH_M) |
                     RTC_FIELD_SET(RTC_MONDAY, RTC_TIME_DOW_S, RTC_TIME_DOW_M);
}

static void RTC_set_default_date(void) {
    RTC_BASE->DATE = RTC_FIELD_SET(0, RTC_DATE_TC_S, RTC_DATE_TC_M) |
                     RTC_FIELD_SET(0, RTC_DATE_C_S, RTC_DATE_C_M) |
                     RTC_FIELD_SET(0, RTC_DATE_TY_S, RTC_DATE_TY_M) |
                     RTC_FIELD_SET(0, RTC_DATE_Y_S, RTC_DATE_Y_M) |
                     RTC_FIELD_SET(0, RTC_DATE_TM_S, RTC_DATE_TM_M) |
                     RTC_FIELD_SET(0, RTC_DATE_M_S, RTC_DATE_M_M) |
                     RTC_FIELD_SET(0, RTC_DATE_TD_S, RTC_DATE_TD_M) |
                     RTC_FIELD_SET(0, RTC_DATE_D_S, RTC_DATE_D_M);
}

static void RTC_interrupt_enable(void) {
    RTC_BASE->CTRL |= RTC_CTRL_INTE_M;
}

RTC_Status RTC_init(void) {
    Clock_enable_RTC();

    RTC_disable();
    RTC_set_default_time();
    RTC_set_default_date();
    RTC_interrupt_enable();
    RTC_enable();

    EPIC_IRQ_enable(RTC_IRQ_INDEX);

    return RTC_DRIVER_OK;
}

static uint8_t RTC_bcd_tens(uint8_t value) {
    return (uint8_t)(value / 10);
}

static uint8_t RTC_bcd_units(uint8_t value) {
    return (uint8_t)(value % 10);
}

static bool RTC_is_valid_time(const RTC_Time *time) {
    if (time == 0) {
        return false;
    }

    if (time->hours > 23u) {
        return false;
    }
    if (time->minutes > 59u) {
        return false;
    }
    if (time->seconds > 59u) {
        return false;
    }
    if (time->dow > RTC_SUNDAY || time->dow < RTC_MONDAY) {
        return false;
    }

    return true;
}

static uint16_t RTC_full_year(const RTC_Date *date) {
    return (uint16_t)(date->century * 100u + date->year);
}

static bool RTC_is_leap_year(uint16_t year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static uint8_t RTC_days_in_month(const RTC_Date *date) {
    switch (date->month) {
        case 1u:
        case 3u:
        case 5u:
        case 7u:
        case 8u:
        case 10u:
        case 12u:
            return 31u;

        case 4u:
        case 6u:
        case 9u:
        case 11u:
            return 30u;

        case 2u: {
            uint16_t full_year = RTC_full_year(date);
            return RTC_is_leap_year(full_year) ? 29u : 28u;
        }

        default:
            return 0u;
    }
}

static bool RTC_is_valid_date(const RTC_Date *date) {
    if (date == 0) {
        return false;
    }

    if (date->century > 99u) {
        return false;
    }
    if (date->year > 99u) {
        return false;
    }
    if (date->month > 12u || date->month < 1u) {
        return false;
    }
    
    uint8_t max_day = RTC_days_in_month(date); 
    if (date->day < 1u || date->day > max_day) {
        return false;
    }

    return true;
}

static uint32_t RTC_pack_time(const RTC_Time *time) {
    const uint8_t h_tens = RTC_bcd_tens(time->hours);
    const uint8_t h_units = RTC_bcd_units(time->hours);
    const uint8_t m_tens = RTC_bcd_tens(time->minutes);
    const uint8_t m_units = RTC_bcd_units(time->minutes);
    const uint8_t s_tens = RTC_bcd_tens(time->seconds);
    const uint8_t s_units = RTC_bcd_units(time->seconds);

    return RTC_FIELD_SET(s_units, RTC_TIME_S_S, RTC_TIME_S_M)   |
           RTC_FIELD_SET(s_tens, RTC_TIME_TS_S, RTC_TIME_TS_M)  |
           RTC_FIELD_SET(m_units, RTC_TIME_M_S, RTC_TIME_M_M)   |
           RTC_FIELD_SET(m_tens, RTC_TIME_TM_S, RTC_TIME_TM_M)  |
           RTC_FIELD_SET(h_units, RTC_TIME_H_S, RTC_TIME_H_M)   |
           RTC_FIELD_SET(h_tens, RTC_TIME_TH_S, RTC_TIME_TH_M)  |
           RTC_FIELD_SET(time->dow, RTC_TIME_DOW_S, RTC_TIME_DOW_M);
}

static uint32_t RTC_pack_date(const RTC_Date *date) {
    const uint8_t d_tens = RTC_bcd_tens(date->day);
    const uint8_t d_units = RTC_bcd_units(date->day);
    const uint8_t m_tens = RTC_bcd_tens(date->month);
    const uint8_t m_units = RTC_bcd_units(date->month);
    const uint8_t y_tens = RTC_bcd_tens(date->year);
    const uint8_t y_units = RTC_bcd_units(date->year);
    const uint8_t c_tens = RTC_bcd_tens(date->century);
    const uint8_t c_units = RTC_bcd_units(date->century);

    return RTC_FIELD_SET(d_units, RTC_DATE_D_S, RTC_DATE_D_M)   |
           RTC_FIELD_SET(d_tens, RTC_DATE_TD_S, RTC_DATE_TD_M)  |
           RTC_FIELD_SET(m_units, RTC_DATE_M_S, RTC_DATE_M_M)   |
           RTC_FIELD_SET(m_tens, RTC_DATE_TM_S, RTC_DATE_TM_M)  |
           RTC_FIELD_SET(y_units, RTC_DATE_Y_S, RTC_DATE_Y_M)   |
           RTC_FIELD_SET(y_tens, RTC_DATE_TY_S, RTC_DATE_TY_M)  |
           RTC_FIELD_SET(c_units, RTC_DATE_C_S, RTC_DATE_C_M)   |
           RTC_FIELD_SET(c_tens, RTC_DATE_TC_S, RTC_DATE_TC_M);
}

static void RTC_short_delay(void) {
    for (volatile uint32_t i = 0; i < 1000u; ++i) {
        ;
    }
}

RTC_Status RTC_set_time(const RTC_Time *time) {
    RTC_Date current_date;

    if (!RTC_is_valid_time(time)) {
        return RTC_DRIVER_ERROR;
    }

    if (RTC_get_date(&current_date) != RTC_DRIVER_OK) {
        return RTC_DRIVER_ERROR;
    }

    RTC_disable();
    RTC_short_delay();
    RTC_BASE->DATE = RTC_pack_date(&current_date);
    RTC_short_delay();

    RTC_BASE->TIME = RTC_pack_time(time);
    RTC_short_delay();
    
    RTC_enable();
    RTC_short_delay();

    return RTC_DRIVER_OK;
}

RTC_Status RTC_set_date(const RTC_Date *date) {
    RTC_Time current_time;

    if (!RTC_is_valid_date(date)) {
        return RTC_DRIVER_ERROR;
    }

    if (RTC_get_time(&current_time) != RTC_DRIVER_OK) {
        return RTC_DRIVER_ERROR;
    }

    RTC_disable();
    RTC_short_delay();
    
    RTC_BASE->DATE = RTC_pack_date(date);
    RTC_short_delay();

    RTC_BASE->TIME = RTC_pack_time(&current_time);
    RTC_short_delay();
    
    RTC_enable();
    RTC_short_delay();

    return RTC_DRIVER_OK;
}

void RTC_alarm_enable(void) {
    RTC_BASE->TALRM |= RTC_TALRM_CH_M | RTC_TALRM_CM_M | RTC_TALRM_CS_M;
}

void RTC_alarm_disable(void) {
    RTC_BASE->TALRM &= ~(RTC_TALRM_CH_M | RTC_TALRM_CM_M | RTC_TALRM_CS_M);
}

RTC_Status RTC_set_alarm_time(const RTC_Time *time) {
    if (!RTC_is_valid_time(time)) {
        return RTC_DRIVER_ERROR;
    }

    RTC_BASE->TALRM = RTC_pack_time(time);
    RTC_alarm_enable();

    return RTC_DRIVER_OK;
}

static void RTC_unpack_time(uint32_t reg, RTC_Time *time) {
    uint8_t s_units = (uint8_t)RTC_FIELD_GET(reg, RTC_TIME_S_S, RTC_TIME_S_M);
    uint8_t s_tens = (uint8_t)RTC_FIELD_GET(reg, RTC_TIME_TS_S, RTC_TIME_TS_M);
    uint8_t m_units = (uint8_t)RTC_FIELD_GET(reg, RTC_TIME_M_S, RTC_TIME_M_M);
    uint8_t m_tens = (uint8_t)RTC_FIELD_GET(reg, RTC_TIME_TM_S, RTC_TIME_TM_M);
    uint8_t h_units = (uint8_t)RTC_FIELD_GET(reg, RTC_TIME_H_S, RTC_TIME_H_M);
    uint8_t h_tens = (uint8_t)RTC_FIELD_GET(reg, RTC_TIME_TH_S, RTC_TIME_TH_M);
    uint8_t d_dow = (uint8_t)RTC_FIELD_GET(reg, RTC_TIME_DOW_S, RTC_TIME_DOW_M);

    time->seconds = (uint8_t)(s_tens * 10u + s_units);
    time->minutes = (uint8_t)(m_tens * 10u + m_units);
    time->hours = (uint8_t)(h_tens * 10u + h_units);
    time->dow = d_dow;
}

static void RTC_unpack_date(uint32_t reg, RTC_Date *date) {
    uint8_t d_units = (uint8_t)RTC_FIELD_GET(reg, RTC_DATE_D_S, RTC_DATE_D_M); 
    uint8_t d_tens = (uint8_t)RTC_FIELD_GET(reg, RTC_DATE_TD_S, RTC_DATE_TD_M);
    uint8_t m_units = (uint8_t)RTC_FIELD_GET(reg, RTC_DATE_M_S, RTC_DATE_M_M);
    uint8_t m_tens = (uint8_t)RTC_FIELD_GET(reg, RTC_DATE_TM_S, RTC_DATE_TM_M);
    uint8_t y_units = (uint8_t)RTC_FIELD_GET(reg, RTC_DATE_Y_S, RTC_DATE_Y_M);
    uint8_t y_tens = (uint8_t)RTC_FIELD_GET(reg, RTC_DATE_TY_S, RTC_DATE_TY_M);
    uint8_t c_units = (uint8_t)RTC_FIELD_GET(reg, RTC_DATE_C_S, RTC_DATE_C_M);
    uint8_t c_tens = (uint8_t)RTC_FIELD_GET(reg, RTC_DATE_TC_S, RTC_DATE_TC_M);

    date->day = (uint8_t)(d_tens * 10u + d_units);
    date->month = (uint8_t)(m_tens * 10u + m_units);
    date->year = (uint8_t)(y_tens * 10u + y_units);
    date->century = (uint8_t)(c_tens * 10u + c_units);
}

RTC_Status RTC_get_time(RTC_Time *time) {
    if (time == 0) {
        return RTC_DRIVER_ERROR;
    }

    RTC_unpack_time(RTC_BASE->TIME, time);
    return RTC_DRIVER_OK;
}

RTC_Status RTC_get_date(RTC_Date *date) {
    if (date == 0) {
        return RTC_DRIVER_ERROR;
    }

    RTC_unpack_date(RTC_BASE->DATE, date);
    return RTC_DRIVER_OK;
}

RTC_Status RTC_get_alarm_time(RTC_Time *time) {
    if (time == 0) {
        return RTC_DRIVER_ERROR;
    }

    RTC_unpack_time(RTC_BASE->TALRM, time);
    return RTC_DRIVER_OK;
}

void RTC_IRQHandler(void) {
    alarm_flag = 1u;
    RTC_BASE->CTRL &= ~RTC_CTRL_ALRM_M;
    RTC_BASE->CTRL |= RTC_CTRL_RESET_STROBE_M; 
}

uint32_t RTC_get_alarm_flag(void) {
    if (alarm_flag == 0u) {
        return 0u;
    }
    alarm_flag = 0u;
    return 1u;
}