#ifndef GIGAVECTOR_GV_INDEX_MAINTENANCE_H
#define GIGAVECTOR_GV_INDEX_MAINTENANCE_H

#include <stddef.h>
#include <stdint.h>

#include "index/ivfdisk.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GV_MAINT_JOB_SPLIT = 0,
    GV_MAINT_JOB_REASSIGN,
    GV_MAINT_JOB_MERGE,
    GV_MAINT_JOB_DEFRAG
} GV_MaintenanceJobType;

typedef struct {
    float live_ratio_threshold;   /**< Merge when live_ratio falls below (default 0.5). */
    size_t segment_count_max;     /**< Defrag/merge when segments/head exceed (default 4). */
    size_t max_jobs_per_run;      /**< Cap work per db_compact call (default 8). */
} GV_IVFDiskMaintenanceConfig;

typedef struct {
    size_t splits;
    size_t merges;
    size_t defrags;
    size_t reassigns;
    size_t jobs_run;
} GV_IVFDiskMaintenanceStats;

void ivfdisk_maintenance_config_init(GV_IVFDiskMaintenanceConfig *config);

/**
 * Scan IVFDisk posting lists and run maintenance jobs (split > reassign > merge > defrag).
 * Safe to call from db_compact() while holding the database write lock.
 */
int ivfdisk_maintenance_run(GV_IVFDiskIndex *index,
                            const GV_IVFDiskMaintenanceConfig *config,
                            GV_IVFDiskMaintenanceStats *stats);

/**
 * Lazy merge on read path: compact a head when live_ratio or segment count warrants it.
 * @return 1 if merge ran, 0 if not needed, -1 on error.
 */
int ivfdisk_maintenance_maybe_merge_head(GV_IVFDiskIndex *index, uint64_t head_id,
                                         const GV_IVFDiskMaintenanceConfig *config);

#ifdef __cplusplus
}
#endif

#endif
