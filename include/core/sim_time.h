#ifndef GIGAVECTOR_GV_SIM_TIME_H
#define GIGAVECTOR_GV_SIM_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GV_TIME_WALL = 0,
    GV_TIME_SIM = 1
} GV_TimeMode;

/** Switch between wall clock and deterministic virtual time. */
void gv_sim_time_set_mode(GV_TimeMode mode);
GV_TimeMode gv_sim_time_get_mode(void);

/** Reset virtual clock (seconds since epoch). */
void gv_sim_time_reset(uint64_t start_sec);

/** Advance virtual clock without sleeping. */
void gv_sim_time_advance_sec(uint64_t delta_sec);
void gv_sim_time_advance_ms(uint64_t delta_ms);

/** Current time in seconds / milliseconds. */
uint64_t gv_time_now_sec(void);
uint64_t gv_time_now_ms(void);

/**
 * Sleep for @p ms. In simulation mode advances virtual time only (no thread sleep).
 */
void gv_time_sleep_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* GIGAVECTOR_GV_SIM_TIME_H */
