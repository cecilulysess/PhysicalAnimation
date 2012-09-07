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
using namespace std;

#define WIDTH	    1024	/* window dimensions */
#define HEIGHT		768

//---------------------Including and definitions------------------------ 





static int icolor = 0;

///*
// Display Callback Routine: clear the screen and draw a square
// This routine is called every time the window on the screen needs
// to be redrawn, like if the window is iconized and then reopened
// by the user, and when the window is first created. It is also
// called whenever the program calls glutPostRedisplay()
// */
//void drawSquare(){
//  // red, yellow, green, cyan, blue, magenta
//  float colors[6][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0},
//    {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};
//  
//  glClear(GL_COLOR_BUFFER_BIT);  // clear window to background color
//  
//  // set the drawing color to the currently selected color
//  glColor3f(colors[icolor][0], colors[icolor][1], colors[icolor][2]);
//  
//  // draw the square
//  glBegin(GL_POLYGON);
//  glVertex2i(100, 100);
//  glVertex2i(100, 500);
//  glVertex2i(500, 500);
//  glVertex2i(500, 100);
//  glEnd();
//  
//  // flush the OpenGL pipeline to the viewport
//  glutSwapBuffers();
//}

/*
 Keyboard Callback Routine: 'c' cycle through colors, 'q' or ESC quit
 This routine is called every time a key is pressed on the keyboard
 */
void handleKey(unsigned char key, int x, int y){
  
  switch(key){
    case 'c':		// 'c' - cycle to next color
    case 'C':
      icolor = (icolor + 1) % 6;
      glutPostRedisplay();
      break;
      
    case 'q':		// q - quit
    case 'Q':
    case 27:		// esc - quit
      exit(0);
      
    default:		// not a valid key -- just ignore it
      return;
  }
}

/*
 Main program to draw the square, change colors, and wait for quit
 */
int main(int argc, char* argv[]){
  
  // start up the glut utilities
  glutInit(&argc, argv);
  
  // make GLUT select a double buffered display that uses RGBA colors
  // Julian: Add GLUT_DEPTH when in 3D program so that 3D objects drawed
  // correctly regardless the order they draw
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  
  // create the graphics window, giving width, height, and title text
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("A Simple Square");
  
  // set up the callback routines to be called when glutMainLoop() detects
  // an event
  glutDisplayFunc(drawSquare);	  // display callback
  glutKeyboardFunc(handleKey);	  // keyboard callback
  //glutReshapeFunc(..) // when window size changed
  
  // define the drawing coordinate system on the viewport
  // lower left is (0, 0), upper right is (WIDTH, HEIGHT), measured in pixels
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WIDTH, 0, HEIGHT);
  
  // specify window clear (background) color to be opaque white
  glClearColor(1, 1, 1, 1);
  
  // Routine that loops forever looking for events. It calls the registered
  // callback routine to handle each event that is detected
  glutMainLoop();
  return 0;
}
