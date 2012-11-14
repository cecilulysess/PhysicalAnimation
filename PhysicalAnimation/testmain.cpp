////
////  testmain.c
////  PhysicalAnimation
////
////  Created by Julian Wu on 11/9/12.
////  Copyright (c) 2012 Julian Wu. All rights reserved.
////
//
//#include <stdio.h>
//#include <vector>
//#ifdef __APPLE__
//#  include <GLUT/glut.h>
//#else
//#  include <GL/glut.h>
//#endif
//
//#include "ObjLoader.h"
//#include "Camera.h"
//#
//using namespace std;
//
//Camera *camera = new Camera(Vector3d(0, 5, 4), Vector3d(0, 0, 0),
//                                                Vector3d(0, 1, 0));
//
//physical_objects::ModelObject *obj;
//
//void renderScene(void) {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  camera->PerspectiveDisplay(800, 600);
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//
////	glBegin(GL_TRIANGLES);
////  glVertex3f(-0.5,-0.5,0.0);
////  glVertex3f(0.5,0.0,0.0);
////  glVertex3f(0.0,0.5,0.0);
////	glEnd();

////----------debug usage
//GLfloat grip[] = {
//  1.000000,-1.0000000,-1.000000,  //0
//  1.000000,-1.000000,1.000000,    //1
//  -1.000000,-1.000000,1.000000,   //2
//  -1.000000,-1.000000,-1.000000,  //3
//  1.000000,1.000000,-0.999999,    //4
//  0.999999,1.000000,1.000001,     //5
//  -1.000000,1.000000,1.000000,    //6
//                                  //    -1.000000,1.000000,-1.000000,
//  -1.000000,1.000000,-1.000000};  //7
//
//vector<GLfloat> gripv(grip, grip + sizeof(grip) / sizeof(GLfloat));
//GLubyte indices[] = {
//  //    0, 1, 2, 3,
//  //    4, 7, 6, 5,
//  //    0, 4, 5, 1,
//  //    1, 5, 6, 3,
//  //    2, 6, 7 ,3,
//  //    4, 0, 3, 7};
//  //    1, 2, 3, 4,
//  //    5, 8, 7, 6,
//  //    1, 5, 6, 2,
//  //    2, 6, 7, 3,
//  //    3, 7, 8, 4,
//  //    5, 1, 4, 8 };
//  2, 3, 4, 5,
//  6, 9, 8, 7,
//  2, 6, 7, 3,
//  3, 7, 8, 4,
//  4, 8, 9, 5,
//  6, 2, 5, 9 };
////------------


//////
////  GLfloat grip[] = {1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
////    -1.0f, -1.0f, 0.0f, 1.0f, -1.0f};
////  printf("%d, %d\n", sizeof(grip), sizeof(GLfloat));
////  vector<GLfloat> gripv(grip, grip + sizeof(grip) / sizeof(GLfloat));
////  GLubyte indices[] = {0, 1, 2, 3};
//
//  // activate and specify pointer to vertex array
//  glEnableClientState(GL_VERTEX_ARRAY);
//  glVertexPointer(3, GL_FLOAT, 0, &obj->vertices);
//
//  // draw first half, range is 6 - 0 + 1 = 7 vertices used
//  const GLvoid* indice_array = &obj->indices;
//  glDrawRangeElements(GL_QUADS, 0, obj->indices.size()-1, obj->indices.size(),
//                      GL_UNSIGNED_BYTE, indice_array);
//
//  // draw second half, range is 7 - 1 + 1 = 7 vertices used
////  glDrawRangeElements(GL_TRIANGLES, 1, 7, 18, GL_UNSIGNED_BYTE, indices+18);
//
//  // deactivate vertex arrays after drawing
//  glDisableClientState(GL_VERTEX_ARRAY);
//  
//	glutSwapBuffers();
//}
//
//void motionEventHandler(int x, int y) {
//  // let the camera handle some mouse motions if the camera is to be moved
//  camera->HandleMouseMotion(x, y);
//  glutPostRedisplay();
//}
//
//void RenderScene(){
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  camera->PerspectiveDisplay(800, 600);
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//////
////  GLfloat grip[] = {1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
////    -1.0f, -1.0f, 0.0f, 1.0f, -1.0f};
////  printf("%d, %d\n", sizeof(grip), sizeof(GLfloat));
////  vector<GLfloat> gripv(grip, grip + sizeof(grip) / sizeof(GLfloat));
////  GLubyte indices[] = {0, 1, 2, 3};
////  
////  // activate and specify pointer to vertex array
////  glEnableClientState(GL_VERTEX_ARRAY);
////  glVertexPointer(3, GL_FLOAT, 0, &gripv);
////  
////  // draw first half, range is 6 - 0 + 1 = 7 vertices used
////  glDrawRangeElements(GL_TRIANGLES, 0, 6, 18, GL_UNSIGNED_BYTE, indices);
////  
////  // draw second half, range is 7 - 1 + 1 = 7 vertices used
////  glDrawRangeElements(GL_TRIANGLES, 1, 7, 18, GL_UNSIGNED_BYTE, indices+18);
////  
////  // deactivate vertex arrays after drawing
////  glDisableClientState(GL_VERTEX_ARRAY);
////  glutSwapBuffers();
//
//}
//
//void doReshape(int w, int h){
//  int vpw, vph;
//  
//  float aspect = float(800) / float(600);
//  if(float(w) / float(h) > aspect){
//    vph = h;
//    vpw = int(aspect * h + 0.5);
//  }
//  else{
//    vpw = w;
//    vph = int(w / aspect + 0.5);
//  }
//  
//  glViewport(0, 0, vpw, vph);
//    
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
////  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
//  
//  glMatrixMode(GL_MODELVIEW);
//}
//
//void mouseEventHandler(int button, int state, int x, int y) {
//  // let the camera handle some specific mouse events (similar to maya)
//  camera->HandleMouseEvent(button, state, x, y);
//}
//
//int main(int argc, char **argv){
//  obj = ObjLoader::loadObject(argv[1]);
//  obj->print();
//  /// init GLUT and create Window
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(100,100);
//	glutInitWindowSize(320,320);
//	glutCreateWindow("Lighthouse3D - GLUT Tutorial");
//  
//	// register callbacks
//	glutDisplayFunc(renderScene);
//  glutMouseFunc(mouseEventHandler);
//  glutMotionFunc(motionEventHandler);
//
//	// enter GLUT event processing cycle
//	glutMainLoop();
//	
//	return 1;
////  if(argc != 2){
////    fprintf(stderr, "usage: bounce objfile\n");
////    exit(1);
////  }
////
////  
////  glutInit(&argc, argv);
////  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
////  glutInitWindowPosition(100,100);
////  glutInitWindowPosition(1024, 600);
////  glutCreateWindow("test");
////
//////  glutReshapeFunc(doReshape);
////  glutDisplayFunc(RenderScene);
////  
////  glutMainLoop();
//  
//}
