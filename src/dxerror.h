/*----------------------------------------------------------------------------*\
  MODULE NAME: dxerror.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Error reporting functions for DirectX
\*----------------------------------------------------------------------------*/
#ifndef F3DB_DXERROR_H
#define F3DB_DXERROR_H
#include <d3dx9.h>
#include "types.h"
#include "exception.h"



namespace F3DB {
	
	
/*----------------------------------------------------------------------------*\
  DX Error functions - deprecated
\*----------------------------------------------------------------------------*/

	/*
	HRESULT DXError(cstr message, HRESULT errorCode = D3D_OK);
	HRESULT DXError(HRESULT errorCode);
	*/
	
	
/*----------------------------------------------------------------------------*\
  DX Exception
\*----------------------------------------------------------------------------*/
	
	
	class DXException: public Exception {
		
	public:
		
		DXException(HRESULT errorCode);
		DXException(cstr message, HRESULT errorCode = D3D_OK);
	};
	
	
	
}



#endif // #ifndef F3DB_DXERROR_H