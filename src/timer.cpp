/*----------------------------------------------------------------------------*\
  MODULE NAME: timer.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Timing class
\*----------------------------------------------------------------------------*/
#include "timer.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")



namespace F3DB {
	
	
	Timer::Timer() {
		time = 0.0;
		interval = 0.0;
		paused = true;
	}
	
	
	void Timer::Update() {
		if(paused) return;
		current = _GetTime();
		interval = (double)(current - start)/1000.0;
		time += interval;
		start = current;
	}
	
	
	double Timer::GetTime() {
		return time;
	}
	
	
	double Timer::GetInterval() {
		return interval;
	}
	
	
	void Timer::Pause(bool pause) {
		if(pause == paused) return;
		paused = pause;
		current = _GetTime();
		if(pause) {
			interval = (double)(current - start)/1000.0;
			time += interval;
		} else {
			start = current;
		}
	}
	
	
	bool Timer::IsPaused() {
		return paused;
	}


	uint32 Timer::_GetTime() {
		return timeGetTime();
	}

}
