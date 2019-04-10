// uses USART2,  (PA2 / PA3)
#define PLATFORM_CONSOLE_UART ( 1 ) // PLATFORM_PARAM(PLATFORM_CONSOLE_UART     "1"      STRING "The UART port used by the console UART configuration."   )
#define PLATFORM_CONSOLE_BAUDRATE ( 115200 ) // PLATFORM_PARAM(PLATFORM_CONSOLE_BAUDRATE "115200" STRING "The baudrate used by the second console configuration."       )
#define PLATFORM_CONSOLE_LOCATION ( 0 ) // PLATFORM_PARAM(PLATFORM_CONSOLE_LOCATION "0" STRING "The location") # TODO remove after removing the pins param from the API

#define PLATFORM_MODEM_INTERFACE_UART ( 0 ) // PLATFORM_PARAM(PLATFORM_MODEM_INTERFACE_UART     "0"      STRING "The UART channel used by the modem interface UART configuration."   )
#define PLATFORM_MODEM_INTERFACE_BAUDRATE ( 115200 ) // PLATFORM_PARAM(PLATFORM_MODEM_INTERFACE_BAUDRATE "115200" STRING "The baudrate used by the modem interface configuration."       )

#define PLATFORM_NUM_LEDS ( 4 ) // PLATFORM_PARAM(PLATFORM_NUM_LEDS "4" STRING "The number of LEDs.")
#define PLATFORM_NUM_BUTTONS ( 1 ) // PLATFORM_PARAM(PLATFORM_NUM_BUTTONS "1" STRING "The number of buttons.")
#define PLATFORM_NUM_DEBUGPINS ( 2 ) // PLATFORM_PARAM(PLATFORM_NUM_DEBUGPINS "2" STRING "The number of debugpins available.")