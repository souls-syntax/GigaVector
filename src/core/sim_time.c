/**
 * @file sim_time.c
 * @brief Injectable virtual clock for deterministic simulation testing.
 */

#include "core/sim_time.h"

#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#endif

static GV_TimeMode g_time_mode = GV_TIME_WALL;
static uint64_t g_sim_sec = 1700000000ULL;
static uint64_t g_sim_ms = 0;

void gv_sim_time_set_mode(GV_TimeMode mode) {
    g_time_mode = mode;
    if (mode == GV_TIME_SIM && g_sim_ms == 0) {
        g_sim_ms = g_sim_sec * 1000ULL;
    }
}

GV_TimeMode gv_sim_time_get_mode(void) {
    return g_time_mode;
}

void gv_sim_time_reset(uint64_t start_sec) {
    g_sim_sec = start_sec;
    g_sim_ms = start_sec * 1000ULL;
}

void gv_sim_time_advance_sec(uint64_t delta_sec) {
    g_sim_sec += delta_sec;
    g_sim_ms += delta_sec * 1000ULL;
}

void gv_sim_time_advance_ms(uint64_t delta_ms) {
    g_sim_ms += delta_ms;
    g_sim_sec = g_sim_ms / 1000ULL;
}

uint64_t gv_time_now_sec(void) {
    if (g_time_mode == GV_TIME_SIM) {
        return g_sim_sec;
    }
    return (uint64_t)time(NULL);
}

uint64_t gv_time_now_ms(void) {
    if (g_time_mode == GV_TIME_SIM) {
        return g_sim_ms;
    }
    return (uint64_t)time(NULL) * 1000ULL;
}

void gv_time_sleep_ms(uint32_t ms) {
    if (g_time_mode == GV_TIME_SIM) {
        gv_sim_time_advance_ms(ms);
        return;
    }
#ifndef _WIN32
    if (ms > 0) {
        usleep((useconds_t)ms * 1000U);
    }
#endif
}
