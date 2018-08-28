#include <test1.h>

inline uint64_t cal_time_by_frame(int frame) {
    return MULTI * frame * 10;
}

inline int cal_frame_by_time(uint64_t time) {
    return time / (MULTI * 10);
}