/*----------------------------------------------------------------------------*\
  MODULE NAME: math.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#include "math.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>


namespace F3DB {

	void InitMath() {
		int32 seed;
		FILE *fp = fopen("randseed", "rb");
		if(fp) {
			fread((void*)&seed, 2, 1, fp);
			fclose(fp);
		} else {
			seed = time(null);
		}
		srand(seed);
		fp = fopen("randseed", "wb");
		if(fp) {
			seed = Random((int32)0, (int32)INT32_MAX);
			fwrite((void*)&seed, 2, 1, fp);
			fclose(fp);
		}
	}

	int32 Random(int32 min, int32 max) {
		return (int32)floor((double)min + ((double)max - (double)min)*(double)rand()/(double)RAND_MAX + 0.5);
	}

	float Random(float min, float max) {
		return (float)floor((double)min + ((double)max - (double)min)*(double)rand()/(double)RAND_MAX + 0.5);
	}

}
