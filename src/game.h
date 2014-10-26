/*----------------------------------------------------------------------------*\
  MODULE NAME: game.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Game class, derived from Scene, implements the basic Frogger game
\*----------------------------------------------------------------------------*/
#ifndef F3DB_GAME_H
#define F3DB_GAME_H
#include "timer.h"
#include "camera.h"
#include "scene.h"
#include "leaf.h"
#include "leafshadow.h"
#include "car.h"
#include "frog.h"
#include "controls.h"
#include "button.h"

#define F3DB_LEAF_MAX_SPEED 2.0f
#define F3DB_LEAF_MIN_SPEED 0.5f
#define F3DB_MIN_LEAVES 4
#define F3DB_MAX_LEAVES 12

#define F3DB_CAR_MAX_SPEED 8.0f
#define F3DB_CAR_MIN_SPEED 2.0f
#define F3DB_MIN_CARS 1
#define F3DB_MAX_CARS 4

#define F3DB_FROG_SPEED 5.0f
#define F3DB_FROG2CAM_DIST 9.0f
#define F3DB_FROG_RAD 1.3f // gedeelte van blad waarop kikker kan staan

#define F3DB_ENDSCENE_DURATION 2.0

#define F3DB_SCORE_STD 2
#define F3DB_SCORE_MIN 0
#define F3DB_SCORE_MAX 8


namespace F3DB {


	class Game: public Scene {

	public:

		Game(Timer *timer, Graphics *graphics, Camera *camera, Controls *controls);
		~Game();

		void Update(double time);

		void Render();

	private:

		Graphics *graphics;
		Camera *camera;
		Controls *controls;
		Timer *timer;

		// classes
		Class *terrainCls;
		Class *borderCls;
		Class *waterCls;
		Class *leafCls;
		Class *leafShadowCls;
		Class *carCls;
		Class *frogCls;

		// objects
		Object   *terrain;
		Object   *borderLeft;
		Object   *borderRight;
		Object   *water;
		LeafList leaves;
		LeafList visibleLeaves;
		LeafShadowList leafShadows;
		CarList  cars;
		CarList  visibleCars;
		Frog     *frog;

		Leaf	*frogLeaf;


		// states

		bool intro, wire, full, pause, clip, gameover, help, oldPause, win, lose;


		// buttons
		Button buttonEsc, buttonRet, buttonF1, buttonW, buttonF, buttonP, buttonC, buttonUp, buttonDown, buttonLeft, buttonRight;


		// textures

		IDirect3DTexture9 *textures[9];


		// etc...

		double endTime;
		uint8 score;


		// function to reset the game

		void Reset();
	};

}


#endif // #ifndef F3DB_GAME_H