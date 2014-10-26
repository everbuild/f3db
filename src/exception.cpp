/*----------------------------------------------------------------------------*\
  MODULE NAME: exception.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Base exception class.
\*----------------------------------------------------------------------------*/
#include "exception.h"
#include <string.h>



namespace F3DB {
	
	
	
	Exception::Exception() {
		Exception(F3DB_EXCEPTION_DEFAULT_MESSAGE);
	}
	
	
	
	Exception::Exception(cstr message) {
		uint16 n = strlen(message);
		if(n > F3DB_EXCEPTION_MESSAGE_LENGTH) n = F3DB_EXCEPTION_MESSAGE_LENGTH;
		strncpy(this->message, message, n);
		this->message[n] = 0;
	}
	
	
	cstr Exception::Message() {
		return message;
	}
	
	
	
}