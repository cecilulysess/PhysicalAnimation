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

#define rectsize 0.05

void draw_point(Vector3d& loc) {
  glVertex3d(loc.x , loc.y, loc.z);
  glVertex3d(loc.x, loc.y + rectsize, loc.z);
  glVertex3d(loc.x + rectsize, loc.y + rectsize, loc.z);
  glVertex3d(loc.x + rectsize, loc.y, loc.z);
}


void draw_particles(const std::vector<physical_objects::Particle*>& particles){
  glBegin(GL_QUADS);
//  printf("Particles: %d\n",(int) particles.size());
  
  for (int i = 0 ; i < particles.size(); ++i ) {
    physical_objects::Particle* par = particles.at(i);
    Vector3d& loc = par->location();
    if ( par -> velocity().norm() < 0.8 ) {
      glColor4f(0.33, 0.66, 1.0, 0.8);
    } else {
      glColor4f(0.66, 0.88, 1.0, 0.9);
    }
    draw_point(loc);
//    printf("%d point: %f, %f, %f\n",i,
//           loc.x, loc.y, loc.z);
//    glVertex3f(loc.x, loc.y, loc.z);
  }
//  glVertex3f(0.0, 0.0, 0.0);
//  //  glColor4f(0.0, 1.0, 0.0, 0.5);
//  glVertex3f(1.0, 0.0, 0.0);
//  //  glColor4f(0.0, 0.0, 1.0, 0.5);
//  glVertex3f(1.0, 1.0, 0.0);
//  glVertex3f(0.0, 1.0, 0.0 );
  glEnd();
}


#endif
