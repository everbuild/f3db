/*----------------------------------------------------------------------------*\
  MODULE NAME: exception.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Base exception class.
\*----------------------------------------------------------------------------*/
#ifndef F3DB_EXCEPTION_H
#define F3DB_EXCEPTION_H
#include "types.h"
	
#define F3DB_EXCEPTION_MESSAGE_LENGTH 512
#define F3DB_EXCEPTION_DEFAULT_MESSAGE "No message available"



namespace F3DB {
	
	
	class Exception {
		
	public:
		
		Exception();
		Exception(cstr message);
		
		cstr Message();
		
	private:
		
		char message[F3DB_EXCEPTION_MESSAGE_LENGTH];
	};
	
	
}



#endif // #ifndef F3DB_EXCEPTION_H