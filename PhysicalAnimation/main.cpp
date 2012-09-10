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

//  Definitions and namespace
#include"definitions.h"
using namespace std;

#define WIDTH	    1024	/* window dimensions */
#define HEIGHT		768

void DrawABall(int collision,
               void* obj);
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
//  world2d.add_object_location( ball.identifier() , Vector2d(10, 500));
//
  Vector2d a(30, 30), b(700, 30), c(700, 700), d(30, 700);
  obbox.push_back(a);
  obbox.push_back(b);
  obbox.push_back(b);
  obbox.push_back(c);
  obbox.push_back(c);
  obbox.push_back(d);
  physical_objects::box<Vector2d> box2d(obbox);
  
  Vector3d a3(-1.0f, -1.0f, -1.0f),
    b3(-1.0f, 1.0f, -1.0f),
    c3(1.0f, 1.0f, -1.0f),
    d3(1.0f, -1.0f, -1.0f),
    e3(-1.0f, -1.0f, 1.0f),
    f3(-1.0f, 1.0f, 1.0f),
    g3(1.0f, 1.0f, 1.0f),
    h3 (1.0f, -1.0f, 1.0f);  
  
  obbox3d.push_back(a3);
  obbox3d.push_back(b3);
  obbox3d.push_back(c3);
  obbox3d.push_back(d3);
  // second plane
  obbox3d.push_back(a3);
  obbox3d.push_back(e3);
  obbox3d.push_back(f3);
  obbox3d.push_back(b3);
  // third plane
  obbox3d.push_back(f3);
  obbox3d.push_back(b3);
  obbox3d.push_back(c3);
  obbox3d.push_back(g3);
  // forth plane
  obbox3d.push_back(h3);
  obbox3d.push_back(g3);
  obbox3d.push_back(c3);
  obbox3d.push_back(d3);
  // fifth plane
  obbox3d.push_back(h3);
  obbox3d.push_back(e3);
  obbox3d.push_back(a3);
  obbox3d.push_back(d3);
  // facing plane
  obbox3d.push_back(e3);
  obbox3d.push_back(f3);
  obbox3d.push_back(g3);
  obbox3d.push_back(h3);
  
  physical_objects::box<Vector3d> box3d(obbox3d);
  

}


void DrawWorld(){
  
  // when rendering a new frame, we told the video card that we need
  //  clear the bolor and deph information
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  // loading the identity matrix means reset the screen corrdinate system to
  // XYZ axis of length 1, it starts at z=0, x from [-1, 1] and y from [-1, 1]
  glLoadIdentity();
  
  DrawABall(1, &ball2d);
  glTranslatef(0.0f, 0.0f, -5.0f);
  glColor3f(RGBBLUE);
  
  glutSwapBuffers();
}

void DrawABall(int collision, void* ball) {
  physical_objects::ball<Vector2d>* obj =
    (physical_objects::ball<Vector2d>*)ball;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  Vector2d& obj_loc = obj->location();
  glColor3f(RGBYELLOW);
  Circlef( obj_loc.x, obj_loc.y, obj->radius());
//  OldBall[outline_cnt++] = obj_loc;
//  for (int i = 0 ; i < outline_cnt; ++i) {
//    OutlineCirclef(OldBall[i].x, OldBall[i].y, obj->radius());
//  }
  //-----------------------draw end---------------------
//  glutSwapBuffers();
  
}

void DrawPlanes() {
  float alpha = 1.0f;
  for ( int i = 0; i < obbox3d.size(); i += 4 ) {
    if (i == 20 ) {
      alpha = 0.5f;
    }
    glBegin( GL_QUADS );
    
      glColor4f(0.0f + 0.12*i/4, 1.0f - 0.12*i/4, 0.5 - 0.03*i/4, alpha);
      glVertex3f(obbox3d[i].x, obbox3d[i].y, obbox3d[i].z);
      glVertex3f(obbox3d[i+1].x, obbox3d[i+1].y, obbox3d[i+1].z);
      glVertex3f(obbox3d[i+2].x, obbox3d[i+2].y, obbox3d[i+2].z);
      glVertex3f(obbox3d[i+3].x, obbox3d[i+3].y, obbox3d[i+3].z);
    glEnd();
  }
  
}

