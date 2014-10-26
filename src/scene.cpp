/*----------------------------------------------------------------------------*\
  MODULE NAME: scene.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Scene object classes
\*----------------------------------------------------------------------------*/
#include "scene.h"
#include "dxerror.h"
#include <string.h>



namespace F3DB {
	
	
/*----------------------------------------------------------------------------*\
  BasicVertex FVF vertex structure
\*----------------------------------------------------------------------------*/
	

	BasicVertex::BasicVertex(float x, float y, float z, uint32 color) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->color = color;
	}



/*----------------------------------------------------------------------------*\
  TexturedVertex FVF vertex structure
\*----------------------------------------------------------------------------*/
	
	
	TexturedVertex::TexturedVertex(float x, float y, float z, uint32 color, float u, float v): BasicVertex(x, y, z, color) {
		this->u = u;
		this->v = v;
	}



/*----------------------------------------------------------------------------*\
  Scene::Class
\*----------------------------------------------------------------------------*/
	
	
	Scene::Class::Class(Graphics *graphics, const BasicVertex *vertices, uint16 numVertices, uint32 fvf, D3DPRIMITIVETYPE type, IDirect3DTexture9 *texture, Alpha alpha) {
		this->graphics = graphics;
		vb = null;
		ib = null;
		size = 0;
		this->fvf = fvf;
		this->type = type;
		this->texture = texture;
		d3dDevice = graphics->GetDevice();
		this->alpha = alpha;
		Init(vertices, numVertices);
	}
	
	
	
	Scene::Class::Class(Graphics *graphics, const BasicVertex *vertices, uint16 numVertices, uint32 fvf, D3DPRIMITIVETYPE type, const uint16 *indices, uint16 numIndices, IDirect3DTexture9 *texture, Alpha alpha) {
		this->graphics = graphics;
		vb = null;
		ib = null;
		size = 0;
		this->fvf = fvf;
		this->type = type;
		this->texture = texture;
		d3dDevice = graphics->GetDevice();
		this->alpha = alpha;
		Init(vertices, numVertices, indices, numIndices);
	}
	
	
	
	Scene::Class::~Class() {
		if(vb) vb->Release();
		if(ib) ib->Release();
		if(texture) texture->Release();
	}
	
	
	
	void Scene::Class::AddObject(Object *obj) {
		objs.push_back(obj);
		obj->gps = obj->pos;
	}
	
	
	
	void Scene::Class::RemoveObject(Object *obj) {
		objs.remove(obj);
	}
	
	
	
	void Scene::Class::Render() {

		HRESULT res;
		uint16 primitiveCount = ib ? numIndices : numVertices;


		// determine primitive count

		switch(type) {
		case D3DPT_TRIANGLELIST:
			primitiveCount /= 3;
			break;
		case D3DPT_TRIANGLESTRIP:
			primitiveCount -= 2;
			break;
		default:
			throw new DXException("Attempt to render an unsupported primitive type!");
		}


		// set states

		if(graphics->GetWireframe()) {
			
			// unset texture
			
			d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			
			// unset alpha
			
			d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			
		} else {
			
			// set alpha

			if(alpha == F3DB_SCA_NONE) {
				
				d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				
			} else {
				
				d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				
				d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				
				d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, (alpha == F3DB_SCA_VERTEX) ? D3DTA_DIFFUSE : D3DTA_TEXTURE);
			}
			
			if(texture) {
				
				// set texture
				
				d3dDevice->SetTexture(0, texture);
				
				d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				
			} else {
				
				// unset texture
				
				d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				
			}
		}


		// set stream source (= which VB to use) & FVF (=> disable shaders)

		res = d3dDevice->SetStreamSource(0, vb, 0, size/numVertices);
		if(res != D3D_OK) throw new DXException("Failed to render scene class!", res);
		res = d3dDevice->SetFVF(fvf);
		if(res != D3D_OK) throw new DXException("Failed to render scene class!", res);


		// render

		if(ib) {
			res = d3dDevice->SetIndices(ib);
			if(res != D3D_OK) throw new DXException("Failed to render scene class!", res);
			res = d3dDevice->DrawIndexedPrimitive(type, 0, 0, numVertices, 0, primitiveCount);
		} else {
			res = d3dDevice->DrawPrimitive(type, 0, primitiveCount);
		}
		if(res != D3D_OK) throw new DXException("Failed to render scene class!", res);


		// render child objects

		for(ObjectList::iterator i = objs.begin(); i != objs.end(); i ++) (*i)->Render();
	}



	void Scene::Class::Init(const BasicVertex *vertices, uint16 numVertices) {
		
		D3DVERTEXBUFFER_DESC reqDesc, desc;
		BasicVertex *buffer;
		HRESULT res;
		
		this->numVertices = numVertices;

		// determine size
		switch(fvf) {
		case F3DB_BASICVERTEXFVF:
			size = numVertices*sizeof(BasicVertex);
			break;
		case F3DB_TEXTUREDVERTEXFVF:
			size = numVertices*sizeof(TexturedVertex);
			break;
		}
		
		// create vertex buffer
		reqDesc.Format = D3DFMT_VERTEXDATA;
		reqDesc.Type = D3DRTYPE_VERTEXBUFFER;
		reqDesc.Usage = 0;
		reqDesc.Pool = D3DPOOL_DEFAULT;
		reqDesc.Size = size;
		reqDesc.FVF = fvf;
		res = d3dDevice->CreateVertexBuffer(reqDesc.Size, reqDesc.Usage, reqDesc.FVF, reqDesc.Pool, &vb, null);
		if(res != D3D_OK) throw new DXException("Failed to create vertex buffer!", res);
		if(!vb) throw new DXException("Failed to create vertex buffer!");
		vb->GetDesc(&desc);
		if(memcmp(&desc, &reqDesc, sizeof(D3DVERTEXBUFFER_DESC)) != 0) {
			vb->Release();
			throw new DXException("Failed to create vertex buffer (the requested capabilities could not be met)!");
		}
		
		// upload vertices
		res = vb->Lock(0, size, (void**)&buffer, 0);
		if(res != D3D_OK) {
			vb->Release();
			throw new DXException("Failed to lock vertex buffer!", res);
		}
		memcpy(buffer, vertices, size);
		vb->Unlock();
	}
	
	
	void Scene::Class::Init(const BasicVertex *vertices, uint16 numVertices, const uint16 *indices, uint16 numIndices) {
		
		D3DINDEXBUFFER_DESC reqDesc, desc;
		uint16 *buffer;
		uint16 size = numIndices*2;
		HRESULT res;
		
		Init(vertices, numVertices);
		
		// create index buffer
		this->numIndices = numIndices;
		reqDesc.Format = D3DFMT_INDEX16;
		reqDesc.Type = D3DRTYPE_INDEXBUFFER;
		reqDesc.Usage = 0;
		reqDesc.Pool = D3DPOOL_DEFAULT;
		reqDesc.Size = size;
		res = d3dDevice->CreateIndexBuffer(reqDesc.Size, reqDesc.Usage, reqDesc.Format, reqDesc.Pool, &ib, null);
		if(res != D3D_OK) {
			vb->Release();
			throw new DXException("Failed to create index buffer!", res);
		}
		if(!ib) {
			vb->Release();
			throw new DXException("Failed to create index buffer!");
		}
		ib->GetDesc(&desc);
		if(memcmp(&desc, &reqDesc, sizeof(D3DINDEXBUFFER_DESC)) != 0) {
			vb->Release();
			ib->Release();
			throw new DXException("Failed to create index buffer (the requested capabilities could not be met)!");
		}
		
		// upload indices
		res = ib->Lock(0, size, (void**)&buffer, 0);
		if(res != D3D_OK) {
			vb->Release();
			ib->Release();
			throw new DXException("Failed to lock index buffer!", res);
		}
		memcpy(buffer, indices, size);
		ib->Unlock();
	}
	
	
	
