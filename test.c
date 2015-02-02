#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/boot.h>
#include <util/delay.h>
#include "intcast.h"
#include "AVRootloader.h"

// Pinbelegung
// PD0 = RX = 1-Wire-RS232
// PC0 = PD0 -> damit PinChange funktioniert
// PD1 = TX = 2-Wire-RS232
// PE0 = LED nach GND, ist in AVRootlaoder.asm als RS485 DE-Pin benutzt und zeigt Kommunikation mit dem Bootloader an

uint8_t _MCUCSR;  // Kopie vom MCUCSR Register, gespeichert vom Bootloader bei RAMEND

register uint8_t flags asm("r2");

ISR(PCINT1_vect) {
	flags <<= 1;
	flags |= PIND & (1 << PD0);
	if (flags == 0xAA) {
		bootloader_start_wdt();  // starte Bootloader mit Watchdog
//		bootloader_start_jump(); // starte Bootloader mit einem Sprung
	}
}

static void test(void) {
// Liest/berechnet verschiedene Werte aus dem Bootloader und speichert sie ins EEPROM.
// Demonstriert die UseSpecialWrite, UseSpecialRead, UseSpecialMsg Funktionen des Bootloaders.
// Das passende AVRootloader.asm Projekt findet sich im Unterordner \test\AVR\.
// Mit der Bootloader-PC-Software kann man den EEPROM dann auslesen.
// Fuses sind so gesetzt das LPM aus der Applikation Section nicht die Bootloader Section auslesen kann.

// lese BootMsg + BootInfo
	uint16_t size = bootmsg_size() +4;
	uint16_t addr = bootmsg_addr();
	uint8_t buf[size];
	read_flash(addr, size, buf);
	eeprom_write_block(buf, 0, size);

// lese version
	uint8_t* eepr = (uint8_t*)size;
	uint32_t version = get_appversion();
	eeprom_write_byte(eepr++, HH8(version));
	eeprom_write_byte(eepr++, HL8(version));
	eeprom_write_byte(eepr++, LH8(version));
	eeprom_write_byte(eepr++, LL8(version));

// programmiere in der Bootloader Section die 16 bytes nach der Signature BootSign um
	uint8_t data[16];
	for (uint8_t i = 0; i < 16; i++)
		data[i] = i;
	addr += size;
	write_flash_boot(addr, 16, data);
	for (uint8_t i = 0; i < 16; i++)
		data[i] = ~i;
// nun lese diese bytes und ins eeprom damit
	read_flash(addr, 16, data);
	eeprom_write_block(data, eepr, 16);
}

void init_WDT(void) __attribute__((naked)) __attribute__((section(".init3")));
void init_WDT(void) {
// Watchdog deaktivieren, MCUCSR wurde schon im Bootloader zurück gesetzt
	WDTCR = (1 << WDE) | (1 << WDCE);
	WDTCR = 0;
}

void init_ports(void) __attribute__((naked)) __attribute__((section(".init5")));
void init_ports(void) {
// lese Kopie von MCUCSR gespeichert durch Bootloader vom Stack
	_MCUCSR = *(uint8_t*)(RAMEND);

	DDRA   = 0;
	DDRB   = 0;
	DDRC   = 0;
	DDRD   = 0;
	DDRE   = (1 << PE0);

	PORTA  = 0xFF;
	PORTB  = 0xFF;
	PORTC  = 0xFF & ~(1 << PC0);
	PORTD  = 0xFF;
	PORTE  = 0xFF;

	PCMSK1 = (1 << PCINT8);
	GICR   = (1 << PCIE1);
	MCUCR  = (1 << SE) | (1 << SM1);
	MCUCSR = (0 << SM2);
	EMCUCR = (0 << SM0);

	flags  = 0;
}

int main(void) {
	sei();
	test();
	PORTE &= ~(1 << PE0);

	while (1) {
		sleep_cpu();
	}
}
