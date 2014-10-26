/*----------------------------------------------------------------------------*\
  MODULE NAME: graphics.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 9 2003
  DESCRIPTION: Graphics rendering functions
\*----------------------------------------------------------------------------*/
#include "graphics.h"
#include <string.h>
#include "dxerror.h"
#include "scene.h"
#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "d3dx9.lib") 
#pragma comment(lib, "dxerr9.lib") 


namespace F3DB {
	
	
/*----------------------------------------------------------------------------*\
  Constructor
\*----------------------------------------------------------------------------*/
	
	
	Graphics::Graphics(uint16 width, uint16 height, uint8 depth, bool fullscreen, cstr wndTitle, WNDPROC wndProc, Camera *camera, bool hardware) {
		this->camera = camera;
		camera->viewChanged = true;
		camera->projChanged = true;
		this->wndProc = wndProc;
		rendering = false;
		D3DXMatrixIdentity(&worldTransMtx);
		this->width = width;
		this->height = height;
		this->depth = depth;
		this->fullscreen = fullscreen;
		wndClassRegistered = false;
		d3d = null;
		d3dDevice = null;
		hWnd = null;
		this->wndTitle = new char[strlen(wndTitle) + 1];
		strcpy(this->wndTitle, wndTitle);
		wireframeRendering = false;
		this->hardware = hardware;
		Init();
	}
	
	
	
/*----------------------------------------------------------------------------*\
  Destructor
\*----------------------------------------------------------------------------*/
	
	
	Graphics::~Graphics() {
		CleanUp();
	}
	
	
	
/*----------------------------------------------------------------------------*\
  Init() function
\*----------------------------------------------------------------------------*/
	
	
	void Graphics::Init() {
		
		HRESULT res;
		D3DDISPLAYMODE d3ddm;
		D3DFORMAT d3df;
		RECT clientRect;
		uint16 wndw, wndh, scrw, scrh;
		uint32 style;
		
		// init window
		
		wndClass.style			= CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc	= wndProc;
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hInstance		= GetModuleHandle(NULL);
		wndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName	= NULL;
		wndClass.lpszClassName	= F3DB_WNDCLSNAME;
		
		if(!RegisterClass(&wndClass)) throw new DXException("Failed to register main window class!");
		wndClassRegistered = true;

		scrw = GetSystemMetrics(SM_CXSCREEN);
		scrh = GetSystemMetrics(SM_CYSCREEN);
		
		if(fullscreen) {
			wndw = scrw;
			wndh = scrh;
			style = WS_POPUP;
		} else {
			wndw = width;
			wndh = height;
			style = WS_OVERLAPPED | WS_SYSMENU;
		}

		hWnd = CreateWindow(F3DB_WNDCLSNAME, wndTitle, style, 0, 0, wndw, wndh, null, null, wndClass.hInstance, null);
		if(!hWnd) throw new DXException("Failed to create main window!");
		
		if(!fullscreen){
			GetClientRect(hWnd, &clientRect);
			MoveWindow(hWnd, (scrw - wndw)/2, (scrh - wndh)/2, wndw, wndh, false);
		}
		
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		SetFocus(hWnd);
		
		ShowCursor(false);
		
		
		// init Direct3D
		
		if(!(d3d = Direct3DCreate9(D3D_SDK_VERSION))) throw new DXException("Could not create Direct3D Object");
		
		d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
		d3df = d3ddm.Format;
		if(fullscreen) {
			if(depth == 16) d3df = D3DFMT_R5G6B5;
			if(depth == 32) d3df = D3DFMT_X8R8G8B8;
		}
		
		memset(&d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));
		
		d3dpp.Windowed = !fullscreen;
		d3dpp.hDeviceWindow = hWnd;
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferFormat = d3df;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		if(fullscreen) {
			d3dpp.BackBufferWidth = width;
			d3dpp.BackBufferHeight = height;
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		}
		
