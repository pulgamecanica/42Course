#include <util/delay.h>
#include "pulga/utils.h"
#include "pulga/uart.h"
#include "pulga/timer.h"
#include "pulga/pins.h"

#define LED PIN_B1
#define SUCCESS 1
#define FAILURE 0

const char *USERNAME = "admin";
const char *PASSWORD = "secret";

int try_login() {
  char user[42], pass[42];

  uart_puts("=== Welcome to Pulga(UART) Login ===\n");

  uart_puts("Username: ");
  uart_get_input(user, sizeof(user));

  uart_puts("Password: ");
  uart_get_input_secret(pass, sizeof(pass));

  if (!ft_strcmp(user, USERNAME) && !ft_strcmp(pass, PASSWORD)) {
    uart_puts("\r\nAccess granted!\n");

    // Dramatic effect: blink the LED like crazy
    for (uint8_t i = 0; i < 6; i++) {
      togglePin(LED);
      _delay_ms(300);
    }
    return SUCCESS;
  } else {
    uart_puts("\r\nAccess denied. Wrong credentials.\n");
    return FAILURE;
  }
}

void	play_game(void) {
  char	input[64];
  int		bye_count = 0;

  uart_puts("Granny: Hello dear! Talk to me.\n");

  while (1) {
    uart_puts("> ");
    uart_get_input(input, sizeof(input));

    // Convert to lowercase version for matching
    char lower[64];
    int len = ft_strlen(input);
    for (int i = 0; i < len; i++)
      lower[i] = ft_tolower((unsigned char)input[i]);
    lower[len] = '\0';

    // Check for "bye"
    if (ft_strstr(lower, "bye")) {
      bye_count++;
    if (bye_count == 1)
      uart_puts("Granny: Oh… leaving already?\n");
    else if (bye_count == 2)
      uart_puts("Granny: You said that already, dear…\n");
    else {
      uart_puts("Granny: Fine. Goodbye, sweetheart.\n");
      break;
    }
    continue;
  }

  bye_count = 0;

  int has_upper = 0;
  int has_lower = 0;
  int has_excl = 0;

  for (int i = 0; i < len; i++) {
    if (ft_isupper(input[i])) has_upper = 1;
    if (ft_islower(input[i])) has_lower = 1;
    if (input[i] == '!') has_excl = 1;
  }

  if (has_excl)
    uart_puts("Granny: You don’t talk like that to your granny, lil’ shit!\n");
  else if (has_upper && !has_lower)
    uart_puts("Granny: Hey! Don’t scream at me!\n");
  else if (!has_lower)
    uart_puts("Granny: Huh!? What? No, not since 1944!\n");
  else
    uart_puts("Granny: Oh that’s nice dear.\n");
  }

  uart_puts("Granny: Zzz...\n");
}

int main(void) {
  int is_authenticated = FAILURE;
  uart_init();
  setPinMode(LED, OUTPUT);

  while (42) {
    if (is_authenticated == FAILURE)
      is_authenticated = try_login();
    if (is_authenticated == SUCCESS) {
      play_game();
      is_authenticated = FAILURE;
    }
  }
  return 0;
}
