//
//  definitions.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/3/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef PhysicalAnimation_definitions_h
#define PhysicalAnimation_definitions_h

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
static int NSteps = 0;
static int NTimeSteps = -1;
static Vector2d OldBall[MAXSTEPS];

static double WinWidth = WINDOW_WIDTH;
static double WinHeight = WINDOW_HEIGHT;
static int Step = true;
static int Start = true;
static int Stopped = true;
static int MiddleButton = false;
static int Throw = false;
static int WeightMatters = 0;
static int HaveWind = false;
static int Floor = false;


static physical_objects::Ball2D ball(1.0f, 0.5f, 10.1f);
#endif
