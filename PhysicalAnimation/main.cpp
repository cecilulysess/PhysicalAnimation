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
#include <assert.h>
#include "ObjLoader.h"
#include "physical_object.h"
//#include <Magick++.h>
//#include "FrameGrabber.h"
#include<stdlib.h>
using namespace std;

#define HDWIDTH       1920    // HD image dimensions
#define HDHEIGHT      1080
#define WIDTH       (HDWIDTH/2) // window dimensions = 1/2 HD
#define HEIGHT        (HDHEIGHT/2)
#define STARTFRAME    0     // first frame number for numbering image files
#define OBJRADIUS     0.5

#define AMBIENT_FRACTION 0.1
#define DIFFUSE_FRACTION 0.4
#define SPECULAR_FRACTION 0.4
const float WHITE[] = {1, 1, 1, 1};


static string MYPATH = "/home/yanxiaw/Programming/cs619/final/animation";
static string MYFILENAME = "trampline";
//===================Global parameter============================
double mesh_spring, mesh_damping, mesh_v_mass, obj_mass, obj_spring,
obj_damping, mesh_ulx, mesh_uly;
int mesh_subdiv;
//===============================================================


Camera *camera;

//FrameGrabber framegrabber(MYPATH, MYFILENAME, HDWIDTH, HDHEIGHT, STARTFRAME);
//===================game objects definition=====================
ModelObject *rigid_objects;
MotionController *controller;
physical_objects::BouncingMesh* bouncing_mesh;
std::vector<physical_objects::DropingObject*> dropping_objs;
float current_time = 0.0, deltaT = 0.05;
long framecnt = 0;
long startframe = 0;
bool drawFaceNormal = false;
bool Recording = false;

//using namespace Magick;




