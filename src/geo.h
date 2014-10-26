/*----------------------------------------------------------------------------*\
  MODULE NAME: geo.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Contains the geometry data for ceveral objects
\*----------------------------------------------------------------------------*/
#ifndef F3DB_GEO_H
#define F3DB_GEO_H
#include "scene.h"


#define F3DB_TERRAIN_COLOR 0xFF579142
#define F3DB_BORDER_COLOR 0x5B000000
#define F3DB_WATER_COLOR 0x4F0058B0
#define F3DB_LEAF_COLOR 0xFF44850B
#define F3DB_CAR_COLOR 0xFFC5C5C5
#define F3DB_FROG_COLOR 0xFF226622

#define F3DB_NUM_TERRAIN_VERTICES 22
#define F3DB_NUM_BORDER_VERTICES 4
#define F3DB_NUM_WATER_VERTICES 4
#define F3DB_NUM_LEAF_VERTICES 4
#define F3DB_NUM_CAR_VERTICES 16
#define F3DB_NUM_CAR_INDICES 72
#define F3DB_NUM_FROG_VERTICES 8
#define F3DB_NUM_FROG_INDICES 36



namespace F3DB {

	namespace Geo {

		TexturedVertex terrain[] = {
			
			TexturedVertex( 30.0f,  0.0f,  0.0f, F3DB_TERRAIN_COLOR, 3.75f, 1.000000000f),
			TexturedVertex(-30.0f,  0.0f,  0.0f, F3DB_TERRAIN_COLOR, 0.00f, 1.000000000f),
			TexturedVertex( 30.0f,  0.0f, 11.5f, F3DB_TERRAIN_COLOR, 3.75f, 0.794642857f),
			TexturedVertex(-30.0f,  0.0f, 11.5f, F3DB_TERRAIN_COLOR, 0.00f, 0.794642857f),
			TexturedVertex( 30.0f,  0.5f, 12.0f, F3DB_TERRAIN_COLOR, 3.75f, 0.785714286f),
			TexturedVertex(-30.0f,  0.5f, 12.0f, F3DB_TERRAIN_COLOR, 0.00f, 0.785714286f),
			TexturedVertex( 30.0f,  0.5f, 20.0f, F3DB_TERRAIN_COLOR, 3.75f, 0.642857143f),
			TexturedVertex(-30.0f,  0.5f, 20.0f, F3DB_TERRAIN_COLOR, 0.00f, 0.642857143f),
			TexturedVertex( 30.0f,  0.0f, 20.5f, F3DB_TERRAIN_COLOR, 3.75f, 0.633928571f),
			TexturedVertex(-30.0f,  0.0f, 20.5f, F3DB_TERRAIN_COLOR, 0.00f, 0.633928571f),
			TexturedVertex( 30.0f,  0.0f, 22.0f, F3DB_TERRAIN_COLOR, 3.75f, 0.607142857f),
			TexturedVertex(-30.0f,  0.0f, 22.0f, F3DB_TERRAIN_COLOR, 0.00f, 0.607142857f),
			TexturedVertex( 30.0f, -3.0f, 22.0f, F3DB_TERRAIN_COLOR, 3.75f, 0.553571429f),
			TexturedVertex(-30.0f, -3.0f, 22.0f, F3DB_TERRAIN_COLOR, 0.00f, 0.553571429f),
			TexturedVertex( 30.0f, -3.0f, 28.0f, F3DB_TERRAIN_COLOR, 3.75f, 0.446428571f),
			TexturedVertex(-30.0f, -3.0f, 28.0f, F3DB_TERRAIN_COLOR, 0.00f, 0.446428571f),
			TexturedVertex( 30.0f,  0.0f, 28.0f, F3DB_TERRAIN_COLOR, 3.75f, 0.392857143f),
			TexturedVertex(-30.0f,  0.0f, 28.0f, F3DB_TERRAIN_COLOR, 0.00f, 0.392857143f),
			TexturedVertex( 30.0f,  0.0f, 30.0f, F3DB_TERRAIN_COLOR, 3.75f, 0.357142857f),
			TexturedVertex(-30.0f,  0.0f, 30.0f, F3DB_TERRAIN_COLOR, 0.00f, 0.357142857f),
			TexturedVertex( 30.0f, 20.0f, 30.0f, F3DB_TERRAIN_COLOR, 3.75f, 0.000000000f),
			TexturedVertex(-30.0f, 20.0f, 30.0f, F3DB_TERRAIN_COLOR, 0.00f, 0.000000000f),
		};
		
		BasicVertex border[] = {
			
			BasicVertex(0.0f,  0.0f,  30.0f, F3DB_BORDER_COLOR),
			BasicVertex(0.0f,  0.0f, -30.0f, F3DB_BORDER_COLOR),
			BasicVertex(0.0f, 23.0f,  30.0f, F3DB_BORDER_COLOR),
			BasicVertex(0.0f, 23.0f, -30.0f, F3DB_BORDER_COLOR),
		};
		
