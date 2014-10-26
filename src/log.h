/*----------------------------------------------------------------------------*\
  MODULE NAME: log.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Logging class
\*----------------------------------------------------------------------------*/
#ifndef F3DB_LOG_H
#define F3DB_LOG_H
#include "types.h"
#include <stdio.h>



namespace F3DB {


	class Log {

	public:

		Log(cstr fileName, cstr title);
		~Log();

		void Write(cstr message);

	private:

		FILE *fp;
	};


}



#endif // #ifndef F3DB_LOG_H