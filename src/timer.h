/*----------------------------------------------------------------------------*\
  MODULE NAME: timer.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Timing class
\*----------------------------------------------------------------------------*/
#ifndef F3DB_TIMER_H
#define F3DB_TIMER_H
#include "types.h"



namespace F3DB {


	class Timer {

	public:

		Timer();

		void Update();
		double GetTime();
		double GetInterval();
		void Pause(bool pause);

		bool IsPaused();

	private:

		double time, interval;
		uint32 start, current;
		bool paused;

		inline uint32 _GetTime();
	};

}



#endif // #ifndef F3DB_TIMER_H