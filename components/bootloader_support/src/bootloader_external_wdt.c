#include "bootloader_external_wdt.h"

#include "stdint.h" // Must be included before gpio_struct.h

#include "soc/gpio_struct.h"
#include "soc/io_mux_reg.h"

void bootloader_external_wdt_init() {
    uint32_t pin_function = 0;

    GPIO.enable_w1ts = ((uint32_t) 1 << BOOTLOADER_EXTERNAL_WDT_GPIO);

    pin_function |= ((uint32_t) 2 << FUN_DRV_S);  //what are the drivers?
    pin_function |= FUN_IE;  //input enable but required for output as well?

    pin_function |= ((uint32_t) (0x02 >> 5) << MCU_SEL_S); // 0x02 is OUTPUT

    // WARNING: GPIO cannot be changed without modifying this register first.
    *((volatile uint32_t *)(DR_REG_IO_MUX_BASE + 0x28)) = pin_function;

    GPIO.pin[BOOTLOADER_EXTERNAL_WDT_GPIO].val = 0;
}

void bootloader_external_wdt_toggle() {
    static uint8_t max = BOOTLOADER_EXTERNAL_WDT_MAX;
    if(max) {
        max--;
        static uint8_t current = 0;
        current = (uint8_t) !current;
        if (current) {
            GPIO.out_w1ts = ((uint32_t) 1 << BOOTLOADER_EXTERNAL_WDT_GPIO);
        } else {
            GPIO.out_w1tc = ((uint32_t) 1 << BOOTLOADER_EXTERNAL_WDT_GPIO);
        }
   }
}