void draw_bouncing_mesh(physical_objects::BouncingMesh& mesh){
  
  // for ( int i = 0 ; i < mesh.mesh_particles().size() ; ++i ) {
  //   glColor4f(1 - (1.0 / mesh.mesh_particles().size()) * i,
  //             1 - (1.0 / mesh.mesh_particles().size()) * i,
  //             1 - (1.0 / mesh.mesh_particles().size()) * i, 1);
  //   const physical_objects::Particle& p = mesh.mesh_particles().at(i);
  //   glLoadIdentity();
  //   glTranslatef(p.x.x, p.x.y, p.x.z);
  //   if (p.is_pivot)
  //     glColor4f(0, 0, 0, 1 - (1.0 / mesh.mesh_particles().size()) * i);
  //   glutSolidSphere(0.1, 5, 5);
    
  //   glVertex3f(p.x.x, p.x.y, p.x.z);
  //   if (p.is_pivot)
  //     glColor4f(1 - (1.0 / mesh.mesh_particles().size()) * i,
  //               1 - (1.0 / mesh.mesh_particles().size()) * i,
  //               1 - (1.0 / mesh.mesh_particles().size()) * i, 1);
    
  // }
  // draw temporary vertice as a ball
  // for ( int i = 0; i < mesh.faces().size(); ++i) {
  //   for (int j = 0; j < mesh.faces()[i].temporary_vertices.size(); ++j){
  //     if ( mesh.faces()[i].temporary_vertices.size() > 1 ) {
  //       // printf("More than 1 tmp: %d\t", mesh.faces()[i].temporary_vertices.size());
  //       // mesh.faces()[i].temporary_vertices[0].p->x.print();
  //       // printf("\n");
  //     }
  //     glColor4f(0, 1, 1, 1);
  //     // const physical_objects::Face* face = &mesh.faces()[i];
  //     const physical_objects::ParticleStrutPair* psp =
  //       &mesh.faces()[i].temporary_vertices[j];
  //     if (psp->p == NULL) {
  //       printf("\tDetected null psp\n");
  //       continue;
  //     }
  //     glLoadIdentity();
  //     double x = psp->p->x.x; 
  //     double y = psp->p->x.y + OBJRADIUS; 
  //     double z = psp->p->x.z;
  //     glTranslatef(x, y, z);
  //     glutSolidSphere(OBJRADIUS, 15, 15);
  //     // glVertex3f(psp->p->x.x, psp->p->x.y, psp->p->x.z);
  //   }
  // }
  // draw struts
  glLoadIdentity();
  // glBegin(GL_LINES);
  // for (int i = 0; i < mesh.struts().size(); ++i) {
  //   glColor4f( (1.0/mesh.struts().size())* i, 0.25, 0.5, 0.9);
  //   const physical_objects::Particle& pa = *mesh.struts()[i].vertice_pair.first;
  //   const physical_objects::Particle& pb = *mesh.struts()[i].vertice_pair.second;
    
  //   glTranslatef(pa.x.x, pa.x.y, pa.x.z);
    
  //   glVertex3d(pa.x.x, pa.x.y, pa.x.z);
  //   glVertex3d(pb.x.x, pb.x.y, pb.x.z);
  // }
  glLoadIdentity();
  glBegin(GL_TRIANGLES);
  glColor4f(0.25, 0.3, 1, 1.0);
  for (int i = 0; i < mesh.faces().size(); ++i) {
    // glColor4f( (1.0/mesh.struts().size())* i, 0.25, 0.5, 0.9);
    if(mesh.faces()[i].temporary_vertices.size() > 0) continue;
    const physical_objects::Particle& pa = *mesh.faces()[i].a;
    const physical_objects::Particle& pb = *mesh.faces()[i].b;
    const physical_objects::Particle& pc = *mesh.faces()[i].c;
    Vector3d norm = mesh.faces()[i].normal;
    glTranslatef(pa.x.x, pa.x.y, pa.x.z);
    glNormal3f(norm.x, norm.y, norm.z);
    glVertex3d(pa.x.x, pa.x.y, pa.x.z);
    glVertex3d(pb.x.x, pb.x.y, pb.x.z);
    glVertex3d(pc.x.x, pc.x.y, pc.x.z);

  }
  glEnd();
//  printf("Draw struts: %d\n", mesh.struts().size());
  
  // draw temporary vertices strut
  
  for ( int i = 0; i < mesh.faces().size(); ++i) {
    for (int j = 0; j < mesh.faces()[i].temporary_vertices.size(); ++j){
      if  (*mesh.faces()[i].temporary_vertices[j].isAttached) {
        
        const physical_objects::Particle& pa =
          *mesh.faces()[i].temporary_vertices[j].struts[0]->vertice_pair.first;
        const physical_objects::Particle& pb =
          *mesh.faces()[i].temporary_vertices[j].struts[0]->vertice_pair.second;
        const physical_objects::Particle& pc =
          *mesh.faces()[i].temporary_vertices[j].struts[1]->vertice_pair.second;
          const physical_objects::Particle& pd =
          *mesh.faces()[i].temporary_vertices[j].struts[2]->vertice_pair.second;
          // glTranslatef(pa.x.x, pa.x.y, pa.x.z);
        glBegin(GL_TRIANGLES);
          //face 1 abc
          glVertex3d(pa.x.x, pa.x.y, pa.x.z);
          glVertex3d(pb.x.x, pb.x.y, pb.x.z);
          glVertex3d(pc.x.x, pc.x.y, pc.x.z);
          Vector3d n = ((pc.x - pb.x) % (pa.x - pb.x)).normalize();
          glNormal3f(n.x, n.y, n.z);
          //face 2 acd
          glVertex3d(pa.x.x, pa.x.y, pa.x.z);
          glVertex3d(pc.x.x, pc.x.y, pc.x.z);
          glVertex3d(pd.x.x, pd.x.y, pd.x.z);
          n = ((pd.x - pc.x) % (pa.x - pc.x)).normalize();
          glNormal3f(n.x, n.y, n.z);
          //face 3 adb
          glVertex3d(pa.x.x, pa.x.y, pa.x.z);
          glVertex3d(pd.x.x, pd.x.y, pd.x.z);
          glVertex3d(pb.x.x, pb.x.y, pb.x.z);
          n = ((pb.x - pd.x) % (pa.x - pd.x)).normalize();
          glNormal3f(n.x, n.y, n.z);
        glEnd();
      }
    }
  }
  glColor4f(1,1,1, 0.9);
  
  
//  glTranslatef(0, 1, -4);
//  glutSolidSphere(0.5, 5, 5);
  
  // draw face normal
  if (drawFaceNormal) {
    for (int i = 0;  i < mesh.faces().size(); ++i){
      const physical_objects::Particle& pa = *mesh.faces()[i].a;
      const physical_objects::Particle& pb = *mesh.faces()[i].b;
      const physical_objects::Particle& pc = *mesh.faces()[i].c;

      Vector3d n = mesh.faces()[i].normal;
      Vector3d ctr = ((pa.x - pb.x) + (pc.x - pb.x))/3 + pb.x;
      glBegin(GL_LINES);
      glVertex3d(ctr.x, ctr.y, ctr.z);
      glVertex3d(ctr.x + n.x, ctr.y + n.y, ctr.z + n.z);
      glEnd();
    }
  }
  
}
//===============================================================
void draw_rigid_body(std::vector<physical_objects::DropingObject*> objs) {
//  printf("F_p:(%f, %f, %f)\n", objs->f.x,objs->f.y,objs->f.z);
  for (int i = 0; i < objs.size(); ++i) {
    glColor4f(0, 1, 1, 1);
    glLoadIdentity();
    glTranslatef(objs[i]->center->x.x, objs[i]->center->x.y + OBJRADIUS * 1.5, objs[i]->center->x.z);
    
    glutSolidSphere(OBJRADIUS, 15, 15);

  }
    
//  glVertex3f(p.x.x, p.x.y, p.x.z);

}
bool showGrid = false;

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
  camera = new Camera(Vector3d(-8.5, 7, 28), Vector3d(0, 0, 0),
                                                Vector3d(0, 1, 0));
  // grey background for window
  glClearColor(0.62, 0.62, 0.62, 0.0);
  glDepthRange(0.0, 1.0);
  
}

