//
//  main.cpp
//  PhysicalAnimation
//
//  Created by Julian Wu on 8/23/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

//  Style comments
//  Cooresponding header file here

//  C stdlib here

//  C++ stdlib here
#include <iostream>

//  3rt party header here
#include "Vector.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

//  project header here
#include "MovementStrategy.h"
//  Definitions and namespace
#include"definitions.h"
using namespace std;

#define WIDTH	    1024	/* window dimensions */
#define HEIGHT		768

//----------------------Including and definitions end-----------------------
/*
 Draw an outlined circle with center at position (x, y) and radius rad
 */
void OutlineCirclef(float x, float y, float rad){
  
  float theta;
  
  glLineWidth(1);
  
  glBegin(GL_LINE_LOOP);
  for(theta=0.0; theta < 2 * PI; theta += CIRC_INC)
    glVertex2f(x+rad*cos(theta), y+rad*sin(theta));
  glEnd();
}

/*
 Draw a filled circle with center at position (x, y) and radius rad
 */
void Circlef(float x, float y, float rad){
  
  float theta;
  
  glBegin(GL_POLYGON);
  for(theta=0.0; theta < 2 * PI; theta += CIRC_INC)
    glVertex2f(x+rad*cos(theta), y+rad*sin(theta));
  glEnd();
}


// initilize the world with setting the position of each object
void init_the_world(){
  world2d.add_object_location( ball.identifier() , Vector2d(10, 500));
  
}



void DrawABall(int collision, physical_objects::SphericalObject& obj) {
  
  
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  Vector2d& obj_loc = world2d.get_object_location(obj.identifier() );
  glColor3f(RGBYELLOW);
  Circlef( obj_loc.x, obj_loc.y, obj.radius());
  
  
  //-----------------------draw end---------------------
  glutSwapBuffers();
}

void Simulate(){
  
}


void DrawBall(int collision, physical_objects::SphericalObject& obj) {
  
  
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
//  if(Trace){
//    for(i = 0; i < NSteps && i < MAXSTEPS; i++){
////      if(Collision[i])
////        glColor3f(RGBRED);
////      else
//        glColor3f(RGBYELLOW);
//      
//      OutlineCirclef(OldBall[i].x, OldBall[i].y, obj.radius());
//    }
//  }
  
//  if(Floor)
//    DrawFloor();
//  else
//    EraseFloor();
  
  if(collision)
    glColor3f(RGBRED);
  else
    glColor3f(RGBYELLOW);
  
  Circlef(Ball.x, Ball.y, obj.radius());
  glutSwapBuffers();
  
  if(NSteps < MAXSTEPS){
    OldBall[NSteps] = Ball;
//    Collision[NSteps] = collision;
    NSteps++;
  }
  else{
    cerr << "Ball position table overflow, restarting!!" << endl;
//    Time = 0;
    NSteps = 0;
  }

}

/*
 Keyboard Callback Routine: 'c' cycle through colors, 'q' or ESC quit
 This routine is called every time a key is pressed on the keyboard
 */
void handleKey(unsigned char key, int x, int y){
  
  switch(key){
//    case 'c':		// 'c' - cycle to next color
//    case 'C':
//      icolor = (icolor + 1) % 6;
//      glutPostRedisplay();
//      break;
      
    case 'q':		// q - quit
    case 'Q':
    case 27:		// esc - quit
      exit(0);
      
    default:		// not a valid key -- just ignore it
      return;
  }
}

/*
 On Reshape request, reshape viewing coordinates to keep the viewport set
 to the original window proportions and to keep the window coordinates fixed
 */
void doReshape(int w, int h){
  int vpw, vph;
  
  float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
  if(float(w) / float(h) > aspect){
    vph = h;
    vpw = int(aspect * h + 0.5);
  }
  else{
    vpw = w;
    vph = int(w / aspect + 0.5);
  }
  
  glViewport(0, 0, vpw, vph);
  WinWidth = w;
  WinHeight = h;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
  
  glMatrixMode(GL_MODELVIEW);
}

/*
 Adjust mouse coordinates to match window coordinates
 */
void AdjustMouse(int& x, int& y){
  
  /* reverse y, so zero at bottom, and max at top */
  y = int(WinHeight - y);
  
  /* rescale x, y to match current window size (may have been rescaled) */
  y = int(y * WINDOW_HEIGHT / WinHeight);
  x = int(x * WINDOW_WIDTH / WinWidth);
}

/*
 Watch mouse button presses and handle them
 */
void handleButton(int button, int state, int x, int y){
  
  if(button == GLUT_MIDDLE_BUTTON)
    MiddleButton = (state == GLUT_DOWN);
  
  if(button != GLUT_LEFT_BUTTON)
    return;
  
  AdjustMouse(x, y);	/* adjust mouse coords to current window size */
  
  if(state == GLUT_UP){
    if(Start){
      Start = false;
      Stopped = false;
      Ball.set(STARTX, STARTY);
      DrawBall(0, ball);
      //glutIdleFunc(Simulate);
    }
    else if(Stopped){
      Stopped = false;
      //glutIdleFunc(Simulate);
    }
    else{
      Stopped = true;
      glutIdleFunc(NULL);
    }
  }
}


/*
 On Redraw request, erase the window and redraw everything
 */
void RenderScene(){
  
  glLoadIdentity();
  DrawABall(0, ball);

}

/*
 Main program to draw the square, change colors, and wait for quit
 */
int main(int argc, char* argv[]){
  init_the_world();
  
  // start up the glut utilities
  glutInit(&argc, argv);
  
  // make GLUT select a double buffered display that uses RGBA colors
  // Julian: Add GLUT_DEPTH when in 3D program so that 3D objects drawed
  // correctly regardless the order they draw
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Ball on the air");
  
  // set up the callback routines to be called when glutMainLoop() detects
  // an event
  glutReshapeFunc(doReshape);
  glutDisplayFunc(RenderScene);
  glutKeyboardFunc(handleKey);	  // keyboard callback
//
  glutMouseFunc(handleButton);
  //glutReshapeFunc(..) // when window size changed
  
//  // define the drawing coordinate system on the viewport
//  // lower left is (0, 0), upper right is (WIDTH, HEIGHT), measured in pixels
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  gluOrtho2D(0, WIDTH, 0, HEIGHT);
  
  /* Set up to clear screen to black */
  glClearColor(RGBBLACK, 0);
  
  /* Set shading to flat shading */
  glShadeModel(GL_FLAT);

  
  
  // Routine that loops forever looking for events. It calls the registered
  // callback routine to handle each event that is detected
  glutMainLoop();
  return 0;
}
