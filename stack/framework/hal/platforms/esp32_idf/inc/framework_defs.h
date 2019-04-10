//Add framework options && import them in the framework_defs.h file

#define TRUE  (1)
#define FALSE (0)

#define FRAMEWORK_SCHEDULER_MAX_TASKS ( 32 ) // SET(FRAMEWORK_SCHEDULER_MAX_TASKS "32" CACHE STRING "The maximum number of tasks that can be registered with the scheduler")
#define FRAMEWORK_SCHEDULER_LP_MODE ( 0 ) // SET(FRAMEWORK_SCHEDULER_LP_MODE "0" CACHE STRING "The low power mode to use. Only change this if you know exactly what you are doing")
#define FRAMEWORK_LOG_ENABLED ( FALSE ) // SET(FRAMEWORK_LOG_ENABLED "FALSE" CACHE BOOL "Select whether to enable or disable the generation of logs")
#define FRAMEWORK_LOG_OUTPUT_ON_RTT ( FALSE ) // SET(FRAMEWORK_LOG_OUTPUT_ON_RTT "FALSE" CACHE BOOL "When enabled logs will be outputted using the Segger RTT interface instead of using the serial console.")
#define FRAMEWORK_TIMER_LOG_ENABLED ( FALSE ) // SET(FRAMEWORK_TIMER_LOG_ENABLED "FALSE" CACHE BOOL "Select whether to enable or disable the generation of logs from the timer")
#define FRAMEWORK_PHY_LOG_ENABLED ( FALSE ) // SET(FRAMEWORK_PHY_LOG_ENABLED "FALSE" CACHE BOOL "Select whether to enable or disable the generation of logs from the PHY layer")
#define FRAMEWORK_ALP_LOG_ENABLED ( FALSE ) // SET(FRAMEWORK_ALP_LOG_ENABLED "FALSE" CACHE BOOL "Select whether to enable or disable the generation of logs from the ALP comonent")
#define FRAMEWORK_MODEM_LOG_ENABLED ( FALSE ) // SET(FRAMEWORK_MODEM_LOG_ENABLED "FALSE" CACHE BOOL "Select whether to enable or disable the generation of logs from the modem component")
#define FRAMEWORK_MODEM_INTERFACE_LOG_ENABLED ( FALSE ) // SET(FRAMEWORK_MODEM_INTERFACE_LOG_ENABLED "FALSE" CACHE BOOL "Select whether to enable or disable the generation of logs from the modem interface component")
#define FRAMEWORK_SCHED_LOG_ENABLED ( FALSE ) // SET(FRAMEWORK_SCHED_LOG_ENABLED "FALSE" CACHE BOOL "Select whether to enable or disable the generation of logs from the scheduler component")
#define FRAMEWORK_DEBUG_ASSERT_MINIMAL ( TRUE ) // SET(FRAMEWORK_DEBUG_ASSERT_MINIMAL "FALSE" CACHE BOOL "Enabling this strips file, line functino and condition information from asserts, to save ROM")
//#define FRAMEWORK_DEBUG_ASSERT_REBOOT ( FALSE ) // SET(FRAMEWORK_DEBUG_ASSERT_REBOOT "FALSE" CACHE BOOL "When enabled the system will reboot upon assert")
#define FRAMEWORK_DEBUG_ENABLE_SWD ( TRUE ) // SET(FRAMEWORK_DEBUG_ENABLE_SWD "TRUE" CACHE BOOL "Enable the SWD pins and clocks for debug probe usage")
#define FRAMEWORK_CONSOLE_ENABLED ( TRUE ) // SET(FRAMEWORK_CONSOLE_ENABLED "TRUE" CACHE BOOL "Configures if the serial console is enabled")
#define FRAMEWORK_SHELL_ENABLED ( TRUE ) // SET(FRAMEWORK_SHELL_ENABLED "TRUE" CACHE BOOL "Configures if the shell over console is enabled")
#define FRAMEWORK_TIMER_STACK_SIZE ( 10 ) // SET(FRAMEWORK_TIMER_STACK_SIZE "10" CACHE STRING "The number of simultaneous timer events that can be scheduled. Increase this if you have lots of concurrent timers")
#define FRAMEWORK_TIMER_RESOLUTION 1MS // SET(FRAMEWORK_TIMER_RESOLUTION "1MS" CACHE STRING "The frequency of the framework timer. One of '1MS' (1024 ticks per second) or '32K' (32768 ticks per second)")
//SET_PROPERTY( CACHE FRAMEWORK_TIMER_RESOLUTION PROPERTY STRINGS "1MS;32K")
#define FRAMEWORK_AES_LOG_ENABLED ( FALSE ) // SET(FRAMEWORK_AES_LOG_ENABLED "FALSE" CACHE BOOL "Select whether to enable or disable the generation of logs in the AES algorithms")
#define FRAMEWORK_FS_TRUSTED_NODE_TABLE_SIZE ( 16 ) // SET(FRAMEWORK_FS_TRUSTED_NODE_TABLE_SIZE "16" CACHE STRING "The max number of trusted node entries which can be used to store security state")
#define FRAMEWORK_FS_USER_FILE_COUNT ( 10 ) // SET(FRAMEWORK_FS_USER_FILE_COUNT "10" CACHE STRING "The number of user files in the filesystem")
#define FRAMEWORK_FS_USER_FILESYSTEM_SIZE ( 100 ) // SET(FRAMEWORK_FS_USER_FILESYSTEM_SIZE "100" CACHE STRING "The total number of bytes which can be stored in the user filesystem")


// Override the name of the DASH7 timer routines to not collied with the esp-idf version
#define timer_get_counter_value dash7_timer_get_counter_value
#define timer_init dash7_timer_init