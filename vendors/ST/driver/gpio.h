#include <inttypes.h>

struct gpio {
  volatile uint32_t CRL, CRH, IDR, ODR, BSRR, BRR, LCKR;
};
#define GPIO(bank) ((struct gpio *) (0x40010800 + 0x400 * (bank))) // GPIO(A) base address

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

typedef enum{
GEN_PUSH_PULL_OUTPUT = 0b0011,
GEN_OPEN_DRAIN_OUTPUT = 0b0111,
ALT_PUSH_PULL_OUTPUT = 0b1011,
ALT_OPEN_DRAIN_OUTPUT = 0b1111,
ANALOG_INPUT = 0b0000,
FLOATING_INPUT = 0b0100,
PULL_DOWN_INPUT = 0b1000,
PULL_UP_INPUT = 0b1000,
INPUT_MODE = 0b0100
}pin_mode_t;

static inline void gpio_set_mode(uint16_t pin, pin_mode_t mode);
static inline void gpio_write(uint16_t pin, bool val);

static inline void gpio_set_mode(uint16_t pin, pin_mode_t mode) {
  struct gpio *gpio = GPIO(PINBANK(pin));  // GPIO bank
  int n = PINNO(pin);                      // Pin number
  if (n < 8) {
    gpio->CRL &= ~(0xFU << (n * 4));       // Clear existing setting in CRL
    gpio->CRL |= (mode & 0xFU) << (n * 4); // Set new mode in CRL
  } else {
    gpio->CRH &= ~(0xFU << ((n - 8) * 4)); // Clear existing setting in CRH
    gpio->CRH |= (mode & 0xFU) << ((n - 8) * 4); // Set new mode in CRH
  }
}

static inline void gpio_write(uint16_t pin, bool val) {
  struct gpio *gpio = GPIO(PINBANK(pin));
  gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}