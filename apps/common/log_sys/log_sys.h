/**
 * @file log_sys.h
 * @brief Logging system header file for BLE Mesh application
 * 
 * This file provides a comprehensive logging system with multiple log levels
 * and support for both software and hardware UART output. The logging system
 * can be enabled/disabled at compile time and supports different log levels
 * for runtime filtering.
 * 
 * @author developer
 * @date 2025
 */

#ifndef USER_LOG_SYSTEM_H_
#define USER_LOG_SYSTEM_H_

#include "proj/tl_common.h"
#include "vendor/mesh/app.h"
#include "vendor/mesh_lpn/app.h"
#include "vendor/mesh_provision/app.h"
#include "vendor/mesh_switch/app.h"
#include "vendor/common/sensors_model.h"
#include "proj_lib/mesh_crypto/sha256_telink.h"
#include "vendor/common/app_heartbeat.h"
#include "vendor/common/scene.h"

// ============================================================================
// Log System Configuration
// ============================================================================

/** @brief Enable or disable the logging system (1 = enabled, 0 = disabled) */
#define LOG_ENABLE      1

// ============================================================================
// Log Level Definitions
// ============================================================================

/** @brief Log level: No logging output */
#define LOG_LEVEL_NONE   0
/** @brief Log level: Error messages only */
#define LOG_LEVEL_ERROR  1
/** @brief Log level: Warning and error messages */
#define LOG_LEVEL_WARN   2
/** @brief Log level: Informational, warning, and error messages */
#define LOG_LEVEL_INFO   3
/** @brief Log level: All messages including debug information */
#define LOG_LEVEL_DEBUG  4

// ============================================================================
// Log Macros
// ============================================================================

#if(LOG_ENABLE)

/**
 * @brief Log error message
 * @param fmt Format string (printf-style)
 * @param ... Variable arguments for format string
 * 
 * Example: LOGE("Failed to initialize: %d", error_code);
 */
#if LOG_LEVEL_ERROR
#define LOGE(fmt, ...)    log_output("ERROR", fmt, ##__VA_ARGS__)
#else
#define LOGE(fmt, ...)
#endif

/**
 * @brief Log warning message
 * @param fmt Format string (printf-style)
 * @param ... Variable arguments for format string
 * 
 * Example: LOGW("Low battery: %d%%", battery_level);
 */
#if LOG_LEVEL_WARN
#define LOGW(fmt, ...)    log_output("WARN ", fmt, ##__VA_ARGS__)
#else
#define LOGW(fmt, ...)
#endif

/**
 * @brief Log informational message
 * @param fmt Format string (printf-style)
 * @param ... Variable arguments for format string
 * 
 * Example: LOGI("Device initialized successfully");
 */
#if LOG_LEVEL_INFO
#define LOGI(fmt, ...)    log_output("INFO ", fmt, ##__VA_ARGS__)
#else
#define LOGI(fmt, ...)
#endif

/**
 * @brief Log debug message
 * @param fmt Format string (printf-style)
 * @param ... Variable arguments for format string
 * 
 * Example: LOGD("Processing packet: %d bytes", packet_size);
 */
#if LOG_LEVEL_DEBUG
#define LOGD(fmt, ...)    log_output("DEBUG", fmt, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...)
#endif

/**
 * @brief Log debug message with hexadecimal data dump
 * @param data Pointer to the data buffer to be logged
 * @param len Length of the data buffer in bytes
 * 
 * This macro formats binary data as a hexadecimal string and logs it.
 * Maximum buffer size is 128 bytes for the hex string representation.
 * 
 * Example: LOGD_HEX(packet_data, packet_length);
 */
#define LOGD_HEX(data, len)                                     \
    do {                                                       \
        char hex_buf[128];                                     \
        format_hex_string(hex_buf, data, len);                 \
        log_output("DEBUG", "len: %d, data: %s", len, hex_buf);\
    } while (0)

// ============================================================================
// Function Declarations
// ============================================================================

/**
 * @brief Format binary data as a hexadecimal string
 * @param out Output buffer to store the formatted hex string (must be large enough)
 * @param data Pointer to the binary data to format
 * @param len Length of the data in bytes
 * 
 * Converts binary data to a hexadecimal string representation.
 * The output buffer should be at least (len * 2 + 1) bytes to accommodate
 * the hex string and null terminator.
 */
void format_hex_string(char *out, const unsigned char *data, unsigned int len);

/**
 * @brief Initialize the logging system
 * 
 * Initializes the UART interface (software or hardware) based on the
 * LOG_MODE_CONFIG setting. Must be called before using any log macros.
 */
void log_init(void);

/**
 * @brief Output a log message
 * @param level Log level string (e.g., "ERROR", "WARN", "INFO", "DEBUG")
 * @param fmt Format string (printf-style)
 * @param ... Variable arguments for format string
 * 
 * Core function that formats and outputs log messages to the configured
 * UART interface. This function is typically called through the log macros.
 */
void log_output(const char *level, const char *fmt, ...);

#else

// Logging disabled - all macros expand to empty statements
#define LOGE(fmt, ...)
#define LOGW(fmt, ...)
#define LOGI(fmt, ...)
#define LOGD(fmt, ...)
#define log_init();
#endif

// ============================================================================
// UART Configuration
// ============================================================================

/** @brief UART mode: No UART output (logging disabled) */
#define USE_NONE		  0
/** @brief UART mode: Software UART implementation */
#define USE_SOFT_UART     1   
/** @brief UART mode: Hardware UART peripheral */
#define USE_HARD_UART     2

/** @brief Configure which UART mode to use for logging output */
#define LOG_MODE_CONFIG 	USE_SOFT_UART

// ============================================================================
// GPIO Pin Definitions
// ============================================================================

/** @brief Software UART transmit pin for logging */
#define SUART_TX_PIN_LOG			GPIO_PB1 
/** @brief Software UART receive pin for logging */
#define SUART_RX_PIN_LOG			GPIO_PC3

/** @brief Hardware UART transmit pin for logging */
#define HUART_TX_PIN_LOG			GPIO_PD7 
/** @brief Hardware UART receive pin for logging */
#define HUART_RX_PIN_LOG			GPIO_PA0

// ============================================================================
// UART Timing Configuration
// ============================================================================

/** @brief Software UART start bit duration in microseconds */
#define	UART_TIME_BIT_START	90			//us
/** @brief Software UART bit time duration in microseconds */
#define UART_BIT_TIME	104			//us

/** @brief UART baud rate for hardware UART mode */
#define BAUD_RATE		115200

// ============================================================================
// Type Definitions
// ============================================================================

/**
 * @brief Software UART operation mode enumeration
 */
enum{
	sUartTxMode,    /**< Software UART transmit mode */
	sUartRxMode,    /**< Software UART receive mode */
};

/**
 * @brief Software UART configuration structure
 * 
 * This structure holds the GPIO pin configuration for software UART
 * implementation, including both transmit and receive pins.
 */
typedef struct {
	GPIO_PinTypeDef txPin;  /**< Transmit pin configuration */
	GPIO_PinTypeDef rxPin;  /**< Receive pin configuration */
}sUartTypeDef;

#endif
