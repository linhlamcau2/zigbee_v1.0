/**
 * @file task.h
 * @brief Task scheduler system for periodic and delayed task execution
 * 
 * This header provides a lightweight task scheduler that allows registration
 * of callback functions to be executed periodically or with a delay. The
 * scheduler maintains a registry of tasks and executes them based on their
 * configured intervals and delays.
 * 
 * Features:
 * - Periodic task execution with configurable intervals
 * - Delayed task execution support
 * - Dynamic task registration and unregistration
 * - Fixed-size task registry (compile-time configurable)
 * - Non-blocking task execution model
 * 
 * @author developer
 * @date 2025
 */

#ifndef USER_TASK_H
#define USER_TASK_H

#include<stdint.h>
#include "tl_common.h"
// ============================================================================
// Constants
// ============================================================================

/** @brief Maximum number of loop handlers that can be registered */
#define MAX_LOOP_HANDLERS    30

// ============================================================================
// Type Definitions
// ============================================================================

/**
 * @brief Task handler function pointer type
 * @param param User-defined parameter passed to the handler
 * @return Non-zero to keep the task registered, zero to remove it after execution
 * 
 * Task handler functions are called periodically based on their configured
 * interval. The handler receives a user-defined parameter and can return
 * a value to control whether the task should remain registered.
 * 
 * Example:
 * @code
 * int my_task_handler(void *param) {
 *     int *counter = (int *)param;
 *     (*counter)++;
 *     return 1;  // Keep task registered
 * }
 * @endcode
 */
typedef int (*loop_handler_t)(void *param);

// ============================================================================
// Task Management Macros
// ============================================================================

/**
 * @brief Initialize the task scheduler system
 * 
 * Initializes the task registry. Must be called before using any other
 * task management functions.
 * 
 * Example:
 * @code
 * TASK_INIT();
 * @endcode
 */
#define TASK_INIT()                 loop_registry_init()

/**
 * @brief Add a task to the scheduler
 * @param handler Function pointer to the task handler
 * @param param User-defined parameter to pass to the handler
 * @param interval Execution interval in milliseconds (0 for one-time execution)
 * @param delay_ms Initial delay before first execution in milliseconds
 * @return true if task was successfully registered, false otherwise
 * 
 * Registers a task handler to be executed periodically. The task will
 * first execute after delay_ms milliseconds, then every interval milliseconds.
 * If interval is 0, the task executes only once after the delay.
 * 
 * Example:
 * @code
 * int my_param = 42;
 * TASK_ADD(my_task_handler, &my_param, 1000, 500);  // Execute every 1s, start after 500ms
 * @endcode
 */
#define TASK_ADD(handler, param, interval, delay_ms) \
    register_task((loop_handler_t)(handler), (void*)(param), (u32)(interval), (u32)(delay_ms))

/**
 * @brief Remove a task from the scheduler
 * @param handler Function pointer to the task handler to remove
 * @return true if task was found and removed, false otherwise
 * 
 * Unregisters a previously registered task handler. The task will
 * no longer be executed.
 * 
 * Example:
 * @code
 * TASK_DEL(my_task_handler);
 * @endcode
 */
#define TASK_DEL(handler)           unregister_task((loop_handler_t)(handler))

/**
 * @brief Remove all registered tasks
 * 
 * Unregisters all tasks from the scheduler. The task registry is cleared.
 * 
 * Example:
 * @code
 * TASK_DEL_ALL();
 * @endcode
 */
#define TASK_DEL_ALL()              unregister_all_tasks()

/**
 * @brief Check if a task is registered
 * @param handler Function pointer to the task handler to check
 * @return true if task is registered, false otherwise
 * 
 * Checks whether a specific task handler is currently registered
 * in the scheduler.
 * 
 * Example:
 * @code
 * if (TASK_EXISTS(my_task_handler)) {
 *     // Task is registered
 * }
 * @endcode
 */
#define TASK_EXISTS(handler)        is_task_registered((loop_handler_t)(handler))

/**
 * @brief Run the task scheduler loop
 * 
 * Executes all due tasks in the registry. This function should be
 * called periodically (e.g., in the main loop) to process scheduled tasks.
 * 
 * Example:
 * @code
 * while (1) {
 *     TASK_RUN();
 *     // Other application code
 * }
 * @endcode
 */
#define TASK_RUN()                  run_loop()

// ============================================================================
// Function Declarations
// ============================================================================

/**
 * @brief Initialize the task registry
 * 
 * Initializes the internal task registry structure. Clears all registered
 * tasks and prepares the scheduler for use. Must be called before using
 * any other task management functions.
 */
void loop_registry_init(void);

/**
 * @brief Check if a task handler is registered
 * @param func Function pointer to the task handler
 * @return true if the task is registered, false otherwise
 * 
 * Searches the task registry for the specified handler function.
 */
bool is_task_registered(loop_handler_t func);

/**
 * @brief Register a task handler
 * @param func Function pointer to the task handler
 * @param param User-defined parameter to pass to the handler
 * @param interval Execution interval in milliseconds (0 for one-time execution)
 * @param time_delay_ms Initial delay before first execution in milliseconds
 * @return true if task was successfully registered, false if registry is full
 * 
 * Adds a task handler to the scheduler. The task will execute after
 * time_delay_ms milliseconds, then periodically every interval milliseconds.
 * If interval is 0, the task executes only once.
 * 
 * @note The registry has a maximum capacity of MAX_LOOP_HANDLERS tasks.
 */
bool register_task(loop_handler_t func, void *param, u32 interval, u32 time_delay_ms);

/**
 * @brief Unregister a task handler
 * @param func Function pointer to the task handler to remove
 * @return true if task was found and removed, false otherwise
 * 
 * Removes the specified task handler from the registry. The task will
 * no longer be executed by the scheduler.
 */
bool unregister_task(loop_handler_t func);

/**
 * @brief Unregister all task handlers
 * @return true on success
 * 
 * Clears the entire task registry, removing all registered tasks.
 * The scheduler will have no tasks to execute after this call.
 */
bool unregister_all_tasks();

/**
 * @brief Execute all due tasks
 * 
 * Iterates through the task registry and executes all tasks that are
 * due for execution based on their configured intervals and delays.
 * This function should be called periodically from the main application loop.
 * 
 * Tasks that return 0 from their handler will be automatically unregistered.
 */
void run_loop();

/**
 * @brief Check if a task execution time is due
 * @param now Current time in milliseconds
 * @param next Next scheduled execution time in milliseconds
 * @return true if current time has reached or passed the next execution time
 * 
 * Helper function to determine if a task's scheduled execution time has arrived.
 * Handles time wrap-around for systems with limited time counter range.
 */
bool is_due_time(u32 now, u32 next);

#endif /* TASK_H */