/*----------------------------------------------------------------------------*\
  MODULE NAME: graphics.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 9 2003
  DESCRIPTION: Graphics rendering functions
\*----------------------------------------------------------------------------*/
#ifndef F3DB_GRAPHICS_H
#define F3DB_GRAPHICS_H
#include "types.h"
#include <windows.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include "camera.h"

#define F3DB_WNDCLSNAME "F3DB_WNDCLS"



namespace F3DB {
	
	
	class Graphics {
		
	public:

		WNDPROC wndProc;
		
		Graphics(uint16 width, uint16 height, uint8 depth, bool fullscreen, cstr wndTitle, WNDPROC wndProc, Camera *camera, bool hardware);
		~Graphics();
		
		void Reset();
		void Reset(uint16 width, uint16 height, uint8 depth, bool fullscreen);
		
		D3DXMATRIXA16 AddWorldTransformMatrix(D3DXMATRIXA16 *matrix);
		void RestoreWorldTransformMatrix(D3DXMATRIXA16 *matrix);
		void SetWireframe(bool value);
		
		IDirect3DDevice9 *GetDevice();
		HWND GetWindowHandle();
		bool GetWireframe();
		
		void BeginFrame();
		void EndFrame();

		void Render2DTexture(IDirect3DTexture9 *texture); // renders fullscreen texture
		void Render2DTexture(IDirect3DTexture9 *texture, uint16 x, uint16 y, uint16 w, uint16 h);
		
	private:

		uint16 width, height;
		uint8 depth;
		bool fullscreen;
		Camera *camera;
		HWND hWnd;
		WNDCLASS wndClass;
		str wndTitle;
		bool wndClassRegistered;
		IDirect3D9 *d3d;
		D3DPRESENT_PARAMETERS d3dpp;
		IDirect3DDevice9 *d3dDevice;
		D3DXMATRIXA16 worldTransMtx;
		bool rendering;
		bool wireframeRendering;
		bool hardware;
		IDirect3DVertexBuffer9 *textureVb;
		
		void Init();
		void CleanUp();

		void Init2D();
		void CleanUp2D();
	};
	
	
}



#endif // #ifndef F3DB_GRAPHICS_H