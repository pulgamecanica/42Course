# **************************************************************************** #
#                                PROJECT INFO                                  #
# **************************************************************************** #

PROJ_NAME   := ex00-PiscineEmbeded03
NAME_BIN    := main.bin
NAME_HEX    := main.hex

# **************************************************************************** #
#                                  SETTINGS                                    #
# **************************************************************************** #

# ────────────────────────────── MCU / CLOCK / SERIAL ─────────────────────────
MCU         := atmega328p
F_CPU       := 16000000
SERIAL_PORT := /dev/ttyUSB0
#9600#115200
BAUD_RATE   := 115200

# ──────────────────────────────── BUILD PATHS ────────────────────────────────
SRC         := main.c
BIN_DIR     := bin
OBJ         := $(SRC:%.c=$(BIN_DIR)/%.o)

# ──────────────────────────────── PROGRAMMER ─────────────────────────────────
PROGRAMMER  := arduino
OBJ_FORMAT  := elf32-avr
HEX_FORMAT  := ihex

# **************************************************************************** #
#                                 TOOLCHAIN                                    #
# **************************************************************************** #

CC          := avr-gcc
OBJCPY      := avr-objcopy
TRANSFER    := avrdude
RM          := rm -rf

# **************************************************************************** #
#                                   FLAGS                                      #
# **************************************************************************** #

CPPFLAGS    := -DF_CPU=$(F_CPU)UL -DBAUD_RATE=$(BAUD_RATE)UL
CFLAGS      := -Wall -Werror -Wextra -Wundef \
                -Og -ggdb -std=gnu99 -pedantic \
                -funsigned-char -funsigned-bitfields \
                -fpack-struct -fshort-enums \
                -ffunction-sections -fdata-sections \
                -mmcu=$(MCU) $(CPPFLAGS)
LDFLAGS     := -Wl,--gc-sections
CFLAGS      += -I..

# **************************************************************************** #
#                                   COLORS                                     #
# **************************************************************************** #

C_BLACK     = \033[0;30m
C_RED       = \033[0;31m
C_GREEN     = \033[0;32m
C_YELLOW    = \033[0;33m
C_BLUE      = \033[0;34m
C_MAGENTA   = \033[0;35m
C_CYAN      = \033[0;36m
C_WHITE     = \033[0;37m
C_END       = \033[0m

# **************************************************************************** #
#                                   TARGETS                                    #
# **************************************************************************** #

all: hex flash

# -----------------------------------------------------------------------------
# Compile sources to object files
# -----------------------------------------------------------------------------
$(BIN_DIR)/%.o: %.c
	@mkdir -p $(BIN_DIR)
	@printf "$(C_BLUE)[1/4]$(C_END) $(C_CYAN)Compiling:$(C_END) $< --> $@\n"
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------------------------------------------------------------
# Link ELF and convert to HEX
# -----------------------------------------------------------------------------
$(BIN_DIR)/$(NAME_HEX): $(OBJ)
	@printf "$(C_BLUE)[2/4]$(C_END) $(C_YELLOW)Linking ELF:$(C_END) $(BIN_DIR)/$(NAME_BIN)\n"
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(BIN_DIR)/$(NAME_BIN)
	@printf "$(C_BLUE)[3/4]$(C_END) $(C_YELLOW)Generating HEX:$(C_END) $(BIN_DIR)/$(NAME_HEX)\n"
	$(OBJCPY) -O $(HEX_FORMAT) -I $(OBJ_FORMAT) $(BIN_DIR)/$(NAME_BIN) $@

# -----------------------------------------------------------------------------
# Build HEX only
# -----------------------------------------------------------------------------
hex: $(BIN_DIR)/$(NAME_HEX)
	@printf "$(C_BLUE)[✔]$(C_END) $(C_GREEN)HEX file ready:$(C_END) $(C_YELLOW)$(BIN_DIR)/$(NAME_HEX)$(C_END)\n"

# -----------------------------------------------------------------------------
# Flash to device
# -----------------------------------------------------------------------------
flash: $(BIN_DIR)/$(NAME_HEX)
	@printf "$(C_BLUE)[4/4]$(C_END) $(C_GREEN)Flashing device via avrdude$(C_END)\n"
	@printf "  Programmer : $(PROGRAMMER)\n"
	@printf "  MCU        : $(MCU)\n"
	@printf "  Serial     : $(SERIAL_PORT)\n"
	@printf "  Baud Rate  : $(BAUD_RATE)\n"
	@printf "  File       : $(BIN_DIR)/$(NAME_HEX)\n"
	@if [ ! -e "$(SERIAL_PORT)" ]; then \
		printf "$(C_RED)⚠  Warning: $(SERIAL_PORT) not found!$(C_END)\n"; \
	fi
	$(TRANSFER) -c $(PROGRAMMER) -p $(MCU) -F -V -P $(SERIAL_PORT) \
		-b $(BAUD_RATE) -U flash:w:$(BIN_DIR)/$(NAME_HEX)

# -----------------------------------------------------------------------------
# Cleaning rules
# -----------------------------------------------------------------------------
clean:
	@printf "$(C_RED)Cleaning build files...$(C_END)\n"
	$(RM) $(BIN_DIR)/*.o $(BIN_DIR)/$(NAME_BIN) $(BIN_DIR)/$(NAME_HEX)

fclean: clean
	@printf "$(C_RED)Removing build directory...$(C_END)\n"
	$(RM) $(BIN_DIR)

re: fclean all

# -----------------------------------------------------------------------------
# Open a terminal connected to the UART ATmega328p Serial Port
# -----------------------------------------------------------------------------
# https://stackoverflow.com/questions/41266001/screen-dev-ttyusb0-with-different-options-such-as-databit-parity-etc
screen:
	screen /dev/ttyUSB0 $(BAUD_RATE)

# -----------------------------------------------------------------------------
# System information and diagnostics
# -----------------------------------------------------------------------------
show:
	@printf "\n$(C_MAGENTA)Project Info$(C_END)\n"
	@printf "  PROJ_NAME  : $(C_YELLOW)$(PROJ_NAME)$(C_END)\n"
	@printf "  MCU        : $(C_YELLOW)$(MCU)$(C_END)\n"
	@printf "  F_CPU      : $(C_YELLOW)$(F_CPU)$(C_END)\n"
	@printf "  PROGRAMMER : $(C_YELLOW)$(PROGRAMMER)$(C_END)\n"
	@printf "  BAUD_RATE  : $(C_YELLOW)$(BAUD_RATE)$(C_END)\n"
	@printf "  SERIAL     : $(C_YELLOW)$(SERIAL_PORT)$(C_END)\n"
	@printf "  SOURCES    : $(C_GREEN)$(SRC)$(C_END)\n"
	@printf "  OBJECTS    : $(C_GREEN)$(OBJ)$(C_END)\n"
	@printf "  CC         : $(CC)\n"
	@printf "  CFLAGS     : $(C_CYAN)$(CFLAGS)$(C_END)\n"
	@printf "\n$(C_MAGENTA)Connected Serial Devices$(C_END)\n"
	@if [ -d /dev/serial/by-path ]; then \
		ls --color=yes -l /dev/serial/by-path; \
	else \
		printf "$(C_RED)/dev/serial/by-path not found$(C_END)\n"; \
	fi
	@printf "\n"

# -----------------------------------------------------------------------------
# Phony targets
# -----------------------------------------------------------------------------
.PHONY: all clean fclean re flash hex show screen
