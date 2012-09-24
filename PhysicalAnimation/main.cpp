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
#include "Camera.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

//  project header here

//  Definitions and namespace
#include"definitions.h"
#include "object_drawer.h"
using namespace std;

#define WIDTH	    1024	/* window dimensions */
#define HEIGHT		768

Camera *camera;
bool showGrid = true;
int persp_win;


//void DrawABall(int collision,
//               void* obj);
//----------------------Including and definitions end-----------------------

//-------------------------------------------------------------------------
// Calculates the frames per second
//-------------------------------------------------------------------------
void calculateFPS()
{
  //  Increase frame count
  frameCount++;
  
  //  Get the number of milliseconds since glutInit called
  //  (or first call to glutGet(GLUT ELAPSED TIME)).
  currentTime = glutGet(GLUT_ELAPSED_TIME);
  
  //  Calculate time passed
  int timeInterval = currentTime - previousTime;
  
  if(timeInterval > 1000)
  {
    //  calculate the number of frames per second
    fps = frameCount / (timeInterval / 1000.0f);
    
    //  Set time
    previousTime = currentTime;
    
    //  Reset frame count
    frameCount = 0;
  }
  
  printf("FPS: %f\n", fps);
}

// draws a simple grid
void makeGrid() {
  glColor3f(0.0, 0.0, 0.0);
  
  glLineWidth(1.0);
  
  for (float i=-12; i<12; i++) {
    for (float j=-12; j<12; j++) {
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i, 0, j+1);
      glEnd();
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i+1, 0, j);
      glEnd();
      
      if (j == 11){
        glBegin(GL_LINES);
        glVertex3f(i, 0, j+1);
        glVertex3f(i+1, 0, j+1);
        glEnd();
      }
      if (i == 11){
        glBegin(GL_LINES);
        glVertex3f(i+1, 0, j);
        glVertex3f(i+1, 0, j+1);
        glEnd();
      }
    }
  }
  
  glLineWidth(2.0);
  glBegin(GL_LINES);
  glVertex3f(-12, 0, 0);
  glVertex3f(12, 0, 0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0, 0, -12);
  glVertex3f(0, 0, 12);
  glEnd();
  glLineWidth(1.0);
}

void init() {
  // set up camera
  // parameters are eye point, aim point, up vector
  camera = new Camera(Vector3d(0, 32, 27), Vector3d(0, 0, 0),
                      Vector3d(0, 1, 0));
  
  // grey background for window
  glClearColor(0.62, 0.62, 0.62, 0.0);
  glShadeModel(GL_SMOOTH);
  glDepthRange(0.0, 1.0);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
}

void mouseEventHandler(int button, int state, int x, int y) {
  // let the camera handle some specific mouse events (similar to maya)
  camera->HandleMouseEvent(button, state, x, y);
}

void motionEventHandler(int x, int y) {
  // let the camera handle some mouse motions if the camera is to be moved
  camera->HandleMouseMotion(x, y);
  glutPostRedisplay();
}

void keyboardEventHandler(unsigned char key, int x, int y) {
  switch (key) {
    case 'r': case 'R':
      // reset the camera to its initial position
      camera->Reset();
      break;
    case 'f': case 'F':
      camera->SetCenterOfFocus(Vector3d(0, 0, 0));
      break;
    case 'g': case 'G':
      showGrid = !showGrid;
      break;
      
    case 'q': case 'Q':	// q or esc - quit
    case 27:		// esc
      exit(0);
  }
  
  glutPostRedisplay();
}



