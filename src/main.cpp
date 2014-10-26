/*----------------------------------------------------------------------------*\
MODULE NAME: main.cpp
PROJECT: Frogger
AUTHOR: Evert Bauwens
DATE: september 8 2003
DESCRIPTION: Main Frogger file
\*----------------------------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "game.h"
#include "camera.h"
#include "timer.h"
#include "log.h"
#include "exception.h"
#include "graphics.h"
#include "controls.h"



namespace F3DB {
	
	
/*----------------------------------------------------------------------------*\
  Global vars
\*----------------------------------------------------------------------------*/
	
	bool active = true;
	Camera *camera = null;
	Graphics *graphics = null;
	Game *game = null;
	Timer *timer = null;
	Log *log = null;
	Controls *controls = null;
	
	
/*----------------------------------------------------------------------------*\
  Activate function
\*----------------------------------------------------------------------------*/
	
	void Activate(bool active) {
		static wasPaused = false;
		if(F3DB::active == active) return;
		F3DB::active = active;
		if(active) timer->Pause(!wasPaused);
		else {
			wasPaused = timer->IsPaused();
			timer->Pause(true);
		}
	}

	
/*----------------------------------------------------------------------------*\
  WndProc function
\*----------------------------------------------------------------------------*/
	
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch(uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
			Activate(LOWORD(wParam) != WA_INACTIVE);
			break;
		case WM_KILLFOCUS:
			Activate(false);
			break;
		case WM_SETFOCUS:
			Activate(true);
			break;
		case WM_SYSCOMMAND:
			if(wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) if(active) return 0;
			break;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	
	
/*----------------------------------------------------------------------------*\
  Init() function
\*----------------------------------------------------------------------------*/
	
	void Init() {
		InitMath();
		timer = new Timer();
		camera = new Camera();
		graphics = new Graphics(640, 480, 32, true, "Frogger 3D Basic", WndProc, camera, true);
		controls = new Controls(graphics);
		game = new Game(timer, graphics, camera, controls);
	}
	
	
	
/*----------------------------------------------------------------------------*\
  CleanUp() function
\*----------------------------------------------------------------------------*/
	
	void CleanUp() {
		if(game) {
			delete game;
			game = null;
		}
		if(controls) {
			delete controls;
			controls = null;
		}
		if(graphics) {
			delete graphics;
			graphics = null;
		}
		if(camera) {
			delete camera;
			camera = null;
		}
		if(timer) {
			delete timer;
			timer = null;
		}
	}
	
	
	
/*----------------------------------------------------------------------------*\
  Update() function
\*----------------------------------------------------------------------------*/
	
	void Update() {
		controls->Update();
		timer->Update();
		game->Update(timer->GetInterval());
	}
	
	
	
/*----------------------------------------------------------------------------*\
  Render() function
\*----------------------------------------------------------------------------*/
	
	void Render() {
		graphics->BeginFrame();
		game->Render();
		graphics->EndFrame();
	}
	
	
}

	
	
/*----------------------------------------------------------------------------*\
  WinMain() function
\*----------------------------------------------------------------------------*/

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	bool running = true, error = false;
	MSG msg;

	try {
		F3DB::log = new F3DB::Log("log.txt", "Frogger 3D Basic Log");
	} catch(...) {
		MessageBox(null, "Could not create log file!", "Fatal Error", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}
	
	try {
		
		F3DB::Init();

		F3DB::timer->Pause(false);
		
		while(running) {
			
			if(!F3DB::active) {
				
				// application is suspended
				// handle messages the default way (the app will be frozen if there are no messages)
				if(!GetMessage(&msg, null, 0, 0)) {
					running = false;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				
			} else if(PeekMessage(&msg, null, 0, 0, PM_REMOVE)) {
				
				// application is running and is receiving a message
				// handle messages in a more performant way (the app will go on even if there are no messages)
				switch(msg.message) {
				case WM_QUIT:
					running = false;
					break;
				default:
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				
			} else {
				
				// application is running and there are no messages -> render a frame
				
				F3DB::Update();
				F3DB::Render();
			}
			
		}
		
	} catch(F3DB::Exception *e) {
		F3DB::log->Write(e->Message());
		delete e;
		error = true;
	} catch(...) {
		F3DB::log->Write("An unknown exception occured!");
		error = true;
	}
	
	try {
		F3DB::CleanUp();
	} catch(...) {
		F3DB::log->Write("An exception occured during clean up! The application might not be properly cleaned up!");
	}

	if(error) {
		F3DB::log->Write("Application terminated due to fatal error!");
		MessageBox(null, "The application is terminated due to a fatal error! Check \"log.txt\" for details.", "Fatal Error", MB_OK | MB_ICONEXCLAMATION);
	} else {
		F3DB::log->Write("Application normally terminated.");
	}
	
	if(F3DB::log) {} delete F3DB::log;

	return error ? 1 : 0;
}