		res = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice);
		//res = d3d->CreateDevice(D3DADAPTER_DEFAULT, hardware ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF, hWnd, hardware ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice);
		if(res != D3D_OK) throw new DXException("Could not create Direct3D Device! Possible cause: bad display settings, please check them.", res);
		
		
		// set render and texture stages
		
		res = d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 
		if(res == D3D_OK) res = d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		if(res == D3D_OK) res = d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		if(res != D3D_OK) throw new DXException("Failed to set one of the render or texture stage states!", res);


		// Init 2D

		Init2D();
	}
	
	
	
/*----------------------------------------------------------------------------*\
  CleanUp() function
\*----------------------------------------------------------------------------*/
	
	void Graphics::CleanUp() {

		// cleanup 2d
		CleanUp2D();
		
		// final Direct3D
		if(d3dDevice) {
			d3dDevice->Release();
			d3dDevice = null;
		}
		if(d3d) {
			d3d->Release();
			d3d = null;
		}
		
		
		ShowCursor(true);
		
		// destroy window
		if(hWnd) {
			if(DestroyWindow(hWnd)) {
				MSG msg;
				while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) DispatchMessage(&msg);
			}
			hWnd = null;
		}
		if(wndClassRegistered) {
			UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
			wndClassRegistered = false;
		}
	}
	
	
	
/*----------------------------------------------------------------------------*\
  Reset() functions
\*----------------------------------------------------------------------------*/
	
	void Graphics::Reset() {
		CleanUp();
		camera->viewChanged = true;
		camera->projChanged = true;
		Init();
	}



	void Graphics::Reset(uint16 width, uint16 height, uint8 depth, bool fullscreen) {
		this->width = width;
		this->height = height;
		this->depth = depth;
		this->fullscreen = fullscreen;
		Reset();
	}
	
	
	
/*----------------------------------------------------------------------------*\
  AddWorldTransformMatrix() function
\*----------------------------------------------------------------------------*/
	
	D3DXMATRIXA16 Graphics::AddWorldTransformMatrix(D3DXMATRIXA16 *matrix) {
		
		HRESULT res;
		D3DXMATRIXA16 old = worldTransMtx;
		
		D3DXMatrixMultiply(&worldTransMtx, matrix, &worldTransMtx);
		
		res = d3dDevice->SetTransform(D3DTS_WORLD, &worldTransMtx);
		if(res != D3D_OK) throw new DXException("Failed to add worldTransMtx transformation matrix!", res);
		
		return old;
	}
	
	
	
/*----------------------------------------------------------------------------*\
  RestoreWorldTransformMatrix() function
\*----------------------------------------------------------------------------*/
	
	void Graphics::RestoreWorldTransformMatrix(D3DXMATRIXA16 *matrix) {
		
		HRESULT res;
		
		worldTransMtx = *matrix;
		
		res = d3dDevice->SetTransform(D3DTS_WORLD, matrix);
		if(res != D3D_OK) throw new DXException("Failed to restore worldTransMtx transformation matrix!", res);
	}
	
	
	
/*----------------------------------------------------------------------------*\
  SetWireframe() function
\*----------------------------------------------------------------------------*/
	
	void Graphics::SetWireframe(bool value) {
		if(value == wireframeRendering) return;
		HRESULT res;
		wireframeRendering = value;
		if(wireframeRendering) {
			res = d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); 
			if(res == D3D_OK) res = d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			if(res == D3D_OK) res = d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			if(res == D3D_OK) res = d3dDevice->SetRenderState(D3DRS_FILLMODE ,D3DFILL_WIREFRAME);
			if(res != D3D_OK) throw new DXException("Failed to enable wireframe rendering mode!", res);
		} else {
			res = d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 
			if(res == D3D_OK) res = d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			if(res == D3D_OK) res = d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			if(res == D3D_OK) res = d3dDevice->SetRenderState(D3DRS_FILLMODE ,D3DFILL_SOLID);
			if(res != D3D_OK) throw new DXException("Failed to disable wireframe rendering mode!", res);
		}
	}
	
	
	
