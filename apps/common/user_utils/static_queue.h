/**
 * @file static_queue.h
 * @brief Static circular queue implementation using array-based pool
 * 
 * This header provides a memory-efficient static circular queue (FIFO) 
 * implementation that uses a pre-allocated array pool. The implementation
 * uses a circular buffer design to efficiently reuse memory space.
 * This design eliminates dynamic memory allocation and provides predictable
 * memory usage for embedded systems.
 * 
 * Features:
 * - Fixed-size queue allocated at compile time
 * - O(1) enqueue and dequeue operations
 * - Circular buffer design for efficient memory usage
 * - Type-safe implementation using macros
 * - Thread-safe for single producer/single consumer scenarios
 * 
 * @author developer
 * @date 2025
 */

#ifndef STATIC_QUEUE_H
#define STATIC_QUEUE_H

#include <stdint.h> 

// ============================================================================
// Type Definitions
// ============================================================================

/**
 * @brief Define a static queue structure
 * @param data_type The data type to store in the queue
 * 
 * Creates a queue structure containing:
 * - data_pool: Pointer to the static array pool
 * - front: Index of the front element (next to dequeue)
 * - rear: Index of the rear element (next insertion point)
 * - size: Current number of elements in the queue
 * - max_size: Maximum capacity of the queue
 * 
 * This macro generates a struct type for the queue container.
 * The queue uses a circular buffer implementation.
 */
#define SQUEUE_T(data_type) \
    struct { \
        data_type *data_pool;  /**< Pointer to the static array (pool) */ \
        int front;             /**< Index of the front element (next to dequeue) */ \
        int rear;              /**< Index of the rear element (next insertion point) */ \
        int size;              /**< Current number of elements in the queue */ \
        int max_size;          /**< Maximum capacity of the queue */ \
    }

// ============================================================================
// Queue Definition and Initialization
// ============================================================================

/**
 * @brief Define a static queue with data pool and type
 * @param queue_name Base name for the queue (used to generate type names)
 * @param data_type Data type to store in the queue
 * @param max_size Maximum number of elements in the queue
 * 
 * This macro:
 * - Creates queue type: queue_name##_t
 * - Allocates static pool: queue_name##_pool[max_size]
 * - Declares queue instance: queue_name
 * 
 * Example:
 * @code
 * SQUEUE_DEFINE(my_queue, int, 10);  // Creates my_queue_t, my_queue_pool, my_queue
 * @endcode
 */
#define SQUEUE_DEFINE(queue_name, data_type, max_size) \
    typedef SQUEUE_T(data_type) queue_name##_t; \
    static data_type queue_name##_pool[(max_size)]; \
    static queue_name##_t queue_name;

/**
 * @brief Initialize a static queue
 * @param queue_name Name of the queue instance to initialize
 * 
 * Initializes the queue structure with empty state.
 * Front and rear pointers are set to 0, and size is set to 0.
 * Must be called before using the queue.
 * 
 * Example:
 * @code
 * SQUEUE_DEFINE(my_queue, int, 10);
 * SQUEUE_INIT(my_queue);
 * @endcode
 */
