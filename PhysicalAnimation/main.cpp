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

//#include<glm/glm.hpp>

//  Definitions and namespace

#include "object.h"
#include "definitions.h"
#include "object_drawer.h"
#include<stdlib.h>
#include<time.h>
using namespace std;

#define WIDTH	    1024	/* Window dimensions */
#define HEIGHT		768

Camera *camera;
physical_objects::ModelObject *obj;

bool showGrid = true;
int persp_win;

float get_rand(float low, float high) {
    return ((rand() % 1000) / 1000.0) * (high - low) + low;

}
void push();
////========================================================
//void init_flock(){
//  for( int i = 0 ; i < N; ++i ) {
//    flockmass[i] = 1;
//  }
//  for( int i = 0 ; i < N; ++i ) {
//    X0.s[i] = Vector3d(get_rand(4.0, 6.0),
//                       get_rand(4.0, 6.0),
//                       get_rand(4.0, 6.0) );
//  }
//  for( int i = N; i < 2 * N; ++i ) {
//    X0.s[i] = Vector3d(get_rand(0.0, 1.3),
//                       get_rand(0.0, 1.3),
//                       get_rand(0.0, 0.3));
//  }
//}
//
//Vector3d f(Vector3d X, double t, double T, int i) {
//  Vector3d dxdt;
//  double ctx = obs2ctr.x, cty = obs2ctr.y, ctz = obs2ctr.z;
//  Vector3d ctr(ctx, cty, ctz);
//  Vector3d r(X.x - ctx, X.y - cty, X.z - ctz);//X - ctr;
//  if (r.norm() > 25 * obs2rad ) {
//    dxdt = - r;
//  } else {
//    if (r.norm() > 15 * obs2rad ) {
//      dxdt = - 1.2 * flockmass[i] * curr_X.s[i + N] * curr_X.s[i + N] * r / r.norm();
////      dxdt = - r * ((rand() % 10) / 10.0 );
//    } else {
//      if ( r.norm() < 8 * obs2rad) {
//        dxdt = 2 * r;
//      } else 
//        dxdt = - flockmass[i] * curr_X.s[i + N] * curr_X.s[i + N] * r / r.norm() ;
//    }
//  }
//  for (int i = 0 ; i < N; ++i ) {
//    if ( (X - curr_X.s[i]).norm() < 3 * RECTSIZE ) {
//      Vector3d tmp (0.1, 0.1, 0.1);
//      tmp = tmp + X - curr_X.s[i];
//      tmp = (X - curr_X.s[i]) % tmp;
//      dxdt = dxdt + tmp;
//    }
//  }
//  return dxdt;
//}
//
//StateVector F(StateVector X, double t) {
//  StateVector Xp;
//  for ( int i = 0 ; i < N ; ++i ) {
//    Xp.s[i] = X.s[i + N];
//  }
//  for ( int i = 0;  i < N ; ++i ) {
//    Xp.s[i + N] = 1 / flockmass[i] * f(X.s[i], t, dT, i);
//  }
//  return Xp;
//}
//
//StateVector NumInt ( StateVector X, StateVector Xp, float t, float dt) {
//  StateVector K1, K2, K3, K4;
//  K1 = Xp * dt;
//  
//  K2 = F(X + K1 * (1.0 / 2.0), t + dt / 2.0) * dt;
//  K3 = F(X + K2 * 0.5 , t + 0.5 * dt) * dt;
//  K4 = F(X + K3, t + dt) * dt;
//  return X + (K1 + K2 * 2 + K3 * 2 + K4) * (1.0 / 6.0);
//}
//
//void mainloop(){
//  StateVector Xp = F(curr_X, curr_t);
//  StateVector Xnew = NumInt(curr_X, Xp, curr_t, dt);
//  // collision
//  curr_X = Xnew;
//  curr_t = curr_t + dt;
//}
//
//void draw_flock_point(Vector3d& loc, float rectsize) {
//  glVertex3d(loc.x , loc.y, loc.z);
//  glVertex3d(loc.x, loc.y + rectsize, loc.z);
//  glVertex3d(loc.x + rectsize, loc.y + rectsize, loc.z);
//  glVertex3d(loc.x + rectsize, loc.y, loc.z + rectsize);
//}
//
//void draw_flocking_particles(StateVector X) {
//  glBegin(GL_QUADS);
//  for ( int i = 0 ; i < N ; ++i ) {
//    glColor4f(0.33, 0.76, 1.0, 1.0);
//    draw_flock_point(X.s[i], RECTSIZE * 4 );
//  }
////  draw_flock_point(curr_X.s[0], RECTSIZE);
//  glEnd();
//           
//}
//
////========================================================

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
  float move_step = 0.15;
  switch (key) {
    case 'r': case 'R':
      // reset the camera to its initial position
      camera->Reset();
      break;
    case 'f': case 'F':
      camera->SetCenterOfFocus(Vector3d(0, 0, 0));
      break;
    case 'l': case 'L':
      push();
      break;
    case 'g': case 'G':
      showGrid = !showGrid;
      break;
    case 'a': case 'A':
      obs2ctr.x -= move_step;
      break;
    case 'd': case 'D':
      obs2ctr.x += move_step;
      break;
    case 's': case 'S':
      obs2ctr.y -= move_step;
      break;
    case 'w': case 'W':
      obs2ctr.y += move_step;
      break;
    case 'z': case 'Z':
      obs2ctr.z += move_step;
      break;
    case 'x': case 'X':
      obs2ctr.z -= move_step;
      break;
    case 'q': case 'Q':	// q or esc - quit
    case 27:		// esc
      exit(0);
  }
  
  glutPostRedisplay();
}
using physical_objects::curr_t;
using physical_objects::curr_X;
using physical_objects::dt;
using physical_objects::t_max;

