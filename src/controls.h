/*----------------------------------------------------------------------------*\
  MODULE NAME: controls.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#ifndef F3DB_CONTROLS_H
#define F3DB_CONTROLS_H
#include "types.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dinput.h>
#include <d3dx9math.h>
#include "graphics.h"



namespace F3DB {
	
	
	enum MouseButton {
		F3DB_LMB = 0,
		F3DB_MMB = 2,
		F3DB_RMB = 1,
	};


	class Controls {
		
	public:
		
		Controls(Graphics *graphics);
		~Controls();

		void Init(Graphics *graphics = null);
		void CleanUp();
		void Reset(Graphics *graphics);
		
		void Update();
		
		bool IsKeyDown(uint8 key);
		bool IsMouseButtonDown(MouseButton button);
		D3DXVECTOR3 GetMousePosition();
		
	private:

		Graphics *graphics;
		HWND hWnd;
		HRESULT res;
		IDirectInput *di;
		IDirectInputDevice *diKeybourd, *diMouse;
		uint8 keys[256];
		DIMOUSESTATE mouseState;
		D3DXVECTOR3 mousePos;
	};
	
	
}



#endif // F3DB_CONTROLS_H