/*----------------------------------------------------------------------------*\
  MODULE NAME: dxerror.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Error reporting functions for DirectX
\*----------------------------------------------------------------------------*/
#include "dxerror.h"
#include <dxerr9.h>
#include <stdio.h>



namespace F3DB {
	
	
/*----------------------------------------------------------------------------*\
  Generic DX Error functions
\*----------------------------------------------------------------------------*/
	
	str _DXError(cstr message, HRESULT errorCode) {
		char *format;
		char *msg;
		const char *dxename, *dxemsg;
		if(errorCode == D3D_OK) {
			msg = new char[strlen(message) + 1];
			strcpy(msg, message);
			return msg;
		}
		format = "%s (DirectX Error: %s (%s))!";
		dxename = DXGetErrorString9(errorCode);
		dxemsg = DXGetErrorDescription9(errorCode);
		msg = new char[strlen(format) + strlen(message) + strlen(dxemsg) + strlen(dxename) - 5];
		sprintf(msg, format, message, dxemsg, dxename);
		return msg;
	}
	
	
	str _DXError(HRESULT errorCode) {
		char *format = "DirectX Error: %s (%s)!";
		char *msg;
		const char *dxename = DXGetErrorString9(errorCode), *dxemsg = DXGetErrorDescription9(errorCode);
		msg = new char[strlen(format) + strlen(dxemsg) + strlen(dxename) - 3];
		sprintf(msg, format, dxemsg, dxename);
		return msg;
	}
	
	
/*----------------------------------------------------------------------------*\
  DX Error functions
\*----------------------------------------------------------------------------*/

	/*
	
	HRESULT DXError(cstr message, HRESULT errorCode) {
		str msg = _DXError(message, errorCode);
		Log(msg);
		delete[] msg;
		return errorCode;
	}
	
	
	HRESULT DXError(HRESULT errorCode) {
		str msg = _DXError(errorCode);
		Log(msg);
		delete[] msg;
		return S_FALSE;
	}

	*/
	
	
/*----------------------------------------------------------------------------*\
  DX Exception
\*----------------------------------------------------------------------------*/
	
	
	DXException::DXException(HRESULT errorCode = D3D_OK): Exception(_DXError(errorCode)) {
	}
	
	
	DXException::DXException(cstr message, HRESULT errorCode): Exception(_DXError(message, errorCode)) {
	}
	
	
	
}