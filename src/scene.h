/*----------------------------------------------------------------------------*\
  MODULE NAME: scene.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Scene object classes
\*----------------------------------------------------------------------------*/
#ifndef F3DB_SCENE_H
#define F3DB_SCENE_H
#include "types.h"
#include <d3dx9.h>
#include <d3dx9math.h>
#include "graphics.h"
#include <list>
using namespace std;

// FVF's
#define F3DB_BASICVERTEXFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define F3DB_TEXTUREDVERTEXFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)



namespace F3DB {
	
	
/*----------------------------------------------------------------------------*\
  BasicVertex FVF vertex structure
\*----------------------------------------------------------------------------*/
	
	
	class BasicVertex {

	public:

		BasicVertex(float x, float y, float z, uint32 color);

		float x, y, z;
		uint32 color;
	};
	

/*----------------------------------------------------------------------------*\
  TexturedVertex FVF vertex structure
\*----------------------------------------------------------------------------*/
	
	
	class TexturedVertex: public BasicVertex {

	public:

		TexturedVertex(float x, float y, float z, uint32 color, float u, float v);
	
		float u, v;
	};
	

/*----------------------------------------------------------------------------*\
  Scene
\*----------------------------------------------------------------------------*/
	
	class Scene {
		
	public:
		
		// forward declaration of Object	
		class Object;
		
		// object list type
		typedef list<Object*> ObjectList;
		
		
		// Class
		class Class {
			
			friend class Object;
			
		public:

			enum Alpha {
				F3DB_SCA_NONE = 0,
				F3DB_SCA_VERTEX = 1,
				F3DB_SCA_TEXTURE = 2,
			};
			
			Class(Graphics *graphics, const BasicVertex *vertices, uint16 numVertices, uint32 fvf, D3DPRIMITIVETYPE type, IDirect3DTexture9 *texture, Alpha alpha);
			Class(Graphics *graphics, const BasicVertex *vertices, uint16 numVertices, uint32 fvf, D3DPRIMITIVETYPE type, const uint16 *indices, uint16 numIndices, IDirect3DTexture9 *texture, Alpha alpha);
			virtual ~Class();
			
			void AddObject(Object *obj);
			void RemoveObject(Object *obj);
			
			virtual void Render();
			
		protected:
			
			ObjectList objs;
			Graphics *graphics;
			IDirect3DDevice9 *d3dDevice;
			uint16 size, numVertices, numIndices;
			uint32 fvf;
			D3DPRIMITIVETYPE type;
			IDirect3DVertexBuffer9 *vb;
			IDirect3DIndexBuffer9 *ib;
			IDirect3DTexture9 *texture;
			Alpha alpha;
			
			virtual void Init(const BasicVertex *vertices, uint16 numVertices);
			virtual void Init(const BasicVertex *vertices, uint16 numVertices, const uint16 *indices, uint16 numIndices);

		};
		
		// Object
		class Object {

			friend class Class;
			friend class Scene;
			
		public:
			
			Object(Class *cls);
			
			void SetPosition(D3DXVECTOR3 pos);
			void SetRotation(D3DXVECTOR3 rot);
			void SetScale(D3DXVECTOR3 scl);
			void SetScale(float scl);
			void SetMovement(D3DXVECTOR3 mov);
			void SetTurning(D3DXVECTOR3 trn);
			void SetVisible(bool vis);
			
			void AddPosition(D3DXVECTOR3 pos);
			void AddRotation(D3DXVECTOR3 rot);
			void AddScale(D3DXVECTOR3 scl);
			void AddScale(float scl);
			void AddMovement(D3DXVECTOR3 mov);
			void AddTurning(D3DXVECTOR3 trn);
			
			D3DXVECTOR3 GetPosition();
			D3DXVECTOR3 GetGlobalPosition();
			D3DXVECTOR3 GetRotation();
			D3DXVECTOR3 GetScale();
			D3DXVECTOR3 GetMovement();
			D3DXVECTOR3 GetTurning();
			bool IsVisible();
			
			void AddObject(Object *obj);
			void RemoveObject(Object *obj);
			
			virtual void Update(double time);
			void Render();

		private:
			
			void SetGlobalPosition(D3DXVECTOR3 delta);
			void CalcTransMtx();
			
			D3DXVECTOR3 pos;
			D3DXVECTOR3 gps; // global position
			D3DXVECTOR3 rot;
			D3DXVECTOR3 scl;
			D3DXVECTOR3 mov;
			D3DXVECTOR3 trn;
			bool transChanged;
			D3DXMATRIXA16 transMtx;
			Class *cls;
			ObjectList objs;
			bool vis;
		};
		

		// some methods from Scene itself

		void AddObject(Object *obj);
		void RemoveObject(Object *obj);
		
		virtual void Update(double time);
		virtual void Render();
		
		private:
			
			ObjectList objs;
	};


}
	

#endif // #ifndef F3DB_SCENE_H
