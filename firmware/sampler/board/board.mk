# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_STM32L4_IOT_NODE/board.c

# Required include directories
BOARDINC = $(CHIBIOS)/os/hal/boards/ST_STM32L4_IOT_NODE

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
