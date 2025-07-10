#include <inttypes.h>
#include <stdbool.h>

#include "..\driver\rcc.h"
#include "..\driver\gpio.h"

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

int main(void) {
  uint16_t led = PIN('A', 5);            // Blue LED
  RCC->APB2ENR |= BIT(PINBANK(led) + 2);     // Enable GPIO clock for LED
  gpio_set_mode(led, GEN_PUSH_PULL_OUTPUT);  // Set blue LED to output mode
  for (;;) {
    gpio_write(led, true);
    spin(800000);
    gpio_write(led, false);
    spin(800000);
  }
  return 0;
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
  __asm__("ldr sp, =_estack");

  // memset .bss to zero, and copy .data section to RAM region
  extern long _sbss, _ebss, _sdata, _edata, _sidata;
  for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
  for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

  main();             // Call main()
  for (;;) (void) 0;  // Infinite loop in the case if main() returns
}

extern void _estack(void);  // Defined in link.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
