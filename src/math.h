/*----------------------------------------------------------------------------*\
  MODULE NAME: math.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#ifndef F3DB_MATH_H
#define F3DB_MATH_H
#include "types.h"


namespace F3DB {

	void InitMath();

	int32 Random(int32 min, int32 max);
	float Random(float min, float max);

}


#endif // #ifndef F3DB_MATH_H