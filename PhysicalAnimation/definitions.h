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

//static double WinWidth = WINDOW_WIDTH;
//static double WinHeight = WINDOW_HEIGHT;
static int Step = true;
static int Start = true;
static int Stopped = true;
static int MiddleButton = false;
static int Throw = false;
//static int WeightMatters = 0;
static int HaveWind = false;
static int Floor = false;



//static int frameCount, currentTime, previousTime;
//static float fps;

static char* window_title = "Particle System -- Yanxiang Wu";

static float fall_src_orix = -2.0, fall_src_oriy = 5.0, fall_src_oriz = -2.0;
particle_manager::ParticleManager particle_manager1 (
    50000, // particle #
    Vector3d(
    fall_src_orix, 
    fall_src_oriy,
    fall_src_oriz), // generation plane original
    2 * abs(fall_src_orix), 2.0, //width and height
    Vector3d( 0.0, 1.0, 0.0), // output direction
    1.0,      // position mean
    0.2 );    // position var

static float obs1x = fall_src_orix + 0.3,
  obs1y = fall_src_oriy - 3.0,
  obs1z = fall_src_oriz + 0.3;
static float obs1width = 1.0, obs1height = 0.5;

Vector3d obs1p0(obs1x, obs1y, obs1z),
         obs1p1(obs1x + obs1width, obs1y, obs1z),
         obs1p2(obs1x + obs1width, obs1y, obs1z + obs1height),
         obs1p3(obs1x, obs1y, obs1z + obs1height); 
Vector3d obstancle1[4] = {obs1p0, obs1p1, obs1p2, obs1p3};

Vector3d obs2ctr(obs1x + 1.3, obs1y + 0.6, obs1z + 1.8);
float obs2rad = 0.5;

////static physical_world::Air<Vector2d> air(0.1, Vector2d(0,0));
////static physical_world::StandardWorld<Vector2d, Vector2d> world2d;
////static physical_objects::Ball2D<Vector2d, Vector2d> ball(1.0f, 0.5f, 10.1f,
//          Vector2d(10,10), Vector2d(0,0), Vector2d(0,0), air);
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

//===================flocking==================================
//#define SUBDIVITION 0
//#define N ((SUBDIVITION + 2) * (SUBDIVITION + 2))  //# of interacted particles 
//typedef struct StateVector {
//  Vector3d s[2 * N];
//  
//  StateVector operator*(double s) const{
//    StateVector res;
//    for (int i = 0 ; i < 2 * N; ++i ) {
//      res.s[i] = this->s[i] * s;
//    }
//    return res;
//  }
//  const StateVector& operator=(const StateVector& v2) {
//    for ( int i = 0 ; i < 2 * N; ++i ) {
//      this->s[i] = v2.s[i];
//    }
//    return *this;
//  }
//  StateVector operator+(const StateVector& v2) {
//    StateVector res;
//    for (int i = 0 ; i < 2 * N; ++i ) {
//      res.s[i] = (this->s[i] + v2.s[i]);
//    }
//    return res;
//  }
//}StateVector;
//
//static float flockmass[N];
//static float dt = 0.1, dT = 0.5;
//static StateVector X0;
//static StateVector curr_X;
//static float curr_t = 0.0f, t_max = 10000.0f;

// flocking===========================================
// define
physical_objects::surface surfaceObj(
                   64, 64, Vector3d(0.0, 0.0, 0.0), SUBDIVITION,
                   0.9, //spring
                   0.2, //damper
                   0.3
                   );

#endif
