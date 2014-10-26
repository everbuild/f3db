/*----------------------------------------------------------------------------*\
  MODULE NAME: game.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Game class, derived from Scene, implements the basic Frogger game
\*----------------------------------------------------------------------------*/
#include "game.h"
#include "dxerror.h"
#include "geo.h"
#include "math.h"


namespace F3DB {

	Game::Game(Timer *timer, Graphics *graphics, Camera *camera, Controls *controls):
			buttonEsc(controls, DIK_ESCAPE, F3DB_BT_TRIGGER),
			buttonRet(controls, DIK_RETURN, F3DB_BT_TRIGGER),
			buttonF1(controls, DIK_F1, F3DB_BT_TRIGGER),
			buttonW(controls, DIK_Z, F3DB_BT_TRIGGER),
			buttonF(controls, DIK_F, F3DB_BT_TRIGGER),
			buttonP(controls, DIK_P, F3DB_BT_TRIGGER),
			buttonC(controls, DIK_C, F3DB_BT_TRIGGER),
			buttonUp(controls, DIK_UP, F3DB_BT_KEY),
			buttonDown(controls, DIK_DOWN, F3DB_BT_KEY),
			buttonLeft(controls, DIK_LEFT, F3DB_BT_KEY),
			buttonRight(controls, DIK_RIGHT, F3DB_BT_KEY)
		{

		HRESULT res;
		IDirect3DDevice9 *d3dDevice;
		IDirect3DTexture9 *texture;
		LeafShadow *leafShadow;
		Leaf *leaf;
		Car *car;
		int16 i, j;
		uint8 n;
		LeafList::iterator li;
		LeafShadowList::iterator lsi;
		CarList::iterator ci;
		float speed, dir;
		D3DXVECTOR3 v;

		// init some members

		this->timer = timer;
		this->graphics = graphics;
		this->camera   = camera;
		this->controls = controls;

		terrainCls  = null;
		borderCls   = null;
		waterCls    = null;
		leafCls     = null;
		carCls      = null;
		frogCls     = null;

		terrain     = null;
		borderLeft  = null;
		borderRight = null;
		water       = null;
		car         = null;
		frog        = null;

		frogLeaf = null;

		intro = true;
		wire = false;
		full = false;
		pause = true;
		clip = true;
		gameover = false;
		help = false;
		win = false;
		lose = false;

		score = F3DB_SCORE_STD;

		buttonF.Enable(false);

		d3dDevice = graphics->GetDevice();


		// load textures

		res = D3DXCreateTextureFromFile(d3dDevice, "images/controls.png", &textures[0]);
		if(res != D3D_OK) throw new DXException("Failed to load image \"controls.png\"!", res);

		res = D3DXCreateTextureFromFile(d3dDevice, "images/dead.png", &textures[1]);
		if(res != D3D_OK) throw new DXException("Failed to load image \"dead.png\"!", res);

		res = D3DXCreateTextureFromFile(d3dDevice, "images/finish.png", &textures[2]);
		if(res != D3D_OK) throw new DXException("Failed to load image \"finish.png\"!", res);

		res = D3DXCreateTextureFromFile(d3dDevice, "images/frog.png", &textures[3]);
		if(res != D3D_OK) throw new DXException("Failed to load image \"frog.png\"!", res);

		res = D3DXCreateTextureFromFile(d3dDevice, "images/gameover.png", &textures[4]);
		if(res != D3D_OK) throw new DXException("Failed to load image \"gameover.png\"!", res);

		res = D3DXCreateTextureFromFile(d3dDevice, "images/intro.jpg", &textures[5]);
		if(res != D3D_OK) throw new DXException("Failed to load image \"intro.jpg\"!", res);

		res = D3DXCreateTextureFromFile(d3dDevice, "images/pause.png", &textures[6]);
		if(res != D3D_OK) throw new DXException("Failed to load image \"pause.png\"!", res);

		res = D3DXCreateTextureFromFile(d3dDevice, "images/win.png", &textures[7]);
		if(res != D3D_OK) throw new DXException("Failed to load image \"win.png\"!", res);

		res = D3DXCreateTextureFromFile(d3dDevice, "images/bar.png", &textures[8]);
		if(res != D3D_OK) throw new DXException("Failed to load image \"bar.png\"!", res);

		
		// set camera position

		camera->position.y = 7.65f;
		camera->position.z = 0.0f;
		camera->rotation.x = 0.5f;
		
		
		// create classes

		// terrain
		res = D3DXCreateTextureFromFile(d3dDevice, "images/terrain.jpg", &texture);
		if(res != D3D_OK) throw new DXException("Failed to load image \"terrain.jpg\"!", res);
		terrainCls = new Class(graphics, Geo::terrain, F3DB_NUM_TERRAIN_VERTICES, F3DB_TEXTUREDVERTEXFVF, D3DPT_TRIANGLESTRIP, texture, Class::F3DB_SCA_NONE);

		// border
		borderCls = new Class(graphics, Geo::border, F3DB_NUM_BORDER_VERTICES, F3DB_BASICVERTEXFVF, D3DPT_TRIANGLESTRIP, null, Class::F3DB_SCA_VERTEX);

		// water
		res = D3DXCreateTextureFromFile(d3dDevice, "images/water.jpg", &texture);
		if(res != D3D_OK) throw new DXException("Failed to load image \"water.jpg\"!", res);
		waterCls = new Class(graphics, Geo::water, F3DB_NUM_WATER_VERTICES, F3DB_TEXTUREDVERTEXFVF, D3DPT_TRIANGLESTRIP, texture, Class::F3DB_SCA_VERTEX);

		// leaf
		res = D3DXCreateTextureFromFile(d3dDevice, "images/leaf.png", &texture);
		if(res != D3D_OK) throw new DXException("Failed to load image \"leaf.png\"!", res);
		leafCls = new Class(graphics, Geo::leaf, F3DB_NUM_LEAF_VERTICES, F3DB_TEXTUREDVERTEXFVF, D3DPT_TRIANGLESTRIP, texture, Class::F3DB_SCA_TEXTURE);

		// leaf shadow
		res = D3DXCreateTextureFromFile(d3dDevice, "images/leaf_shadow.png", &texture);
		if(res != D3D_OK) throw new DXException("Failed to load image \"leaf_shadow.png\"!", res);
		leafShadowCls = new Class(graphics, Geo::leaf, F3DB_NUM_LEAF_VERTICES, F3DB_TEXTUREDVERTEXFVF, D3DPT_TRIANGLESTRIP, texture, Class::F3DB_SCA_TEXTURE);

		// car
		carCls = new Class(graphics, Geo::car, F3DB_NUM_CAR_VERTICES, F3DB_BASICVERTEXFVF, D3DPT_TRIANGLELIST, Geo::carIndices, F3DB_NUM_CAR_INDICES, null, Class::F3DB_SCA_NONE);

		// frog
		frogCls = new Class(graphics, Geo::frog, F3DB_NUM_FROG_VERTICES, F3DB_BASICVERTEXFVF, D3DPT_TRIANGLELIST, Geo::frogIndices, F3DB_NUM_FROG_INDICES, null, Class::F3DB_SCA_NONE);


		// create objects

		// terrain
		terrain = new Object(terrainCls);

		// border
		borderLeft = new Object(borderCls);
		borderLeft->SetPosition(D3DXVECTOR3(-5.0f, -3.0f, 0.0f));
		borderRight = new Object(borderCls);
		borderRight->SetRotation(D3DXVECTOR3(0.0f, 3.1415f, 0.0f));
		borderRight->SetPosition(D3DXVECTOR3(5.0f, -3.0f, 0.0f));

		// water
		water = new Object(waterCls);
		water->SetPosition(D3DXVECTOR3(-30.0f, -0.8f, 22.0f));
		water->SetMovement(D3DXVECTOR3(-2.0f, 0.0f, 0.0f));

		// leaves (& shadows)
		n = F3DB_MAX_LEAVES*3;
		for(i = 0; i < n; i ++) {
			leaf = new Leaf(leafCls);
			leaves.push_back(leaf);
			leafShadow = new LeafShadow(leafShadowCls, leaf);
			leafShadows.push_back(leafShadow);
		}

		// cars
		n = F3DB_MAX_CARS*4;
		for(i = 0; i < n; i ++) {
			car = new Car(carCls);
			car->SetScale(0.8f);
			cars.push_back(car);
		}

		// frog
		frog = new Frog(frogCls);
		frog->SetPosition(D3DXVECTOR3(0.0f, 0.5f, 9.0f));


		// add objects to scene

		AddObject(terrain);
		for(lsi = leafShadows.begin(); lsi != leafShadows.end(); lsi ++) AddObject((*lsi));
		AddObject(water);
		for(li = leaves.begin(); li != leaves.end(); li ++) AddObject((*li));
		for(ci = cars.begin(); ci != cars.end(); ci ++) AddObject((*ci));
		AddObject(frog);
		AddObject(borderLeft);
		AddObject(borderRight);


		// start leaves

		dir = Random((int32)0, (int32)1) ? -1.0f : 1.0f;
		for(i = 0; i < 3; i ++) {
			n = Random((int32)F3DB_MIN_LEAVES, (int32)F3DB_MAX_LEAVES);
			dir = -dir;
			speed = Random(F3DB_LEAF_MIN_SPEED, F3DB_LEAF_MAX_SPEED)*dir;
			for(j = 0; j < n; j ++) {
				leaf = leaves.back();
				leaves.remove(leaf);
				leaf->Start(i, speed);
				v = leaf->GetPosition();
				v.x += Random(0.0f, 60.0f)*dir;
				leaf->SetPosition(v);
				visibleLeaves.push_back(leaf);
			}
		}


		// start cars

		for(i = 0; i < 4; i ++) {
			n = Random((int32)F3DB_MIN_CARS, (int32)F3DB_MAX_CARS);
			dir = (i < 2) ? 1.0f : -1.0f;
			speed = Random(F3DB_CAR_MIN_SPEED, F3DB_CAR_MAX_SPEED)*dir;
			for(j = 0; j < n; j ++) {
				car = cars.back();
				cars.remove(car);
				car->Start(i, speed);
				v = car->GetPosition();
				v.x += Random(0.0f, 60.0f)*dir;
				car->SetPosition(v);
				if(dir < 0.0f) car->SetRotation(D3DXVECTOR3(0.0f, 3.1415f, 0.0f));
				visibleCars.push_back(car);
			}
		}

	}

	
	Game::~Game() {
		delete frog;
		for(CarList::iterator ci = cars.begin(); ci != cars.end(); ci ++) delete *ci;
		for(LeafShadowList::iterator lsi = leafShadows.begin(); lsi != leafShadows.end(); lsi ++) delete *lsi;
		for(LeafList::iterator li = leaves.begin(); li != leaves.end(); li ++) delete *li;
		delete water;
		delete terrain;
		delete frogCls;
		delete carCls;
		delete leafCls;
		delete leafShadowCls;
		delete waterCls;
		delete terrainCls;
		for(uint8 i = 0; i < 7; i ++) textures[i]->Release();
	}