/*----------------------------------------------------------------------------*\
  Scene::Object
\*----------------------------------------------------------------------------*/
	
	Scene::Object::Object(Class *cls): pos(0.0f, 0.0f, 0.0f), gps(0.0f, 0.0f, 0.0f), rot(0.0f, 0.0f, 0.0f), scl(1.0f, 1.0f, 1.0f), mov(0.0f, 0.0f, 0.0f), trn(0.0f, 0.0f, 0.0f) {
		this->cls = cls;
		transChanged = true;
		vis = true;
		D3DXMatrixIdentity(&transMtx);
	}


	void Scene::Object::SetPosition(D3DXVECTOR3 pos) {
		transChanged = true;
		SetGlobalPosition(D3DXVECTOR3(pos.x - this->pos.x, pos.y - this->pos.y, pos.z - this->pos.z));
		this->pos.x = pos.x;
		this->pos.y = pos.y;
		this->pos.z = pos.z;
	}
	
	
	
	void Scene::Object::SetRotation(D3DXVECTOR3 rot) {
		transChanged = true;
		this->rot.x = rot.x;
		this->rot.y = rot.y;
		this->rot.z = rot.z;
	}
	
	
	
	void Scene::Object::SetScale(D3DXVECTOR3 scl) {
		transChanged = true;
		this->scl.x = scl.x;
		this->scl.y = scl.y;
		this->scl.z = scl.z;
	}
	
	
	
	void Scene::Object::SetScale(float scl) {
		transChanged = true;
		this->scl.x = scl;
		this->scl.y = scl;
		this->scl.z = scl;
	}
	
	
	
	void Scene::Object::SetMovement(D3DXVECTOR3 mov) {
		this->mov.x = mov.x;
		this->mov.y = mov.y;
		this->mov.z = mov.z;
	}
	
	
	
	void Scene::Object::SetTurning(D3DXVECTOR3 trn) {
		this->trn.x = trn.x;
		this->trn.y = trn.y;
		this->trn.z = trn.z;
	}



	void Scene::Object::SetVisible(bool vis) {
		this->vis = vis;
	}
	
	
	
	void Scene::Object::AddPosition(D3DXVECTOR3 pos) {
		transChanged = true;
		SetGlobalPosition(pos);
		this->pos.x += pos.x;
		this->pos.y += pos.y;
		this->pos.z += pos.z;
	}
	
	
	
	void Scene::Object::AddRotation(D3DXVECTOR3 rot) {
		transChanged = true;
		this->rot.x += rot.x;
		this->rot.y += rot.y;
		this->rot.z += rot.z;
	}
	
	
	
	void Scene::Object::AddScale(D3DXVECTOR3 scl) {
		transChanged = true;
		this->scl.x *= scl.x;
		this->scl.y *= scl.y;
		this->scl.z *= scl.z;
	}
	
	
	void Scene::Object::AddScale(float scl) {
		transChanged = true;
		this->scl.x *= scl;
		this->scl.y *= scl;
		this->scl.z *= scl;
	}
	
	
	
	void Scene::Object::AddMovement(D3DXVECTOR3 mov) {
		this->mov.x += mov.x;
		this->mov.y += mov.y;
		this->mov.z += scl.z;
	}
	
	
	void Scene::Object::AddTurning(D3DXVECTOR3 trn) {
		this->trn.x += trn.x;
		this->trn.y += trn.y;
		this->trn.z += trn.z;
	}
	
	
	D3DXVECTOR3 Scene::Object::GetPosition() {
		return pos;
	}
	
	
	
	D3DXVECTOR3 Scene::Object::GetGlobalPosition() {
		return gps;
	}
	
	
	
	D3DXVECTOR3 Scene::Object::GetRotation() {
		return rot;
	}
	
	
	
	D3DXVECTOR3 Scene::Object::GetScale() {
		return scl;
	}
	
	
	
	D3DXVECTOR3 Scene::Object::GetMovement() {
		return mov;
	}
	
	
	
	D3DXVECTOR3 Scene::Object::GetTurning() {
		return trn;
	}



	bool Scene::Object::IsVisible() {
		return vis;
	}
	
	
	
	void Scene::Object::AddObject(Object *obj) {
		objs.push_back(obj);
		obj->gps = obj->pos;
		obj->gps.x += gps.x;
		obj->gps.y += gps.y;
		obj->gps.z += gps.z;
	}
	
	
	
	void Scene::Object::RemoveObject(Object *obj) {
		objs.remove(obj);
	}
	
	
	
	void Scene::Object::Update(double time) {
		D3DXVECTOR3 nulv(0.0f, 0.0f, 0.0f);
		if(memcmp(&mov, &nulv, sizeof(D3DXVECTOR3)) != 0) AddPosition(D3DXVECTOR3(mov.x*time, mov.y*time, mov.z*time));
		if(memcmp(&trn, &nulv, sizeof(D3DXVECTOR3)) != 0) AddRotation(D3DXVECTOR3(trn.x*time, trn.y*time, trn.z*time));
		for(ObjectList::iterator i = objs.begin(); i != objs.end(); i ++) (*i)->Update(time);
	}



	void Scene::Object::Render() {
		
		D3DXMATRIXA16 transMtxBak;

		if(!vis) return;
		
		if(transChanged) {
			CalcTransMtx();
			transChanged = false;
		}
		
		transMtxBak = cls->graphics->AddWorldTransformMatrix(&transMtx);
		
		cls->Render();
		for(ObjectList::iterator i = objs.begin(); i != objs.end(); i ++) (*i)->Render();
		
		cls->graphics->RestoreWorldTransformMatrix(&transMtxBak);
	}



	void Scene::Object::SetGlobalPosition(D3DXVECTOR3 delta) {
		this->gps.x += delta.x;
		this->gps.y += delta.y;
		this->gps.z += delta.z;
		for(ObjectList::iterator i = objs.begin(); i != objs.end(); i ++) (*i)->SetGlobalPosition(delta);
	}
	
	
	
	void Scene::Object::CalcTransMtx() {
		
		D3DXMATRIXA16 helpMtx;
		
		// matS * matZ * matX * matY * matP
		
		// matS
		D3DXMatrixScaling(&transMtx, scl.x, scl.y, scl.z);
		
		// matZ
		D3DXMatrixRotationZ(&helpMtx, rot.z);
		
		// matS * matZ
		D3DXMatrixMultiply(&transMtx, &transMtx, &helpMtx);
		
		// matX
		D3DXMatrixRotationX(&helpMtx, rot.x);
		
		// (matS * matZ) * matX
		D3DXMatrixMultiply(&transMtx, &transMtx, &helpMtx);
		
		// matY
		D3DXMatrixRotationY(&helpMtx, rot.y);
		
		// (matS * matZ * matX) * matY
		D3DXMatrixMultiply(&transMtx, &transMtx, &helpMtx);
		
		// matP
		D3DXMatrixTranslation(&helpMtx, pos.x, pos.y, pos.z);
		
		// (matS * matZ * matX * matY) * matP
		D3DXMatrixMultiply(&transMtx, &transMtx, &helpMtx);
	}
	
	
	
/*----------------------------------------------------------------------------*\
  Scene
\*----------------------------------------------------------------------------*/
	
	void Scene::AddObject(Object *obj) {
		objs.push_back(obj);
		obj->gps = obj->pos;
	}
	
	void Scene::RemoveObject(Object *obj) {
		objs.remove(obj);
	}
	
	void Scene::Update(double time) {
		for(ObjectList::iterator i = objs.begin(); i != objs.end(); i ++) (*i)->Update(time);
	}

	void Scene::Render() {
		for(ObjectList::iterator i = objs.begin(); i != objs.end(); i ++) (*i)->Render();
	}
	
	
}