/*----------------------------------------------------------------------------*\
  BeginFrame() function
\*----------------------------------------------------------------------------*/
	
	void Graphics::BeginFrame() {
		
		HRESULT res;
		D3DXMATRIXA16 matrix;
		
		if(rendering) EndFrame();
		
		D3DXMatrixIdentity(&matrix);
		res = d3dDevice->SetTransform(D3DTS_WORLD, &matrix);
		camera->UpdateCamera();
		if(res == D3D_OK && camera->viewChanged) {
			camera->viewChanged = false;
			camera->GetViewMatrix(matrix);
			res = d3dDevice->SetTransform(D3DTS_VIEW, &matrix);
		}
		if(res == D3D_OK && camera->projChanged) {
			camera->projChanged = false;
			camera->GetProjMatrix(matrix);
			res = d3dDevice->SetTransform(D3DTS_PROJECTION, &matrix);
		}
		if(res != D3D_OK) throw new DXException("Failed to set one of the transformation matrices!", res);
		
		res = d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 128), camera->GetFarClippingZ(), 0L);
		if(res != D3D_OK) throw new DXException("Failed to clear screen!", res);
		
		res = d3dDevice->BeginScene();
		if(res != D3D_OK) throw new DXException("Failed to begin scene", res);
		
		rendering = true;
	}
	
	
	
/*----------------------------------------------------------------------------*\
  EndFrame() function
\*----------------------------------------------------------------------------*/
	
	void Graphics::EndFrame() {
		
		HRESULT res;
		
		if(!rendering) BeginFrame();
		
		res = d3dDevice->EndScene();
		if(res != D3D_OK) throw new DXException("Failed to end scene!", res);
		
		res = d3dDevice->Present(null, null, null, null);
		if(res == D3DERR_DEVICELOST) {
			if(d3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
				Reset();
			}
			res = D3D_OK;
		}
		if(res != D3D_OK) throw new DXException("Failed to present scene!", res);
		
		rendering = false;
	}
	
	
	
/*----------------------------------------------------------------------------*\
  GetDevice() function
\*----------------------------------------------------------------------------*/
	
	
	IDirect3DDevice9 *Graphics::GetDevice() {
		return d3dDevice;
	}
	
	

/*----------------------------------------------------------------------------*\
  GetWindow() function
\*----------------------------------------------------------------------------*/
	
	
	HWND Graphics::GetWindowHandle() {
		return hWnd;
	}
	
	

/*----------------------------------------------------------------------------*\
  GetWireframe() function
\*----------------------------------------------------------------------------*/
	
	
	bool Graphics::GetWireframe() {
		return wireframeRendering;
	}
	

	
/*----------------------------------------------------------------------------*\
  RenderTexture() function
\*----------------------------------------------------------------------------*/
	
	
	void Graphics::Render2DTexture(IDirect3DTexture9 *texture) {
		Render2DTexture(texture, 0, 0, width, height);
	}