#define SQUEUE_INIT(queue_name) \
    do { \
        (queue_name).data_pool = queue_name##_pool; \
        (queue_name).max_size = sizeof(queue_name##_pool) / sizeof(queue_name##_pool[0]); \
        (queue_name).front = 0; \
        (queue_name).rear = 0; \
        (queue_name).size = 0; \
    } while(0)

// ============================================================================
// Status Check Macros
// ============================================================================

/**
 * @brief Check if the queue is empty
 * @param queue_ptr Pointer to the queue structure
 * @return Non-zero if queue is empty, zero otherwise
 * 
 * Example:
 * @code
 * if (SQUEUE_IS_EMPTY(&my_queue)) {
 *     // Queue is empty
 * }
 * @endcode
 */
#define SQUEUE_IS_EMPTY(queue_ptr) \
    ((queue_ptr)->size == 0)

/**
 * @brief Check if the queue is full
 * @param queue_ptr Pointer to the queue structure
 * @return Non-zero if queue is full, zero otherwise
 * 
 * Example:
 * @code
 * if (!SQUEUE_IS_FULL(&my_queue)) {
 *     // Can enqueue more elements
 * }
 * @endcode
 */
#define SQUEUE_IS_FULL(queue_ptr) \
    ((queue_ptr)->size == (queue_ptr)->max_size)

/**
 * @brief Get the number of elements currently in the queue
 * @param queue_ptr Pointer to the queue structure
 * @return Number of elements in the queue
 * 
 * Example:
 * @code
 * int count = QUEUE_SIZE(&my_queue);
 * printf("Queue has %d elements\n", count);
 * @endcode
 */
#define QUEUE_SIZE(queue_ptr) \
    ((queue_ptr)->size)

// ============================================================================
// Queue Operations
// ============================================================================

/**
 * @brief Add an element to the rear of the queue (enqueue)
 * @param queue_ptr Pointer to the queue structure
 * @param element_ptr Pointer to the element to enqueue
 * @return 0 on success, -1 if queue is full
 * 
 * Copies the element data into the queue at the rear position.
 * The rear pointer is advanced using modulo arithmetic for circular buffer.
 * 
 * Example:
 * @code
 * int value = 42;
 * if (SQUEUE_ENQUEUE(&my_queue, &value) == 0) {
 *     // Successfully enqueued
 * } else {
 *     // Queue is full
 * }
 * @endcode
 */
#define SQUEUE_ENQUEUE(queue_ptr, element_ptr) \
    ({ \
        int _ret = -1; \
        if (!SQUEUE_IS_FULL(queue_ptr)) { \
            memcpy(&(queue_ptr)->data_pool[(queue_ptr)->rear], (element_ptr), sizeof(*(queue_ptr)->data_pool)); \
            (queue_ptr)->rear = ((queue_ptr)->rear + 1) % (queue_ptr)->max_size; \
            (queue_ptr)->size++; \
            _ret = 0; /* Success */ \
        } \
        _ret; \
    })

/**
 * @brief Remove an element from the front of the queue (dequeue)
 * @param queue_ptr Pointer to the queue structure
 * @param element_ptr Pointer to a pointer that will receive the element address
 * @return 0 on success, -1 if queue is empty
 * 
 * Removes the front element from the queue and returns a pointer to it.
 * The front pointer is advanced using modulo arithmetic for circular buffer.
 * The element_ptr parameter will be set to point to the dequeued element.
 * 
 * Note: The returned pointer points to the queue's internal storage.
 * The data remains valid until the next enqueue operation that wraps around.
 * 
 * Example:
 * @code
 * int *element = NULL;
 * if (SQUEUE_DEQUEUE(&my_queue, &element) == 0) {
 *     // element points to the dequeued data
 *     printf("Dequeued: %d\n", *element);
 * } else {
 *     // Queue is empty
 * }
 * @endcode
 */
#define SQUEUE_DEQUEUE(queue_ptr, element_ptr) \
    ({ \
        int _ret = -1; \
        if (!SQUEUE_IS_EMPTY(queue_ptr)) { \
            (element_ptr) = &((queue_ptr)->data_pool[(queue_ptr)->front]); \
            (queue_ptr)->front = ((queue_ptr)->front + 1) % (queue_ptr)->max_size; \
            (queue_ptr)->size--; \
            _ret = 0;  \
        } \
        _ret; \
    })

/**
 * @brief Peek at the front element without removing it
 * @param queue_ptr Pointer to the queue structure
 * @param element_ptr Pointer to a pointer that will receive the element address
 * @return 0 on success, -1 if queue is empty
 * 
 * Returns a pointer to the front element without removing it from the queue.
 * The queue state remains unchanged.
 * 
 * Note: The returned pointer points to the queue's internal storage.
 * The data remains valid until the element is dequeued or overwritten.
 * 
 * Example:
 * @code
 * int *element = NULL;
 * if (SQUEUE_PEEK(&my_queue, &element) == 0) {
 *     // element points to the front element
 *     printf("Front element: %d\n", *element);
 * } else {
 *     // Queue is empty
 * }
 * @endcode
 */
#define SQUEUE_PEEK(queue_ptr, element_ptr) \
    ({ \
        int _ret = -1; \
        if (!SQUEUE_IS_EMPTY(queue_ptr)) { \
            (element_ptr) = &((queue_ptr)->data_pool[(queue_ptr)->front]); \
            _ret = 0; \
        } \
        _ret; \
    })

#endif // STATIC_QUEUE_H
