#include "bootloader_external_wdt.h"

#include "stdint.h" // Must be included before gpio_struct.h

#include "rom/gpio.h"
#include "soc/gpio_sig_map.h"
#include "soc/io_mux_reg.h"

// TODO: pull this from menuconfig
#define MYSA_PRODUCT_IS_INF 1

/*
 * Define the pin to be used for the Watchdog timer.
 * IMPORTANT: All three of these values must match the specific GPIO pin that you wish to use.
 */
#if MYSA_PRODUCT_IS_INF
#define BOOTLOADER_EXTERNAL_WDT_GPIO_NUM 32
#define BOOTLOADER_EXTERNAL_WDT_GPIO_REG PERIPHS_IO_MUX_GPIO32_U
#define BOOTLOADER_EXTERNAL_WDT_GPIO_FUNC FUNC_GPIO32_GPIO32
#else
#define BOOTLOADER_EXTERNAL_WDT_GPIO_NUM 26
#define BOOTLOADER_EXTERNAL_WDT_GPIO_REG PERIPHS_IO_MUX_GPIO26_U
#define BOOTLOADER_EXTERNAL_WDT_GPIO_FUNC FUNC_GPIO26_GPIO26
#endif

#define BOOTLOADER_EXTERNAL_WDT_MAX 40

void bootloader_external_wdt_init() {
    /* Use pin as a GPIO */
    PIN_FUNC_SELECT(BOOTLOADER_EXTERNAL_WDT_GPIO_REG, BOOTLOADER_EXTERNAL_WDT_GPIO_FUNC);

    /* Make sure the pin is not set up as an input pin */
    PIN_INPUT_DISABLE(BOOTLOADER_EXTERNAL_WDT_GPIO_REG);

    /* bind pin as an output GPIO in the IO matrix */
    gpio_matrix_out(BOOTLOADER_EXTERNAL_WDT_GPIO_NUM, SIG_GPIO_OUT_IDX, 0, 0);
}

void bootloader_external_wdt_toggle() {
    static uint8_t max = BOOTLOADER_EXTERNAL_WDT_MAX;
    if(max) {
        max--;
        static uint8_t current = 0;
        current = (uint8_t) !current;

        /*
         * Read in the current value if the GPIO out reg and set/clear the GPIO pin.
         * IMPORTANT: This is not thread safe so DO NOT use outside of bootloader.
         */
#if BOOTLOADER_EXTERNAL_WDT_GPIO_NUM < 32
        uint32_t out_reg = REG_READ(GPIO_OUT_REG);
        if (current) {
            REG_WRITE(GPIO_OUT_REG, out_reg | BIT(BOOTLOADER_EXTERNAL_WDT_GPIO_NUM)); // Set GPIO26 ON
        } else {
            REG_WRITE(GPIO_OUT_REG, out_reg & ~BIT(BOOTLOADER_EXTERNAL_WDT_GPIO_NUM)); // Set GPIO26 OFF
        }

#else // GPIO >= 32 are handled by a different register
        uint32_t out_reg = REG_READ(GPIO_OUT1_REG);
        if (current) {
            REG_WRITE(GPIO_OUT1_REG, out_reg | BIT(BOOTLOADER_EXTERNAL_WDT_GPIO_NUM - 32)); // Set GPIO ON
        } else {
            REG_WRITE(GPIO_OUT1_REG, out_reg & ~BIT(BOOTLOADER_EXTERNAL_WDT_GPIO_NUM - 32)); // Set GPIO OFF
        }
#endif
    }
}
