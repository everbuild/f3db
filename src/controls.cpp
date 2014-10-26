/*----------------------------------------------------------------------------*\
  MODULE NAME: controls.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#include "controls.h"
#include "dxerror.h"
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput.lib")



namespace F3DB {
	
	
	Controls::Controls(Graphics *graphics): mousePos() {

		di = null;
		diKeybourd = null;
		diMouse = null;

		Init(graphics);
	}



	Controls::~Controls() {
		CleanUp();
	}



	void Controls::Reset(Graphics *graphics) {
		CleanUp();
		Init(graphics);
	}



	void Controls::Init(Graphics *graphics) {

		if(graphics) this->graphics = graphics;

		hWnd = graphics->GetWindowHandle();

		memset(keys, 0, 255);
		memset(&mouseState, 0, sizeof(DIMOUSESTATE));
		
		
		// init direct input
		
		res = DirectInputCreate(GetModuleHandle(null), DIRECTINPUT_VERSION, &di, null);
		if(res != DI_OK) throw new DXException(res);
		
		
		// init keybourd
		
		res = di->CreateDevice(GUID_SysKeyboard, &diKeybourd, null); 
		if(res != DI_OK) throw new DXException(res);
		res = diKeybourd->SetDataFormat(&c_dfDIKeyboard);
		if(res != DI_OK) throw new DXException(res);
		res = diKeybourd->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(res != DI_OK) throw new DXException(res);
		res = diKeybourd->Acquire();
		if(res != DI_OK) throw new DXException(res);
		
		
		// init mouse
		
		res = di->CreateDevice(GUID_SysMouse, &diMouse, null); 
		if(res != DI_OK) throw new DXException(res);
		res = diMouse->SetDataFormat(&c_dfDIMouse);
		if(res != DI_OK) throw new DXException(res);
		res = diMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(res != DI_OK) throw new DXException(res);
		res = diMouse->Acquire();
		if(res != DI_OK) throw new DXException(res);
	}



	void Controls::CleanUp() {
		if(diMouse) {
			diMouse->Unacquire();
			diMouse->Release();
			diMouse = null;
		}
		if(diKeybourd) {
			diKeybourd->Unacquire();
			diKeybourd->Release();
			diKeybourd = null;
		}
		if(di) {
			di->Release();
			di = null;
		}
	}
	
	
	
	void Controls::Update() {
		
		
		// update keybourd
		
		res = diKeybourd->GetDeviceState(256, &keys);
		if(res != DI_OK) {
			do {
				res = diKeybourd->Acquire();
			} while(res == DIERR_INPUTLOST);
		}
		if(res != DI_OK) throw new DXException(res);
		
		
		// update mouse
		
		res = diMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
		if(res != DI_OK) {
			do {
				res = diMouse->Acquire();
			} while(res == DIERR_INPUTLOST);
		}
		if(res != DI_OK) throw new DXException(res);

		mousePos.x = (float)mouseState.lX;
		mousePos.y = (float)mouseState.lY;
		mousePos.z = (float)mouseState.lZ;
	}
	
	
	
	bool Controls::IsKeyDown(uint8 key) {
		return (keys[key] & 0x80) ? true : false;
	}
	
	
	
	bool Controls::IsMouseButtonDown(MouseButton button) {
		return (mouseState.rgbButtons[button] & 0x80) ? true : false;
	}
	
	
	
	D3DXVECTOR3 Controls::GetMousePosition() {
		return mousePos;
	}
	
	
}