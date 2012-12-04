/**
 * @file log.h
 * @brief Loging procedures. You have to define function used to print
 * messages to output in convention like printf.
 *
 * You also ned to define macro LOG_STD_OUTPUT_FUNCTION with your printf
 * function.
 * 
 * @author Daniel Szot
 */
#ifndef _LOG_H
#define _LOG_H

#if defined(LOG_ENABLED)

#define LOG_MAX_CHARS_PER_LINE 100
#define LOG_MAX_LINE_DELAY portMAX_DELAY
#define LOG_STD_OUTPUT_FUNCTION log_output

enum logMessageType
{
    LOG_MESSAGE,
    LOG_ERROR,
    LOG_WARNING,
    LOG_SUCCESS,
    LOG_DEBUG_INFO
};

/**
 * Primitive should be like this:
 * void write(logMessageType type, char *str, ...);
 */
#if !defined(LOG_STD_OUTPUT_FUNCTION)
    #error "LOG_STD_OUTPUT_FUNCTION not defined. Please define your printf function"
#endif

#define LOG(f, ...) LOG_STD_OUTPUT_FUNCTION(LOG_MESSAGE, f, ##__VA_ARGS__)
#define ERROR(f, ...) LOG_STD_OUTPUT_FUNCTION(LOG_ERROR, f, ##__VA_ARGS__)
#define WARNING(f, ...) LOG_STD_OUTPUT_FUNCTION(LOG_WARNING, f, ##__VA_ARGS__)
#define DONE(f, ...) LOG_STD_OUTPUT_FUNCTION(LOG_SUCCESS, f, ##__VA_ARGS__)

#if defined(LOG_DEBUG)
#define DEBUG(f, ...) LOG_STD_OUTPUT_FUNCTION(LOG_DEBUG_INFO, f, ##__VA_ARGS__)
#else
#define DEBUG(...)
#endif
#define MUTEX_ERROR() LOG_STD_OUTPUT_FUNCTION(LOG_ERROR, \
        "Can't take mutex. file: %s, line: %d", __FILE__, __LINE__)
#define LOG_PUTC(c) log_putc(c);
#else

#define LOG(...)
#define ERROR(...)
#define WARNING(...)
#define DONE(...)
#define DEBUG(...)
#define LOG_PUTC(...)
#define MUTEX_ERROR(...)
#define logSetUartDevice(...)

#endif

#endif // _LOG_H
