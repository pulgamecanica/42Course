#include "pulga/utils.h"
#include "pulga/uart.h"
#include "pulga/builtin_rgb.h"

int main(void) {
  uart_init();
  init_rgb();

  uart_puts("Pulga RGB UART Ready. Send colors as #RRGGBB\n");

  char buffer[16];
  while (1) {
    uart_puts("> ");
    uart_get_input(buffer, sizeof(buffer));

    // Must be "#RRGGBB" (7 chars total)
    uart_puts("Read(");
    uart_putnum(ft_strlen(buffer));
    uart_puts(")\n");
    if (ft_strlen(buffer) != 7 || buffer[0] != '#') {
      uart_puts("Invalid format. Use #RRGGBB\n");
      continue;
    }

    // Validate all hex digits
    int valid = 1;
    for (int i = 1; i < 7; ++i) {
      if (hex_char_to_val(buffer[i]) == 255) {
        valid = 0;
        break;
      }
    }
    if (!valid) {
      uart_puts("Invalid hex digits. Use 0–9 or A–F.\n");
      continue;
    }

    uint8_t r = hex_pair_to_byte(buffer[1], buffer[2]);
    uint8_t g = hex_pair_to_byte(buffer[3], buffer[4]);
    uint8_t b = hex_pair_to_byte(buffer[5], buffer[6]);

    set_rgb(r, g, b);

    uart_puts("Color set to ");
    uart_puts(buffer);
    uart_puts(" (");
    uart_puts("R:"); uart_putnum(r);
    uart_puts(", G:"); uart_putnum(g);
    uart_puts(", B:"); uart_putnum(b);
    uart_puts(")\n");
  }
}
