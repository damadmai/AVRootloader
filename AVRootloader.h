#ifndef AVRootloader_H
#define AVRootloader_H

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include "intcast.h"

// start bootloader with direct jump
static inline void bootloader_start_jump(void) __attribute__((always_inline, noreturn));
// start bootloader trough watchdog reset
static inline void bootloader_start_wdt(void) __attribute__((always_inline, noreturn));
// retrieve size in bytes of bootmsg
static inline uint16_t bootmsg_size(void) __attribute__((always_inline));
// retrieve flash address of bootmsg
static inline uint32_t bootmsg_addr(void) __attribute__((always_inline));
// retrieve application version number, if not supported return 0xFFFFFFFF
static inline uint32_t get_appversion(void) __attribute__((always_inline));
// read FLASH from address, size bytes into SRAM buffer
static inline void read_flash(uint32_t address, uint16_t size, const uint8_t* buffer) __attribute__((always_inline));
// write FLASH to address, size bytes from SRAM buffer, there is no limitation for alignment address and size
static inline void write_flash(uint32_t address, uint16_t size, const uint8_t* buffer) __attribute__((always_inline));
// write FLASH, but we can write into bootloader section if SPM is not locked by lockbi fuses
static inline void write_flash_boot(uint32_t address, uint16_t size, const uint8_t* buffer) __attribute__((always_inline));
// execute SPM opcode
static inline void dospm(uint8_t command) __attribute__((always_inline));

#define GETBOOTMSG_VEC      FLASHEND -9
#define READFLASH_VEC       FLASHEND -7
#define WRITEFLASH_VEC      FLASHEND -5
#define DOSPM_VEC           FLASHEND -3
#define BOOTSTART_VEC       FLASHEND -1


void bootloader_start_jump(void) {

    asm volatile("jmp -2");
    for (;;);
}

void bootloader_start_wdt(void) {

    wdt_reset();
    _WD_CONTROL_REG = (1 << WDE) | (1 << _WD_CHANGE_BIT);
    _WD_CONTROL_REG = (1 << WDE);
    for (;;);
}

uint16_t bootmsg_size(void) {

    uint16_t (*proc)(void) = (void*)GETBOOTMSG_VEC;
    return H8(proc());
}

uint32_t bootmsg_addr(void) {

    uint32_t (*proc)(void) = (void*)GETBOOTMSG_VEC;
    uint32_t res = proc();
    asm volatile("clr %D0" : "+&r" (res) : "r" (res));
    return res;
}

uint32_t get_appversion(void) {

    uint8_t pagecount;
    read_flash(bootmsg_addr() + bootmsg_size() + 3, 1, &pagecount);
    return pgm_read_dword((FLASHEND - 3) - SPM_PAGESIZE * pagecount);
}

void read_flash(uint32_t address, uint16_t size, const uint8_t* buffer) {

    void (*proc)(uint32_t address, uint16_t size, const uint8_t* buffer) = (void*)READFLASH_VEC;
    proc(address, size, buffer);
}

void write_flash(uint32_t address, uint16_t size, const uint8_t* buffer) {

    void (*proc)(uint32_t address, uint16_t size, const uint8_t* buffer) = (void*)WRITEFLASH_VEC;
    proc(address, size, buffer);
}

void write_flash_boot(uint32_t address, uint16_t size, const uint8_t* buffer) {

    void (*proc)(uint32_t address, uint16_t size, const uint8_t* buffer) = (void*)WRITEFLASH_VEC;
    asm volatile("ldi %D0, 0xAC" : "+&r" (address) : "r" (address));  // magic code, address |= 0xAC000000
    proc(address, size, buffer);
}

void dospm(uint8_t command) {

    void (*proc)(uint8_t command) = (void*)DOSPM_VEC;
    proc(command);
}
#endif
