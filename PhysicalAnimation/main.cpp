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

#include "ObjLoader.h"
#include<stdlib.h>
using namespace std;

#define WIDTH	    1024	/* Window dimensions */
#define HEIGHT		768

Camera *camera;
//===================game objects definition=====================
ModelObject *rigid_objects;
MotionController *controller;
//===============================================================

bool showGrid = true;

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
  srand((unsigned int)time(NULL));
  // set up camera
  // parameters are eye point, aim point, up vector
  //  camera = new Camera(Vector3d(0, 32, 27), Vector3d(0, 0, 0),
  //                      Vector3d(0, 1, 0));
  camera = new Camera(Vector3d(0, 5, 4), Vector3d(0, 0, 0),
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
//  float move_step = 0.15;
  switch (key) {
    case 'r': case 'R':
      // reset the camera to its initial position
      camera->Reset();
      break;
    case 'f': case 'F':
      camera->SetCenterOfFocus(Vector3d(0, 0, 0));
      break;
    // case 'l': case 'L':
    //   push();
    //   break;
    case 'g': case 'G':
      showGrid = !showGrid;
      break;
    // case 'a': case 'A':
    //   obs2ctr.x -= move_step;
    //   break;
    // case 'd': case 'D':
    //   obs2ctr.x += move_step;
    //   break;
    // case 's': case 'S':
    //   obs2ctr.y -= move_step;
    //   break;
    // case 'w': case 'W':
    //   obs2ctr.y += move_step;
    //   break;
    // case 'z': case 'Z':
    //   obs2ctr.z += move_step;
    //   break;
    // case 'x': case 'X':
    //   obs2ctr.z -= move_step;
    //   break;
    case 'q': case 'Q':	// q or esc - quit
    case 27:		// esc
      exit(0);
  }
  
  glutPostRedisplay();
}

void rigid_object_simulation(){
  //testing with random behavior
  //  rigid_objects->rotate(get_rand(0, 10), get_rand(-2.0, 2.0),
  //                        get_rand(-2.0, 2.0),
  //                        get_rand(-2.0, 2.0));
  controller->next_step();
}

//// simulation function that called in glIdle loop
void Simulate(){
  rigid_object_simulation();
  glutPostRedisplay();
  usleep(30000);
}


/*
 On Redraw request, erase the window and redraw everything
 */
void RenderScene(){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  camera->PerspectiveDisplay(WIDTH, HEIGHT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (showGrid)
    makeGrid();

  glColor3f(0.0, 1.0, 1.0);
  
  ModelObject *obj;
  for(int i = 0 ; i < ObjLoader::objects.size(); ++i ) {
    obj = ObjLoader::ObjLoader::objects[i];
    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, obj->vertices_array);
    glDrawElements(GL_POINT_BIT,
                   (int)obj->indices.size(),
                   GL_UNSIGNED_BYTE,
                   obj->indice_array);

  }
  glutSwapBuffers();
  
}

void init_rigid_object_world(char argc, char **argv){
  int obj_no = 0 ;
  ObjLoader::loadObject(argv[1], obj_no);
  if (obj_no < 1) {
    printf("Loading object from %s error, exit", argv[1]);
    exit(1);
  }
  ObjLoader::objects[0]->rotate(90, 0, 0, 1);
  rigid_objects = ObjLoader::objects[0];
  
  controller = new MotionController(rigid_objects, 0.0, 0.05);
}

// set up something in the world
void init_the_world(char argc, char **argv) {
  init_rigid_object_world(argc, argv);
  
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
  
//  ParamFilename = filename;
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
  
  init_the_world(argc, argv);
  printf("R reset camera\nG toggle grid\nPress L to rain\nQ quit");
  
  // start up the glut utilities
  glutInit(&argc, argv);
  
  
  // make GLUT select a double buffered display that uses RGBA colors
  // Julian: Add GLUT_DEPTH when in 3D program so that 3D objects drawed
  // correctly regardless the order they draw
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
//  glutInitWindowPosition(50, 50);
  glutCreateWindow("Strange Rigid Body");
  
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