void mouseEventHandler(int button, int state, int x, int y) {
  // let the camera handle some specific mouse events (similar to maya)
  camera->HandleMouseEvent(button, state, x, y);
}

void motionEventHandler(int x, int y) {
  // let the camera handle some mouse motions if the camera is to be moved
  camera->HandleMouseMotion(x, y);
  // camera->Pos.print();
  // glutPostRedisplay();
}

void push(){
  double x = (rand() % 2000) /100.0 - 10.0, y = (rand() % 2000) / 100.0 - 10.0;
  double vy = - (rand() % 1000) /100.0;
  physical_objects::Particle * p = new physical_objects::Particle(x, 20.0, y, //x
                                                                  0.0, vy, 0.0, //v
                                                                  obj_mass, false);
  assert(p != NULL);
  physical_objects::DropingObject* dop =
    new physical_objects::DropingObject(p, false);

  dropping_objs.push_back(dop);

  // for(int i = 0; i < dropping_objs.size(); ++i) {
  //   printf("\t");
  //   dropping_objs[i]->center->x.print();
  // }
  // printf("\n");
  startframe = framecnt;
  bouncing_mesh->droping_object(dop, obj_spring, obj_mass);
//  bouncing_mesh->add_temp_spring(loc, vol, obj_spring, obj_damping, obj_mass);
}

void keyboardEventHandler(unsigned char key, int x, int y) {
//  float move_step = 0.15;
  switch (key) {
    case 'c': case 'C':
      // reset the camera to its initial position
      camera->Reset();
      break;
    case 'f': case 'F':
      camera->SetCenterOfFocus(Vector3d(0, 0, 0));
      break;
     case 's': case 'S':
       push();
       break;
    case 'g': case 'G':
      showGrid = !showGrid;
      break;
    case 'n': case 'N':
      drawFaceNormal = ! drawFaceNormal;
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
    case 'r': case 'R':
      Recording = !Recording;
      break;
    case 'q': case 'Q':	// q or esc - quit
    case 27:		// esc
      exit(0);
  }
  
  // glutPostRedisplay();
}

void rigid_object_simulation(){
  //testing with random behavior
//  rigid_objects->rotate(get_rand(0, 10), get_rand(-2.0, 2.0),
//                        get_rand(-2.0, 2.0),
//                        get_rand(-2.0, 2.0));
//  controller->next_step();
  for (int i = 0; i < dropping_objs.size(); ++i) {
    if ( !dropping_objs[i] ->isAttached ) {
      physical_objects::DropingObject::next_step(
                                    dropping_objs[i], current_time, deltaT);
    }
  }
  
}

void bouncing_mesh_simulation(){
  // printf("simu this frame\n");
  physical_objects::StateVector state =
    physical_objects::NumericalIntegrator::RK4Integrate(
          *bouncing_mesh, current_time, deltaT);
  // printf("\tI get new state\n");
  bouncing_mesh->update_particles(state);
  for (int i = 0; i < dropping_objs.size(); ++i) {
    bouncing_mesh->droping_object(dropping_objs[i], obj_spring, obj_damping);
  }
  // printf("\tFinished dropping_objs\n");
  framecnt ++;
  printf("%fs\tFrames:%d\n", framecnt * deltaT, (int) framecnt);
  if( framecnt - startframe == 2000 ) exit(0);
}

int cnt = 1;
//// simulation function that called in glIdle loop
void Simulate(){
//  printf("Frame: %d\n", cnt++);
  rigid_object_simulation();
  bouncing_mesh_simulation();
  // glutPostRedisplay();
//  usleep(13000);
}

void RenderBouncingMesh(){
  draw_bouncing_mesh(*bouncing_mesh);
}

void RenderRigidBody(){
  // glColor3f(0.0, 1.0, 1.0);
  
  // ModelObject *obj;
  // glTranslatef(0, 10.0, 0);
  // for(int i = 0 ; i < ObjLoader::objects.size(); ++i ) {
  //   obj = ObjLoader::ObjLoader::objects[i];
  //   // activate and specify pointer to vertex array
  //   glEnableClientState(GL_VERTEX_ARRAY);
  //   glVertexPointer(3, GL_FLOAT, 0, obj->vertices_array);
  //   glDrawElements(GL_POINT_BIT,
  //                  (int)obj->indices.size(),
  //                  GL_UNSIGNED_BYTE,
  //                  obj->indice_array);
    
  // }

}



