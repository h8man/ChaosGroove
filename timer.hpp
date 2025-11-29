// Timer (.Hpp)
// ------------

#ifndef __Timer_hpp__
#define __Timer_hpp__ 

typedef union {
    struct {
        unsigned long LowPart;
        long HighPart;
    };
    long long QuadPart;
} LARGE_TICK;

struct ttimer_t   
{
 LARGE_TICK tstart, tticks, tnow, tlast;
 int high_freq, logic_frames, logic_fps, gfx_frames, gfx_fps;
};

extern struct ttimer_t timer;

void start_timer(void);
void reset_timer(void);
int check_timer(int frac_sec);
void reset_timer_alt(void);

#endif
