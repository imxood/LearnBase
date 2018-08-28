#include <stdint.h>

#define MULTI 1

static inline uint64_t cal_time_by_frame(int frame);

static inline int cal_frame_by_time(uint64_t time);


int main(){
	cal_time_by_frame(1);
	cal_frame_by_time(2);
	return 0;
}