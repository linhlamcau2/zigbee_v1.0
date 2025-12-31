#include "task.h"
#include "../log_sys/log_sys.h"
#include "static_list.h" 
#include <string.h>
#include "../drivers/drv_uptime.h"

typedef struct {
    loop_handler_t handler;
    void *param;
    u32 interval;   /* ms */
    u32 next_tick;
} loop_entry_t;

SLIST_DEFINE(g_task_list, loop_entry_t, MAX_LOOP_HANDLERS)

bool is_due_time(u32 now, u32 next)
{
    return ((now - next) < 0x80000000U);
}

void loop_registry_init(void)
{
    SLIST_INIT(g_task_list);
}

bool is_task_registered(loop_handler_t func)
{
    if (!func) return false;

    SLIST_FOREACH(idx, &g_task_list) {
        loop_entry_t *entry = SLIST_GET_DATA(&g_task_list, idx);
        if (entry->handler == func) {
            return true;
        }
    }
    return false;
}

bool register_task(loop_handler_t func, void *param, u32 interval, u32 time_delay_ms)
{
    if (!func) return false;
    if (is_task_registered(func)) {
        LOGI("Task already registered!\n");
        return false;
    }

    int new_idx = _SLIST_ALLOC_NODE(&g_task_list);
    
    if (new_idx == SLIST_NULL) {
        LOGW("Loop registry full!");
        return false;
    }

    loop_entry_t *new_entry = SLIST_GET_DATA(&g_task_list, new_idx);
    new_entry->handler = func;
    new_entry->param = param;
    new_entry->interval = interval - (interval >> 6) - (interval >> 9);  // ~98% realtime clock
    new_entry->next_tick = drv_uptime_get_ms() + time_delay_ms;
    _SLIST_LINK_TAIL(&g_task_list, new_idx);
    
    return true;
}

bool unregister_task(loop_handler_t func)
{
    if (!func) return false;
    
    int prev_idx = SLIST_NULL;
    int curr_idx = SLIST_HEAD_IDX(&g_task_list);
    bool found = false;

    while (curr_idx != SLIST_NULL) {
        int next_idx = SLIST_NEXT_IDX(&g_task_list, curr_idx);
        loop_entry_t* entry = SLIST_GET_DATA(&g_task_list, curr_idx);

        if (entry->handler == func) {
            SLIST_REMOVE_CURRENT(&g_task_list, curr_idx, prev_idx);
            found = true;
            break; 
        } else {
            prev_idx = curr_idx;
        }
        curr_idx = next_idx;
    }
    return found;
}

bool unregister_all_tasks()
{
    SLIST_INIT(g_task_list);
    return true;
}

void run_loop()
{
    int prev_idx = SLIST_NULL;
    u32 now = drv_uptime_get_ms(); 

    SLIST_FOREACH_SAFE(curr_idx, next_idx, &g_task_list) {
        loop_entry_t *entry = SLIST_GET_DATA(&g_task_list, curr_idx);

        if (entry->handler && is_due_time(now, entry->next_tick)) {
            int keep = entry->handler(entry->param);
            
            if (keep) {
                entry->next_tick = now + entry->interval;
                prev_idx = curr_idx; 
            } else {
                SLIST_REMOVE_CURRENT(&g_task_list, curr_idx, prev_idx);
            }
        } else {
            prev_idx = curr_idx; 
        }
    }
}
