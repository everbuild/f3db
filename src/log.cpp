/*----------------------------------------------------------------------------*\
  MODULE NAME: log.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Logging class
\*----------------------------------------------------------------------------*/
#include "log.h"
#include <string.h>
#include <time.h>
#include "exception.h"



namespace F3DB {


	Log::Log(cstr fileName, cstr title) {
		// open file
		fp = fopen(fileName, "w");
		if(!fp) throw new Exception("Failed to open logfile!");
		// write header
		time_t rawtime;
		time(&rawtime);
		str timestr = asctime(localtime(&rawtime));
		int len = strlen(title) + 1 + strlen(timestr);
		fputs(title, fp);
		fputs(", ", fp);
		fputs(timestr, fp);
		for(int i = 0; i < len; i ++) fputc('=', fp);
		fputs("\r\n", fp);
		fputs("\r\n", fp);
		fflush(fp);
	}

	Log::~Log() {
		fclose(fp);
	}

	void Log::Write(cstr message) {
		fputs(message, fp);
		fputs("\r\n", fp);
		fflush(fp);
	}


}