void push(){
  /* initialize random seed: */
  srand ( time(NULL) );
  int push_loc = rand() % (surfaceObj.vertices.size() - 0) + 1 ;
  int force = (rand() % 10 + 1) * 150 + 150;
  
  surfaceObj.vertices[push_loc].external_force =
  Vector3d(0, - force, 0);
  
}
void mainloop(){
//  surfaceObj.print_surface();
  physical_objects::StateVector Xp =
    surfaceObj.calculate_dynamics(surfaceObj.X, curr_t);
  physical_objects::StateVector Xnew = surfaceObj.NumInt(surfaceObj.X, Xp, curr_t, dt);
  
  surfaceObj.update_State(Xnew);
  for(int i = 0; i < surfaceObj.vertices.size(); ++i ) {
    surfaceObj.vertices[i].location =
    //surfaceObj.vertices[i].location  + Vector3d(0,0.5,0);
    surfaceObj.X.s[i];
  }
  curr_t = curr_t + dt;
}

//// simulation function that called in glIdle loop
void Simulate(){
  
//  particle_manager1.move_particles(0.03f, obs2ctr, obs2rad);
  
  mainloop();
  glutPostRedisplay();
//  usleep(130000);
  usleep(30000);
  //  sleep(1);
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
  

//  draw_obstancles(&obs2ctr);
//  drawParticleGenerationPlane();
//  draw_particles(particle_manager1.particles());
  //draw scene
//  draw_flocking_particles(curr_X);
  
//  glTranslatef(0, 3.5, 0);
//  Draw3DWorld();
//  glutWireTeapot(5);
//  draw_surface(surfaceObj);
  
  // activate and specify pointer to vertex array
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &obj->vertices);
  
  // draw first half, range is 6 - 0 + 1 = 7 vertices used
  const GLvoid* indice_array = &obj->indices;
  glDrawRangeElements(GL_QUADS, 0, obj->indices.size()-1, obj->indices.size(),
                      GL_UNSIGNED_BYTE, indice_array);

  glutSwapBuffers();
  
}


// set up something in the world
void init_the_world() {
//  init_flock();
//  curr_X = X0;
  
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
}

static char* parafile;
void Reset(){
  LoadParameters(parafile);
}


/*
 Main program to draw the square, change colors, and wait for quit
 */
int main(int argc, char* argv[]){
//  if(argc != 2){
//    fprintf(stderr, "usage: bounce paramfile\n");
//    exit(1);
//  }
//  LoadParameters(argv[1]);
//  parafile = argv[1];

  init_the_world();
  printf("R reset camera\nG toggle grid\nPress L to rain\nQ quit");
  
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
