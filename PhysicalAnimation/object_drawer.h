//
//  object_drawer.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/23/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef PhysicalAnimation_object_drawer_h
#define PhysicalAnimation_object_drawer_h

#include "object.h"
#include "stdio.h"
#include<cmath>

#define RECTSIZE 0.05

void draw_point(Vector3d& loc, float rectsize) {
  glVertex3d(loc.x , loc.y, loc.z);
  glVertex3d(loc.x, loc.y + rectsize, loc.z);
  glVertex3d(loc.x + rectsize, loc.y + rectsize, loc.z);
  glVertex3d(loc.x + rectsize, loc.y, loc.z + rectsize);
}


void draw_particles(const std::vector<physical_objects::Particle*>& particles){
  //rotate the square so that is face camera
  GLdouble m[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, m);
  glPushMatrix();
  glRotatef(-acos(m[10]), m[6], -m[2], 0.0);
  
  glBegin(GL_QUADS);
//  printf("Particles: %d\n",(int) particles.size());
  
  for (int i = 0 ; i < particles.size(); ++i ) {
    physical_objects::Particle* par = particles.at(i);
    if (!par->is_alive || par->age_ < 0.0) {
      // skip if not alive
      continue;
    }
    Vector3d& loc = par->location();
    float speed_threshold = 4.0f;
    if ( par->mass() < 0.1 ) {
      glColor4f(1.0, 1.0, 1.0, 1.0);
      draw_point(loc, RECTSIZE/4);

    } else {
    glColor4f( 0.33 * (1.0 + fmin(1.0, par -> velocity().norm() / speed_threshold )),
              0.66 + 0.1 * (1 + fmin(1.0, par -> velocity().norm() / speed_threshold)),
              1.0,
              0.8 + 0.1 * (fmin(1, par -> velocity().norm() / speed_threshold )));
      draw_point(loc, RECTSIZE);

    }
      //    printf("%d point: %f, %f, %f\n",i,
//           loc.x, loc.y, loc.z);
//    glVertex3f(loc.x, loc.y, loc.z);
  }

  glEnd();
  
  //end rotate
  glPopMatrix();
}

void draw_obstancles(Vector3d* obses){
//  int listID = glGenLists(1);
//  glNewList(listID, glCheckFramebufferStatus(<#GLenum target#>))
  glColor4f(0.15, 0.2, 0.0, 0.9);
  glClear(GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glTranslatef(obses[0].x, obses[0].y, obses[0].z);
  glutSolidSphere(obs2rad, 20, 20);
  glPopMatrix();
}



#endif
