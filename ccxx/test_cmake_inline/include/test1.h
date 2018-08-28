#pragma one

#include <stdint.h>

#define MULTI 1

static inline uint64_t cal_time_by_frame(int frame) {
    return MULTI * frame * 10;
}

static inline int cal_frame_by_time(uint64_t time) {
    return time / (MULTI * 10);
}