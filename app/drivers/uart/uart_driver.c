#include "uart_driver.h"
#include <uart.h>
#include "platform/epic_irq.h"
#include "platform/clock_control.h"
#include "platform/io_mapping.h"
#include "platform/pad_control.h"

#define UART_BAUDRATE (57600u)

#define UART_DIVIDER_MIN (16u)
#define UART_DIVIDER_MAX (0xFFFFu)

#define UART_INIT_TIMEOUT (1000000u)
#define UART_TRANSMIT_TIMEOUT (100000u)

#define UART_CLEARABLE_FLAG_MASK ( \
        UART_FLAGS_CTSIF_M | \
        UART_FLAGS_LBDF_M  | \
        UART_FLAGS_TXE_M   | \
        UART_FLAGS_TC_M    | \
        UART_FLAGS_RXNE_M  | \
        UART_FLAGS_IDLE_M  | \
        UART_FLAGS_ORE_M   | \
        UART_FLAGS_NF_M    | \
        UART_FLAGS_FE_M    | \
        UART_FLAGS_PE_M      \
    )

typedef struct __uart_rx_data {
    uint8_t buf_pointer;
    char rxdata_buf[RXDATA_BUF_SIZE];
    bool buf_ready; // reset from service
    bool overflow; // reset from service
    bool discard_until_nl;
} UART_RX_Data;

static UART_RX_Data uart_rx_data =  {    
                                        .buf_pointer = 0, 
                                        .overflow = false,
                                        .buf_ready = false,
                                        .discard_until_nl = false
                                    };

static bool UART_validate_divider(uint32_t *divider) {
    *divider = Clock_get_UART0_FREQ() / UART_BAUDRATE;
    if (*divider < UART_DIVIDER_MIN) {
        return false;
    }
    if (*divider > UART_DIVIDER_MAX) {
        return false;
    }
    return true;
}

static void UART_reset_peripheral(void) {
    UART_0_BASE->CONTROL1 = 0u;
    UART_0_BASE->CONTROL2 = 0u;
    UART_0_BASE->CONTROL3 = 0u;
    UART_0_BASE->MODEM = 0u;
}

static void UART_clear_flags(void) {
    UART_0_BASE->FLAGS = UART_CLEARABLE_FLAG_MASK;
}

static void UART_load_config(uint32_t divider) {
    UART_0_BASE->CONTROL1 = UART_CONTROL1_RXNEIE_M | UART_CONTROL1_TE_M | UART_CONTROL1_RE_M;
    UART_0_BASE->CONTROL3 = UART_CONTROL3_OVRDIS_M;
    
    UART_0_BASE->DIVIDER = divider;
    
    UART_0_BASE->MODEM = UART_MODEM_DTR_M;
}

static void UART_enable(void) {
    UART_0_BASE->CONTROL1 |= UART_CONTROL1_UE_M;
}

static bool UART_wait_flag(uint32_t timeout, uint32_t flag_mask) {
    while (timeout) {
        if (UART_0_BASE->FLAGS & flag_mask) {
            return true;
        }
        --timeout;
    }
    return false;
}

UART_Status UART_init(void) {

    uint32_t divider;
    if (!UART_validate_divider(&divider)) {
        return UART_DRIVER_ERROR;
    }

    Clock_enable_UART0();
    PAD_UART0_settings();
    
    UART_reset_peripheral();
    UART_clear_flags();
    UART_load_config(divider);
    UART_enable();

    if (!UART_wait_flag(UART_INIT_TIMEOUT, UART_FLAGS_TEACK_M)) {
        return UART_DRIVER_ERROR;
    }
    if (!UART_wait_flag(UART_INIT_TIMEOUT, UART_FLAGS_REACK_M)) {
        return UART_DRIVER_ERROR;
    }

    EPIC_IRQ_enable(UART_0_IRQ_INDEX);

    return UART_DRIVER_OK;
}

static void UART_Receive(void) {
    char data = (char)(UART_0_BASE->RXDATA & 0xFF);

    if (uart_rx_data.discard_until_nl) {
        if (data == '\n') {
            uart_rx_data.discard_until_nl = false;
        }
        return;
    }

    if (uart_rx_data.buf_ready || uart_rx_data.overflow) {
        return;
    }

    if (data == '\n') {
        uart_rx_data.rxdata_buf[uart_rx_data.buf_pointer] = '\0';
        uart_rx_data.buf_pointer = 0;
        uart_rx_data.buf_ready = true;
    }

    else {
        if (uart_rx_data.buf_pointer >= RXDATA_BUF_SIZE - 1) {
            uart_rx_data.buf_pointer = 0;
            uart_rx_data.overflow = true;
            uart_rx_data.discard_until_nl = true;
        }
        else {
            uart_rx_data.rxdata_buf[uart_rx_data.buf_pointer++] = data;
        }
    }
}

void UART_Transmit(const char * src) {
    if (src == 0) {
        return;
    }

    while (*src != '\0') {
        UART_0_BASE->FLAGS = UART_FLAGS_TC_M;
        UART_0_BASE->TXDATA = (uint32_t)(*src++);

        if (!UART_wait_flag(UART_TRANSMIT_TIMEOUT, UART_FLAGS_TC_M)) {
            return;
        }
    }

    return;
}

bool UART_isCommandReady(void) {
    return uart_rx_data.buf_ready;
}

bool UART_isOverflow(void) {
    return uart_rx_data.overflow;
}

void UART_ClearCommandReady(void) {
    uart_rx_data.buf_ready = false;
}

void UART_ClearOverflow(void) {
    uart_rx_data.overflow = false;
}

bool UART_GetCommand(char * dst) {
    if (dst == 0) {
        return false;
    }
    uint32_t i = 0u;
    while ((i < RXDATA_BUF_SIZE - 1u) && (uart_rx_data.rxdata_buf[i] != '\0')) {
        dst[i] = uart_rx_data.rxdata_buf[i];
        ++i;
    }
    dst[i] = '\0';
    return true;
}

void UART_IRQHandler(void) {
    if (UART_0_BASE->FLAGS & UART_FLAGS_RXNE_M) {
        UART_Receive();
    }
}