		TexturedVertex water[] = {
			
			TexturedVertex(120.0f, 0.0f, 0.0f, F3DB_WATER_COLOR, 10.0f, 1.0f),
			TexturedVertex(  0.0f, 0.0f, 0.0f, F3DB_WATER_COLOR,  0.0f, 1.0f),
			TexturedVertex(120.0f, 0.0f, 6.0f, F3DB_WATER_COLOR, 10.0f, 0.0f),
			TexturedVertex(  0.0f, 0.0f, 6.0f, F3DB_WATER_COLOR,  0.0f, 0.0f),
		};
		
		TexturedVertex leaf[] = {

			TexturedVertex( 1.0f, 0.0f, -1.0f, F3DB_LEAF_COLOR, 1.0f, 1.0f),
			TexturedVertex(-1.0f, 0.0f, -1.0f, F3DB_LEAF_COLOR, 0.0f, 1.0f),
			TexturedVertex( 1.0f, 0.0f,  1.0f, F3DB_LEAF_COLOR, 1.0f, 0.0f),
			TexturedVertex(-1.0f, 0.0f,  1.0f, F3DB_LEAF_COLOR, 0.0f, 0.0f),
		};

		BasicVertex car[] = {

			BasicVertex( 2.0f,  0.0f, -1.0f, F3DB_CAR_COLOR),
			BasicVertex(-2.0f,  0.0f, -1.0f, F3DB_CAR_COLOR),
			BasicVertex( 2.0f,  0.0f,  1.0f, F3DB_CAR_COLOR),
			BasicVertex(-2.0f,  0.0f,  1.0f, F3DB_CAR_COLOR),

			BasicVertex( 2.0f,  0.5f, -1.0f, F3DB_CAR_COLOR),
			BasicVertex(-2.0f,  0.5f, -1.0f, F3DB_CAR_COLOR),
			BasicVertex( 2.0f,  0.5f,  1.0f, F3DB_CAR_COLOR),
			BasicVertex(-2.0f,  0.5f,  1.0f, F3DB_CAR_COLOR),

			BasicVertex( 1.0f,  0.5f, -1.0f, F3DB_CAR_COLOR),
			BasicVertex(-1.5f,  0.5f, -1.0f, F3DB_CAR_COLOR),
			BasicVertex( 1.0f,  0.5f,  1.0f, F3DB_CAR_COLOR),
			BasicVertex(-1.5f,  0.5f,  1.0f, F3DB_CAR_COLOR),

			BasicVertex( 0.50f,  1.25f, -1.0f, F3DB_CAR_COLOR),
			BasicVertex(-1.25f,  1.25f, -1.0f, F3DB_CAR_COLOR),
			BasicVertex( 0.50f,  1.25f,  1.0f, F3DB_CAR_COLOR),
			BasicVertex(-1.25f,  1.25f,  1.0f, F3DB_CAR_COLOR),
		};

		uint16 carIndices[] = {

			0, 1, 4,
			1, 5, 4,
			8, 9, 12,
			9, 13, 12,

			3, 6, 7,
			2, 6, 3,
			11, 14, 15,
			10, 14, 11,

			3, 1, 2,
			1, 0, 2,
			2, 0, 6,
			0, 4, 6,
			6, 4, 10,
			4, 8, 10,
			10, 8, 14,
			8, 12, 14,
			14, 12, 15,
			12, 13, 15,
			15, 13, 11,
			13, 9, 11,
			11, 9, 7,
			9, 5, 7,
			7, 5, 3,
			5, 1, 3,
		};

		BasicVertex frog[] = {
			
			BasicVertex(-0.5f,  0.5f, -0.5f, F3DB_FROG_COLOR),
			BasicVertex(-0.5f,  0.5f,  0.5f, F3DB_FROG_COLOR),
			BasicVertex( 0.5f,  0.5f,  0.5f, F3DB_FROG_COLOR),
			BasicVertex( 0.5f,  0.5f, -0.5f, F3DB_FROG_COLOR),
			
			BasicVertex(-0.5f, -0.5f, -0.5f, F3DB_FROG_COLOR),
			BasicVertex(-0.5f, -0.5f,  0.5f, F3DB_FROG_COLOR),
			BasicVertex( 0.5f, -0.5f,  0.5f, F3DB_FROG_COLOR),
			BasicVertex( 0.5f, -0.5f, -0.5f, F3DB_FROG_COLOR),
		};
		
		uint16 frogIndices[] = {
			
			4, 0, 7,
			0, 3, 7,
				
			7, 3, 6,
			3, 2, 6,
				
			6, 2, 5,
			2, 1, 5,
				
			5, 1, 4,
			4, 1, 0,
				
			0, 1, 3,
			1, 2, 3,
				
			5, 4, 6,
			4, 7, 6,
		};
	}
}


#endif // #ifndef F3DB_GEO_H