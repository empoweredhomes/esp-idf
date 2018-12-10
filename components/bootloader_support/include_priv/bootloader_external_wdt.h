#ifndef __BOOTLOADER_EXTERNAL_WDT_H
#define __BOOTLOADER_EXTERNAL_WDT_H

/**
 * @file bootloader_external_wdt.h
 * @brief Custom library for feeding an External Watchdog Timer during boot.
 *
 * This is a custom library created at Mysa to handle feeding the External Watchdog connected to
 * many Mysa devices. This timer expires in 1.2 seconds, and therefore must be fed during the ESP boot
 * process otherwise the device will be infinitely reset.
 *
 */

/**
 * @brief Initialize the GPIO pin for the external Watchdog Timer.
 *
 * This pin is Hardcoded to BOOTLOADER_EXTERNAL_WDT_GPIO. This may change in the future, but does not need to be
 * handled at runtime since bootloaders are as immutable as the hardware.
 */
void bootloader_external_wdt_init();

/**
 * @brief Toggle the GPIO connected to the external Watchdog Timer.
 *
 * This will change the pin from low to high, or from high to low respectively. If the watchdog timer requires a pulse,
 * you must call this fuction multiple times accordingly. The first call of this function will init the pin high.
 *
 * There is also an enforced max feed of BOOTLOADER_EXTERNAL_WDT_MAX in this function. This means that you will only be
 * able to feed the timer BOOTLOADER_EXTERNAL_WDT_MAX times during the boot process. This is to prevent a rogue loop
 * from hammering the pin and preventing reset.
 */
void bootloader_external_wdt_toggle();

#endif