/*----------------------------------------------------------------------------*\
  RenderTexture() function
\*----------------------------------------------------------------------------*/
	
	
	void Graphics::Render2DTexture(IDirect3DTexture9 *texture, uint16 x, uint16 y, uint16 w, uint16 h) {
		
		HRESULT res;
		D3DXMATRIX matProjection;
		D3DXMATRIX matView;
		D3DXMATRIX matWorld;
		D3DXMATRIX matTranslation;
		D3DXMATRIX matScaling;
		D3DXMATRIX matTransform;
		D3DXMATRIX matOrtho;
		D3DXMATRIX matIdentity;
		float sx, sy;


		if(wireframeRendering) return;


		// Backup transformation matrices

		d3dDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
		d3dDevice->GetTransform(D3DTS_VIEW, &matView);
		d3dDevice->GetTransform(D3DTS_WORLD, &matWorld);


		// Setup an orthographic perspective

		D3DXMatrixOrthoLH(&matOrtho, (float)width, (float)height, 1.0f, 10.0f);
		D3DXMatrixIdentity(&matIdentity);
		d3dDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
		d3dDevice->SetTransform(D3DTS_VIEW, &matIdentity);


		// Get coordinates

		sx = (float)x - (float)width/2.0f;
		sy = -(float)y + (float)height/2.0f; 


		// Setup translation and scaling matrices

		D3DXMatrixScaling(&matScaling, (float)w, (float)h, 1.0f);
		D3DXMatrixTranslation(&matTranslation, sx, sy, 0.0f);
		matTransform = matScaling*matTranslation;
		d3dDevice->SetTransform(D3DTS_WORLD, &matTransform);


		// Set render and texture stage states
		d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		
		d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		d3dDevice->SetTexture(0, texture);

		d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		
		
		// Set stream source and FVF
		res = d3dDevice->SetStreamSource(0, textureVb, 0, sizeof(TexturedVertex));
		if(res != D3D_OK) throw new DXException("Failed to render texture!", res);
		res = d3dDevice->SetFVF(F3DB_TEXTUREDVERTEXFVF);
		if(res != D3D_OK) throw new DXException("Failed to render texture!", res);


		// Render texture

		res = d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		if(res != D3D_OK) throw new DXException("Failed to render texture!", res);


		// Restore original transformation matrices

		d3dDevice->SetTransform(D3DTS_PROJECTION, &matProjection);
		d3dDevice->SetTransform(D3DTS_VIEW, &matView);
		d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	}



/*----------------------------------------------------------------------------*\
  Init2D() function
\*----------------------------------------------------------------------------*/
	
	
	void Graphics::Init2D() {

		D3DVERTEXBUFFER_DESC reqDesc, desc;
		TexturedVertex *vertices;
		HRESULT res;
		
		// create vertex buffer
		textureVb = null;
		reqDesc.Format = D3DFMT_VERTEXDATA;
		reqDesc.Type = D3DRTYPE_VERTEXBUFFER;
		reqDesc.Usage = 0;
		reqDesc.Pool = D3DPOOL_DEFAULT;
		reqDesc.Size = 4*sizeof(TexturedVertex);
		reqDesc.FVF = F3DB_TEXTUREDVERTEXFVF;
		res = d3dDevice->CreateVertexBuffer(reqDesc.Size, reqDesc.Usage, reqDesc.FVF, reqDesc.Pool, &textureVb, null);
		if(res != D3D_OK) throw new DXException("Failed to create vertex buffer!", res);
		if(!textureVb) throw new DXException("Failed to create vertex buffer!");
		textureVb->GetDesc(&desc);
		if(memcmp(&desc, &reqDesc, sizeof(D3DVERTEXBUFFER_DESC)) != 0) {
			textureVb->Release();
			throw new DXException("Failed to create vertex buffer (the requested capabilities could not be met)!");
		}
		
		// init vertices
		res = textureVb->Lock(0, reqDesc.Size, (void**)&vertices, 0);
		if(res != D3D_OK) {
			textureVb->Release();
			throw new DXException("Failed to lock vertex buffer!", res);
		}
		vertices[0] = TexturedVertex(1.0f, -1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		vertices[1] = TexturedVertex(0.0f, -1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f);
		vertices[2] = TexturedVertex(1.0f,  0.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f);
		vertices[3] = TexturedVertex(0.0f,  0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);
		textureVb->Unlock();
	}



/*----------------------------------------------------------------------------*\
  CleanUp2D() function
\*----------------------------------------------------------------------------*/
	
	
	void Graphics::CleanUp2D() {
		if(textureVb) textureVb->Release();
	}
}