/*
 On Redraw request, erase the window and redraw everything
 */
void drawEverything(){
  
  camera->PerspectiveDisplay(WIDTH, HEIGHT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  // Somewhere in the initialization part of your programâ¦
  
  // glEnable(GL_COLOR_MATERIAL);
  // Create light components
  float amb_factor = 0.5f;
  float ambientLight[] = { amb_factor, amb_factor, amb_factor, 1.0f };
  float diffuseLight[] = { 0.3f, 0.5, 2.0, 1.0f };
  float specularLight[] = { 0.8f, 0.8f, 0.5f, 0.5f };
  float position[] = { 10.0f, 25.0f, 10.0f, 0.0f };
  float ambient_color[4] = {1.0, 1.0, 1.0, 1.0};
  float diffuse_color[4] = {1.0, 1.0, 0.5, 1.0};
  float specular_color[4] = {1.0, 1.0, 0.5, 1.0};
  int shininess = 60; 
    // glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    // glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    // glMaterialf(GL_FRONT, GL_SHININESS, shininess);
  
  // // Assign created components to GL_LIGHT0
  float ambL1[] = { amb_factor * 0.5, amb_factor * 0.5, amb_factor* 0.5, 1.0f };
  float difL1[] = { 0.5, 0.5, 0.5 , 1.0f };
  float pos1[] = { -10, 10, 10, 0.0f };
  glLightfv(GL_LIGHT1, GL_POSITION, pos1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambL1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, difL1);

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST );
  glEnable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glutPostRedisplay();

  if (showGrid)
    makeGrid();
  
  

  RenderBouncingMesh();

  float balldiffuseLight[] = { 1.0f, 1, 1.0, 1.0f };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, balldiffuseLight);
  draw_rigid_body(dropping_objs);

//  RenderRigidBody();
  glutSwapBuffers();
  
}

void RenderScene(){
  
  drawEverything();
  glutSwapBuffers();
  glutPostRedisplay();
//  if(Recording)
//    framegrabber.recordimage(drawEverything);
}
void init_rigid_object_world(char argc, char **argv){
  int obj_no = 0 ;
  ObjLoader::loadObject(argv[1], obj_no);
  if (obj_no < 1) {
    printf("Loading object from %s error, exit", argv[1]);
    exit(1);
  }
//  ObjLoader::objects[0]->rotate(90, 0, 0, 0);
  rigid_objects = ObjLoader::objects[0];
  
  controller = new MotionController(rigid_objects, 0.0, 0.01);
  rigid_objects->rigid_body.omega = Vector3d(0.1, 0.01, 0.01);
  
  rigid_objects->rigid_body.Ibody = Matrix3x3(1.0, 0.0, 0.0,
                                              0.0, 1.0, 0.0,
                                              0.0, 0.0, 1.0);
}

void init_bouncing_mesh(){
  bouncing_mesh = new physical_objects::BouncingMesh(mesh_ulx, 0, mesh_uly,
                                                     Abs(mesh_ulx*2),
                                                     Abs(mesh_uly*2),
                                                     mesh_subdiv, mesh_v_mass,
                                                     mesh_spring, mesh_damping ); //spring and d
//  push();
}

// set up something in the world
void init_the_world(char argc, char **argv) {
  init_bouncing_mesh();
  //  init_rigid_object_world(argc, argv);
  
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
  if(fscanf(paramfile, "%lf %lf %lf %lf %lf %lf %f %lf %lf %d",
            &mesh_spring, &mesh_damping, &mesh_v_mass, &obj_mass, &obj_spring,
            &obj_damping, &deltaT, &mesh_ulx, &mesh_uly, &mesh_subdiv) != 10){
    fprintf(stderr, "error reading parameter file %s\n", filename);
    fclose(paramfile);
    exit(1);
  }
//  deltaT = 0.005;
}

static char* parafile;

void Reset(){
  LoadParameters(parafile);
}


/*
 Main program to draw the square, change colors, and wait for quit
 */
int main(int argc, char* argv[]){
  if(argc != 2){
    fprintf(stderr, "usage: boucing paramfile\n");
    exit(1);
  }
  LoadParameters(argv[1]);
  parafile = argv[1];
  
  init_the_world(argc, argv);
  printf("R reset camera\nG toggle grid\nPress S to drop ball\nPress N to draw face normal\nQ quit\n");
  
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
