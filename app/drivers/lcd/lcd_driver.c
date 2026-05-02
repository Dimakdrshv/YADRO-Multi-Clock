// init png in tg

#include "lcd_driver.h"
#include "drivers/i2c/i2c_driver.h"
#include "platform/mtimer.h"

#define LCD_RS (1u << 0u)
#define LCD_RW (1u << 1u)
#define LCD_E  (1u << 2u)
#define LCD_BL (1u << 3u)

#define LCD_DATA_SHIFT (4u)

static LCD_Status LCD_expander_write(uint8_t data) { // PCF8574
    if (I2C_write(data) != I2C_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    return LCD_DRIVER_OK;
}

static LCD_Status LCD_pulse_enable(uint8_t data) {
    if (LCD_expander_write(data) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }

    if (LCD_expander_write(data | LCD_E) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }

    if (LCD_expander_write(data) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }

    return LCD_DRIVER_OK;
}

static LCD_Status LCD_send_nibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = 0u;

    data |= ((nibble & 0x0Fu) << LCD_DATA_SHIFT);
    data |= LCD_BL;

    if (rs) {
        data |= LCD_RS;
    }

    return LCD_pulse_enable(data);
}

static LCD_Status LCD_send_byte(uint8_t byte, uint8_t rs) {
    if (LCD_send_nibble(byte >> 4, rs) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    if (LCD_send_nibble(byte & 0x0F, rs) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    return LCD_DRIVER_OK;
}

static LCD_Status LCD_send_command(uint8_t cmd) {
    return LCD_send_byte(cmd, 0u);
}

static LCD_Status LCD_send_data(uint8_t data) {
    return LCD_send_byte(data, 1u);
}

LCD_Status LCD_init(void) { // (HD44780)
    MTIMER_delay_ms(30u);

    if (LCD_send_nibble(0x03, 0) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_ms(5u);

    if (LCD_send_nibble(0x03, 0) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    if (LCD_send_nibble(0x03, 0) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    if (LCD_send_nibble(0x02, 0) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    if (LCD_send_command(0x28u) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    if (LCD_send_command(0x0Cu) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    if (LCD_send_command(0x01u) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_ms(2u);

    if (LCD_send_command(0x06u) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    if (LCD_send_command(0x80u) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    return LCD_DRIVER_OK;
}

LCD_Status LCD_clear(void) {
    if (LCD_send_command(0x01u) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_ms(2u);
    return LCD_DRIVER_OK;
}

LCD_Status LCD_home(void) {
    if (LCD_send_command(0x02u) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_ms(2u);
    return LCD_DRIVER_OK;
}

LCD_Status LCD_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = 0u;
    
    if (col > 15u) {
        return LCD_DRIVER_ERROR;
    }

    switch (row) {
        case 0u:
            address = 0x00u + col;
            break;
        case 1u:
            address = 0x40u + col;
            break;
        default: 
            return LCD_DRIVER_ERROR;
    }

    if (LCD_send_command(0x80u | address) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    return LCD_DRIVER_OK;
}

LCD_Status LCD_write_char(char c) {
    if (LCD_send_data((uint8_t)c) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    return LCD_DRIVER_OK;
}

LCD_Status LCD_write_string(const char *str) {
    if (str == 0) {
        return LCD_DRIVER_ERROR;
    }

    while (*str != '\0') {
        if (LCD_write_char(*str) != LCD_DRIVER_OK) {
            return LCD_DRIVER_ERROR;
        }
        ++str;
    }

    return LCD_DRIVER_OK;
}

LCD_Status LCD_display_on(void) {
    if (LCD_send_command(0x0Cu) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    return LCD_DRIVER_OK;
}

LCD_Status LCD_display_off(void) {
    if (LCD_send_command(0x08u) != LCD_DRIVER_OK) {
        return LCD_DRIVER_ERROR;
    }
    MTIMER_delay_us(100u);

    return LCD_DRIVER_OK;
}