	void Game::Update(double time) {

		Leaf *leaf, *closestLeaf;
		Car *car;
		D3DXVECTOR3 p1, p2, v;
		uint8 lane;
		CarList::iterator ci;
		LeafList::iterator li;
		float x, dist, minDist;


		// handle buttons

		if(buttonEsc.IsActive()) {
			if(help) {
				help = false;
				timer->Pause(oldPause);
			} else {
				PostQuitMessage(0);
			}
		}

		if(buttonF1.IsActive()) {
			if(help) {
				help = false;
				timer->Pause(oldPause);
			} else {
				help = true;
				oldPause = timer->IsPaused();
				timer->Pause(true);
			}
		}

		if(intro) {
			if(buttonRet.IsActive()) {
				intro = false;
				timer->Pause(false);
				pause = false;
			} else {
				timer->Pause(true);
				return;
			}
		}

		if(buttonW.IsActive()) graphics->SetWireframe((wire = !wire));
		if(buttonF.IsActive()) {
			controls->CleanUp();
			graphics->Reset(640, 480, 16, (full = !full));
			controls->Init();
		}
		if(buttonP.IsActive()) {
			timer->Pause((pause = !pause));
			if(pause) {
				buttonUp.Enable(false);
				buttonDown.Enable(false);
				buttonLeft.Enable(false);
				buttonRight.Enable(false);
			} else {
				buttonUp.Enable(true);
				buttonDown.Enable(true);
				buttonLeft.Enable(true);
				buttonRight.Enable(true);
			}
		}
		if(buttonC.IsActive()) clip = !clip;


		// handle movement buttons

		if(!gameover) {
			v = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if(buttonUp.IsActive())    v.z += F3DB_FROG_SPEED;
			if(buttonDown.IsActive())  v.z -= F3DB_FROG_SPEED;
			if(buttonRight.IsActive()) v.x += F3DB_FROG_SPEED;
			if(buttonLeft.IsActive())  v.x -= F3DB_FROG_SPEED;
			frog->SetMovement(v);
		}


		// update base class

		Scene::Update(time);


		if(!gameover) {
			
			// clipping (make sure the frog doesn't escape from his cage! ;-)
			
			p1 = frog->GetPosition();
			p2 = frog->GetGlobalPosition();
			if(clip) {
				if(p2.x < -4.5f) p1.x -= p2.x - -4.5f;
				if(p2.x >  4.5f) p1.x -= p2.x -  4.5f;
				if(p2.z <  9.0f) p1.z -= p2.z -  9.0f;
				if(p2.z > 29.5f) p1.z -= p2.z - 29.5f;
				frog->SetPosition(p1);
			}
			
			
			// update camera position
			
			camera->position.z = frog->GetGlobalPosition().z - F3DB_FROG2CAM_DIST;
			if(clip) camera->position.x = 0.0f;
			else camera->position.x = frog->GetGlobalPosition().x;
			
		}

		
		// update water

		v = water->GetPosition();
		if(v.x <= -90.0f) {
			v.x += 60.0f;
			water->SetPosition(v);
		}


		// update leaves

		for(li = visibleLeaves.begin(); li != visibleLeaves.end();) {
			leaf = *li;
			li ++;
			if(!leaf->started) {
				switch(Random((int32)1, (int32)3)) {
				case 1:
					// remove leaf
					visibleLeaves.remove(leaf);
					leaves.push_back(leaf);
					break;
				case 2:
					// relocate leaf
					v = leaf->GetMovement();
					lane = leaf->lane;
					leaf->Start(lane, v.x);
					break;
				case 3:
					// relocate leaf and add new one
					v = leaf->GetMovement();
					lane = leaf->lane;
					leaf->Start(lane, v.x);
					if(leaves.size() > 0) {
						leaf = leaves.back();
						leaves.remove(leaf);
						leaf->Start(lane, v.x);
						visibleLeaves.push_back(leaf);
					}
					break;
				}
			}

			// avoid collisions with other leaves

			leaf->AvoidCollisions(visibleLeaves);
		}

		
		// update cars

		for(ci = visibleCars.begin(); ci != visibleCars.end();) {
			
			car = *ci;
			ci ++;
			
			// remove, relocate, or add a car

			if(!car->started) {
				switch(Random((int32)1, (int32)3)) {
				case 1:
					// remove car
					visibleCars.remove(car);
					cars.push_back(car);
					break;
				case 2:
					// relocate car
					v = car->GetMovement();
					lane = car->lane;
					car->Start(lane, v.x);
					break;
				case 3:
					// relocate car and add new one
					v = car->GetMovement();
					lane = car->lane;
					car->Start(lane, v.x);
					if(cars.size() > 0) {
						car = cars.back();
						cars.remove(car);
						if(v.x < 0.0f) car->SetRotation(D3DXVECTOR3(0.0f, 3.1415f, 0.0f));
						else car->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						car->Start(lane, v.x);
						visibleCars.push_back(car);
					}
					break;
				}
			}

			// avoid collisions with other cars

			car->AvoidCollisions(visibleCars);
		}


		if(!gameover && clip) {
			
			// detect collision of frog with cars
			
			x = frog->GetGlobalPosition().z;
			if(x >= 11.5f && x < 20.5f) {
				for(ci = visibleCars.begin(); ci != visibleCars.end(); ci ++) {
					car = *ci;
					if(car->Collide(frog, 1.0f, 1.0f)) {
						p1 = frog->GetGlobalPosition();
						p2 = car->GetGlobalPosition();
						x = (p1.x - p2.x)*car->GetDirection();
						if(x > 2.5f) continue;
						if(x <= -2.5f) continue;
						frog->status = Frog::F3DB_FS_DYING;
						score --;
						break;
					}
				}
			}
			
			
			// het beek-gedeelte = detect 'colliosion' with leaves and more...
			
			x = frog->GetGlobalPosition().z;
			if(x >= 22.5f && x < 27.5f) {
				if(frogLeaf) {
					v = frog->GetPosition();
					dist = sqrtf(v.x*v.x + v.z*v.z);
					if(dist >= F3DB_FROG_RAD) {
						v = frog->GetGlobalPosition();
						frogLeaf->RemoveObject(frog);
						frogLeaf = null;
						AddObject(frog);
						frog->SetPosition(v);
						frog->status = Frog::F3DB_FS_IDLE;
					}
				}
				if(!frogLeaf) {
					minDist = 1000.0f;
					closestLeaf = null;
					for(li = visibleLeaves.begin(); li != visibleLeaves.end(); li ++) {
						leaf = *li;
						if(leaf->Collide(frog, 1.0f, 1.0f)) {
							p1 = frog->GetPosition();
							p2 = leaf->GetPosition();
							v.x = p1.x - p2.x;
							v.z = p1.z - p2.z;
							dist = sqrtf(v.x*v.x + v.z*v.z);
							if(dist < minDist) {
								minDist = dist;
								closestLeaf = leaf;
							}
						}
					}
					if(minDist < F3DB_FROG_RAD) {
						RemoveObject(frog);
						frogLeaf = closestLeaf;
						p1 = frog->GetPosition();
						p2 = frogLeaf->GetPosition();
						v.x = p1.x - p2.x;
						v.z = p1.z - p2.z;
						v.y = 0.5f;
						frog->SetPosition(v);
						frogLeaf->AddObject(frog);
						frog->status = Frog::F3DB_FS_ONLEAF;
					}
				}
				if(!frogLeaf) {
					frog->status = Frog::F3DB_FS_DRAWNING; // kikker verdinkt!
					score --;
				}
			} else {
				if(frogLeaf) {
					v = frog->GetGlobalPosition();
					frogLeaf->RemoveObject(frog);
					frogLeaf = null;
					AddObject(frog);
					frog->SetPosition(v);
					frog->status = Frog::F3DB_FS_IDLE;
				}
			}


			// finished ?

			if(frog->GetGlobalPosition().z > 29.0f) {
				frog->status = Frog::F3DB_FS_ATFINISH;
				score ++;
			}
		}


		// game over animations etc..

		if(gameover) {
			endTime += time;
			if(endTime >= F3DB_ENDSCENE_DURATION) Reset();
		} else {
			if(score == F3DB_SCORE_MAX) win = true;
			if(score == F3DB_SCORE_MIN) lose = true;
			switch(frog->status) {
			case Frog::F3DB_FS_ATFINISH:
				gameover = true;
				frog->SetMovement(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				endTime = 0.0f;
				break;
			case Frog::F3DB_FS_DYING:
				gameover = true;
				frog->SetMovement(D3DXVECTOR3(20.0f, 100.0f, 50.0f));
				endTime = 0.0f;
				break;
			case Frog::F3DB_FS_DRAWNING:
				gameover = true;
				frog->SetMovement(D3DXVECTOR3(0.0f, -0.5f, 0.0f));
				endTime = 0.0f;
				break;
			}
		}
	}


	
	void Game::Render() {
		Scene::Render();
		if(intro) {
			graphics->Render2DTexture(textures[5]);
		} else {
			graphics->Render2DTexture(textures[8], 0, 0, 640, 50);
			for(uint8 i = 0; i < score; i ++) graphics->Render2DTexture(textures[3], 590 - i*40, 0, 50, 50);
		}
		if(help) {
			graphics->Render2DTexture(textures[0]);
			return;
		}
		if(intro) return;
		if(win) {
			graphics->Render2DTexture(textures[7]);
		} else if(lose) {
			graphics->Render2DTexture(textures[4]);
		} else if(gameover) {
			switch(frog->status) {
			case Frog::F3DB_FS_ATFINISH:
				graphics->Render2DTexture(textures[2]);
				break;
			case Frog::F3DB_FS_DYING:
			case Frog::F3DB_FS_DRAWNING:
				graphics->Render2DTexture(textures[1]);
				break;
			}
		}
		if(pause) graphics->Render2DTexture(textures[6]);
	}



	void Game::Reset() {
		gameover = false;
		if(win || lose) {
			win = false;
			lose = false;
			score = F3DB_SCORE_STD;
		}
		frog->SetPosition(D3DXVECTOR3(0.0f, 0.5f, 9.0f));
		frog->status = Frog::F3DB_FS_IDLE;
	}

};