//// simulation function that called in glIdle loop
void Simulate(){
//  ball2d.move(DrawABall, 0.1f, obbox);
//  ball3d.move(DrawSphere, 0.005f, obbox3d);
  particle_manager1.move_particles(0.03f);
  
  calculateFPS();
  glutPostRedisplay();
  usleep(13000);
  //  sleep(1);
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
  glViewport(0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT); // Set our viewport to the size of our window
  glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
  glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
  gluPerspective(60, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
  glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly
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
//      DrawABall(0, &ball2d);
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

void drawParticleGenerationPlane(){
  glColor4f(1.0, 1.0, 1.0, 0.2);
  Vector3d* genP = particle_manager1.generation_plane();
  glBegin(GL_QUADS);
  glVertex3f(genP[0].x, genP[0].y, genP[0].z );
//  glColor4f(0.0, 1.0, 0.0, 0.5);
  glVertex3f(genP[1].x, genP[1].y, genP[1].z );
//  glColor4f(0.0, 0.0, 1.0, 0.5);
  glVertex3f(genP[2].x, genP[2].y, genP[2].z );
  glVertex3f(genP[3].x, genP[3].y, genP[3].z );
  glEnd() ;
}


/*
 On Redraw request, erase the window and redraw everything
 */
void RenderScene(){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  camera->PerspectiveDisplay(WIDTH, HEIGHT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (showGrid)
    makeGrid();
  drawParticleGenerationPlane();
  draw_particles(particle_manager1.particles());
  //draw scene
//  glTranslatef(0, 3.5, 0);
//  Draw3DWorld();
//  glutWireTeapot(5);
  
  glutSwapBuffers();
  
}






// set up something in the world
void init_the_world() {
  
}

/*
 Load parameter file and reinitialize global parameters
 */
void LoadParameters(char *filename){
  
  FILE *paramfile;
  
  if((paramfile = fopen(filename, "r")) == NULL){
    fprintf(stderr, "error opening parameter file %s\n", filename);
    exit(1);
  }
  
  ParamFilename = filename;
  double Mass, v0x, v0y, v0z, drag, elastic, time_step, vwx, vwy, vwz, disp_time;
  if(fscanf(paramfile, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
            &Mass, &v0x, &v0y, &v0z, &drag, &elastic,
            &time_step, &disp_time, &vwx, &vwy, &vwz) != 11){
    fprintf(stderr, "error reading parameter file %s\n", filename);
    fclose(paramfile);
    exit(1);
  }
//  ball2d = *(new physical_objects::ball<Vector2d>(
//    20.0f, // radius
//    Mass, // mass
//    elastic, // elasticity
//    drag, // drag_coeef
//    Vector2d(v0x, v0y), //init_v
//    Vector2d(0.0f, 0.0f),  //init_a
//    Vector2d(0.0f, -9.86f), //g MotionVector g,
//    Vector2d(100, 600), //init loc
//    Vector2d(vwx, vwy))); // medium_speed
//  ball3d = *(new physical_objects::ball<Vector3d>(
//     0.15f,
//     Mass,
//     elastic,
//     drag,
//     Vector3d(v0x, v0y, v0z),
//     Vector3d(0.0, 0.0, 0.0),
//     Vector3d(0.0f, -9.86f, 0.0f),
//     Vector3d(0.0, 0.0, 0.0),
//     Vector3d(vwx, vwy, vwz)));

}

static char* parafile;
void Reset(){
  LoadParameters(parafile);
}

//void HandleMenu(int index){
//  switch(index){
//    case MenuReset:
//      Reset();
//      break;
//    case MenuQuit:
//      exit(0);
//      
//  }
//}
///*
// Set up pop-up menu on right mouse button
// */
//void MakeMenu(){
//  
//  int id = glutCreateMenu(HandleMenu);
//  
//
//  glutAddMenuEntry("Reset", MenuReset);
//  glutAddMenuEntry("Quit", MenuQuit);
//  
//  glutSetMenu(id);
//  glutAttachMenu(GLUT_RIGHT_BUTTON);
//}

/*
 Main program to draw the square, change colors, and wait for quit
 */
int main(int argc, char* argv[]){
  if(argc != 2){
    fprintf(stderr, "usage: bounce paramfile\n");
    exit(1);
  }
  LoadParameters(argv[1]);
  parafile = argv[1];
  init_the_world();
  
  // start up the glut utilities
  glutInit(&argc, argv);
  
  
  // make GLUT select a double buffered display that uses RGBA colors
  // Julian: Add GLUT_DEPTH when in 3D program so that 3D objects drawed
  // correctly regardless the order they draw
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//  glutInitWindowPosition(50, 50);
  persp_win = glutCreateWindow(window_title);
  
  // initialize the camera and such
  init();
  
  
  // set up the callback routines to be called when glutMainLoop() detects
  // an event
//  glutReshapeFunc(doReshape);
  glutDisplayFunc(RenderScene);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(keyboardEventHandler);
  glutIdleFunc(Simulate);
  
  
  /* Set shading to flat shading */
//  glShadeModel(GL_FLAT);

//  MakeMenu();
  
  // Routine that loops forever looking for events. It calls the registered
  // callback routine to handle each event that is detected
  glutMainLoop();
  return 0;
}
