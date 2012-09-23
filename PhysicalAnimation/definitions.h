//
//  definitions.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/3/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef PhysicalAnimation_definitions_h
#define PhysicalAnimation_definitions_h

#include "object.h"
#include "particle_manager.h"
#include "Vector.h"
#include <vector>

// colors in RGB
#define RGBBLACK	0, 0, 0
#define RGBRED		1, 0, 0
#define RGBORANGE	1, 0.5, 0
#define RGBYELLOW	1, 1, 0
#define RGBGREEN	0, 1, 0
#define RGBBLUE		0.5, 0.5, 1
#define RGBVIOLET	1, 0, 0.5
#define RGBWHITE	1, 1, 1


#define EPS		0.1

#define MAXSTEPS	10000

#define MenuContinuous	1
#define MenuThrow	2
#define MenuTrace	3
#define MenuWeight	4
#define MenuWind	5
#define MenuFloor	6
#define MenuReset	7
#define MenuClean	8
#define MenuQuit	9

#define NOAIR		0
#define LIGHT		1
#define MEDIUM		2
#define HEAVY		3


//---------for test -------
#define Trace true
#define WINDOW_WIDTH	1000	/* window dimensions */
#define WINDOW_HEIGHT	740
#define STARTX		(WINDOW_WIDTH / 20)
#define STARTY		(WINDOW_HEIGHT - WINDOW_HEIGHT / 20)
#define CIRC_INC	(2 * PI / 30)	/* fineness of circle drawing */

static Vector2d Ball(STARTX, STARTY);
//static int NSteps = 0;
//static int NTimeSteps = -1;
static Vector2d OldBall[MAXSTEPS];

static double WinWidth = WINDOW_WIDTH;
static double WinHeight = WINDOW_HEIGHT;
static int Step = true;
static int Start = true;
static int Stopped = true;
static int MiddleButton = false;
static int Throw = false;
//static int WeightMatters = 0;
static int HaveWind = false;
static int Floor = false;

particle_manager::ParticleManager particle_manager1 ( 10,
                                                     Vector3d( -2.0,5.0, 2.0),
                                                     4.0, 2.0,
                                                     Vector3d( 0.0, 1.0, 0.0),
                                                     1.0,
                                                     0.8 );

////static physical_world::Air<Vector2d> air(0.1, Vector2d(0,0));
////static physical_world::StandardWorld<Vector2d, Vector2d> world2d;
////static physical_objects::Ball2D<Vector2d, Vector2d> ball(1.0f, 0.5f, 10.1f, Vector2d(10,10), Vector2d(0,0), Vector2d(0,0), air);
//static physical_objects::ball<Vector2d>
//  ball2d(
//         20.0f, // radius
//         30.0f, // mass
//         0.5f, // elasticity
//         0.8f, // drag_coeef
//         Vector2d(10.0f, 0.0f), //init_v
//         Vector2d(0.0f, 0.0f),  //init_a
//         Vector2d(0.0f, -9.86f), //g MotionVector g,
//         Vector2d(100, 600), //init loc
//         Vector2d(10.0f, 0.0f)); // medium_speed
//static physical_objects::ball<Vector3d>
//  ball3d(
//        0.15f,
//         30.0f,
//         0.3f,
//         0.8f,
//         Vector3d(10.0, 0.0, 0.01),
//         Vector3d(0.0, 0.0, 0.0),
//         Vector3d(0.0f, -9.86f, 0.0f),
//         Vector3d(0.0, 0.0, 0.0),
//         Vector3d(0.0, 0.0, 0.0));
//
//
//static std::vector<Vector2d> obbox;
//
//static std::vector<Vector3d> obbox3d;
static char *ParamFilename = NULL;


#endif