void DrawSphere(int collison, void* ball) {
  glColor3f(RGBYELLOW);
  glutSolidSphere(ball3d.radius(), 50, 50);
//  GLUquadricObj* qdj = NULL;
//  qdj = gluNewQuadric();
//  gluQuadricDrawStyle(qdj, GLU_FILL);
//  gluQuadricNormals(qdj, GLU_SMOOTH);
//  gluQuadricOrientation(qdj, GLU_OUTSIDE);
//  
//  gluSphere(gluNewQuadric(), ball3d.radius(), 50, 50 );
//  gluDeleteQuadric(qdj);
}
void Draw3DWorld(){
  // when rendering a new frame, we told the video card that we need
  //  clear the bolor and deph information
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable( GL_BLEND );
  
  // loading the identity matrix means reset the screen corrdinate system to
  // XYZ axis of length 1, it starts at z=0, x from [-1, 1] and y from [-1, 1]
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -3.0f);
  glColor3f(RGBBLUE);
  DrawPlanes();
  DrawSphere(0, NULL);
//  glutWireCube(2.0f);
  
  glutSwapBuffers();
}

// simulation function that called in glIdle loop
void Simulate(){
//  ball2d.move(DrawABall, 0.1f, obbox);
  ball3d.move(DrawSphere, 0.1f, obbox3d);
  glutPostRedisplay();
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
      DrawABall(0, &ball2d);
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
  Draw3DWorld();
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
  double Mass, v0x, v0y, drag, elastic, time_step, vwx, vwy, disp_time;
  if(fscanf(paramfile, "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
            &Mass, &v0x, &v0y, &drag, &elastic,
            &time_step, &disp_time, &vwx, &vwy) != 9){
    fprintf(stderr, "error reading parameter file %s\n", filename);
    fclose(paramfile);
    exit(1);
  }
  ball2d = *(new physical_objects::ball<Vector2d>(
    20.0f, // radius
    Mass, // mass
    elastic, // elasticity
    drag, // drag_coeef
    Vector2d(v0x, v0y), //init_v
    Vector2d(0.0f, 0.0f),  //init_a
    Vector2d(0.0f, -9.86f), //g MotionVector g,
    Vector2d(100, 600), //init loc
    Vector2d(vwx, vwy))); // medium_speed

}

static char* parafile;
void Reset(){
  LoadParameters(parafile);
}

void HandleMenu(int index){
  switch(index){
    case MenuReset:
      Reset();
      break;
    case MenuQuit:
      exit(0);
      
  }
}
/*
 Set up pop-up menu on right mouse button
 */
void MakeMenu(){
  
  int id = glutCreateMenu(HandleMenu);
  

  glutAddMenuEntry("Reset", MenuReset);
  glutAddMenuEntry("Quit", MenuQuit);
  
  glutSetMenu(id);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

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
  
//  glEnable(GL_DEPTH_TEST);
  
  // make GLUT select a double buffered display that uses RGBA colors
  // Julian: Add GLUT_DEPTH when in 3D program so that 3D objects drawed
  // correctly regardless the order they draw
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Ball on the air");
  
  // set up the callback routines to be called when glutMainLoop() detects
  // an event
  glutReshapeFunc(doReshape);
  glutDisplayFunc(RenderScene);
  glutKeyboardFunc(handleKey);	  // keyboard callback
  glutIdleFunc(Simulate);
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
//  glShadeModel(GL_FLAT);

  MakeMenu();
  
  // Routine that loops forever looking for events. It calls the registered
  // callback routine to handle each event that is detected
  glutMainLoop();
  return 0;
}
