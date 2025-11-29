//#include <windows.h>
#include "raylib.h"
#include "timer.hpp"

#define TO_NANOSECOND 10000000000

// High resolution timer code for Windows. Call start_timer first and then call 
// check_timer with the required accuracy range.

struct ttimer_t timer;

LARGE_TICK QueryPerformanceCounter()
{
    LARGE_TICK ticks;
    ticks.QuadPart = (GetTime() * TO_NANOSECOND); //nanosec per tick
    //LARGE_INTEGER t;
    //QueryPerformanceCounter(&t);
    //ticks.QuadPart = t.QuadPart;
    return ticks;
}

void start_timer(void) 
{
 //LARGE_INTEGER ticks;
 timer.high_freq = 1;//QueryPerformanceFrequency(&ticks);
 timer.tticks.QuadPart = TO_NANOSECOND;//ticks.QuadPart;
 timer.logic_frames = 0;
 	
 if (timer.high_freq)	
 {
  LARGE_TICK ticks = QueryPerformanceCounter();
  timer.tstart.QuadPart = ticks.QuadPart;
  timer.tlast = timer.tstart;	
 } 
} 

void reset_timer(void) 
{
 if (timer.high_freq)
 {
  LARGE_TICK ticks = QueryPerformanceCounter();
  timer.tnow.QuadPart = ticks.QuadPart;
  timer.tstart = timer.tnow;
 } 
}

int check_timer(int frac_sec) 
{
 int t;

 if (timer.high_freq)
 {
  LARGE_TICK ticks = QueryPerformanceCounter();
  timer.tnow.QuadPart = ticks.QuadPart;

  t = (int) (((timer.tnow.QuadPart - timer.tstart.QuadPart) * frac_sec) / timer.tticks.QuadPart);		
	  
  // Have we done 1 second since the timer was last reset?
  if (t > 240)
  {
   // Yes, so reset again and update details.
   // If we reset the timer after each check we get errors building up causing a lack
   // of precision. We also reset it here to make it as fast as possible.
   
   timer.tstart = timer.tlast;
   t -= timer.logic_frames;
   timer.logic_fps = timer.logic_frames;
   timer.logic_frames = 0;
	   
   timer.gfx_fps = timer.gfx_frames;
   timer.gfx_frames = 0;   
  }
  timer.tlast = timer.tnow;
    
  return t;
 }
 	
 return 0;
}

//#endif	 
 
/*
#if defined(ALLEGRO_MACOSX) || defined(ALLEGRO_UNIX) || defined(ALLEGRO_LINUX)
#include <sys/time.h>

static struct timeval tstart;
static struct timeval tlast;

void start_timer(void)
{
 gettimeofday(&tstart, NULL);
 tlast.tv_sec = tstart.tv_sec;
 tlast.tv_usec = tstart.tv_usec;
}

void reset_timer(void)
{
 gettimeofday(&tstart, NULL);
}

int check_timer(int frac_sec)
{
 struct timeval now;
 double hi, low;
 int t, ms;

 rest(0);
 gettimeofday(&now, NULL);
 
 hi = (double) (now.tv_sec - tstart.tv_sec);
 low = ((double) (now.tv_usec - tstart.tv_usec)) / 1.0e6;
 t = (int) ((hi + low) * ((double) frac_sec));

 if ( t > 240 ) 
 {
  tstart.tv_usec = tlast.tv_usec;
  tstart.tv_sec = tlast.tv_sec;
  
  t -= timer.logic_frames;
  timer.logic_fps = timer.logic_frames;
  timer.logic_frames = 0;
	   
  timer.gfx_fps = timer.gfx_frames;
  timer.gfx_frames = 0;   
 }

 tlast.tv_usec = now.tv_usec;
 tlast.tv_sec = now.tv_sec;
 return t;
}

#endif
*/