//
//  object.cpp
//  PhysicalAnimation
//
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//  
//  Author: cosmobserver@gmail.com (Julian Wu )
//    9/9/12.

#include "object.h"

#include<utility>
using namespace std;

namespace physical_objects{

  // external definition of template is unsupported
//  template<class MotionVector>
//  void ball<MotionVector>::move(DrawObject_collision disp_func,
//                                float time_step) {
//    this->time_step_ = time_step;
//    
//    MotionVector tmp_a, tmp_v, tmp_x;
//    
//    tmp_a = g() - drag_coeff() / mass() *
//            medium_speed_ - velocity_ ;
//    disp_func(0, this);
//    
//    tmp_v = velocity_ + accel_ * time_step;
//    tmp_x = location_ + velocity_ * time_step;
//    //if collison do sth
//    
//    
//    velocity_ = tmp_v;
//    location_ = tmp_x;
//    accel_ = tmp_a;
//    current_time_ += time_step;
//  }
  
  Strut::Strut(float spring, float damper, float original_len,
                   Struts_Vertice& a, Struts_Vertice& b) :
  K(spring), D(damper), L0(original_len) {
    this->vertices_pair = make_pair(&a, &b);
  }

  
  surface::surface(int width, int height, Vector3d Center, int subdivide,
                   float spring, float damper, float mass) {
    int wstep = width / (subdivide + 1), hstep = height / (subdivide + 1);
    int vertice_no = (subdivide + 2) * (subdivide + 2);
    //int strut_no = (subdivide + 1) * (3 * subdivide + 5);
    int strut_no = (subdivide + 1) * (4 * subdivide + 6); // for all connection
    subdividion_no = subdivide;
    
    for(int i = 0 ; i < subdivide + 2; ++i) {
      for(int j = 0; j < subdivide + 2; ++j) {
        vertices.push_back(
                           Struts_Vertice(
                                          mass, // mass
                                          Vector3d(0.0, 0.0, 0.0), // force
                                          Vector3d(Center.x - width / 2 + (int) (wstep * j),
                                                   0.0,
                                                   Center.z - hstep / 2 + (int) (hstep * i)
                                                   ), //location
                                          Vector3d(0.0, 0.0, 0.0) // velocity
                                          
                                          )
                           );
      }
    }
    
    Vector3d current_loc = vertices[0].location;
    
    for(int i = 0 ; i < subdivide + 2; ++i) {
      for(int j = 0; j < subdivide + 2; ++j) {
        Struts_Vertice& curr_vtx = vertices[i * (subdivide + 2) + j];
        current_loc = curr_vtx.location;
        
        // right i + 1, j
        if ( i + 1 < subdivide + 2 ) {
          Strut* strut = new Strut(spring, damper,
             (vertices[(i + 1) * ( subdivide + 2 ) + j].location - current_loc).norm(),
             curr_vtx, vertices[(i + 1) * ( subdivide + 2 ) + j]
             );
          struts.push_back(
                           *strut
                           );
          curr_vtx.add_connect_strut(*strut);
        }
        // right down i+1, j+1
        if ( i + 1 < subdivide + 2 && j + 1 < subdivide ){
          Strut* strut = new Strut(spring, damper,
             (vertices[(i+1) * ( subdivide + 2 ) + j + 1].location - current_loc).norm(),
             curr_vtx, vertices[(i+1) * ( subdivide + 2 ) + j + 1]);
          struts.push_back(
                            *strut
                            );
           curr_vtx.add_connect_strut(*strut);
        }
        
        // down i, j + 1
        if ( j + 1 < subdivide + 2) {
           Strut* strut = new Strut(spring, damper,
                     (vertices[i * ( subdivide + 2 ) + j + 1].location - current_loc).norm(),
                                   curr_vtx, vertices[i * ( subdivide + 2 ) + j + 1] );
            struts.push_back(
                             *strut
                             );
            curr_vtx.add_connect_strut(*strut);
          }
        //left down i - 1, j - 1
        if ( j - 1 < 0 || i - 1  < 0 ){
          Strut* strut = new Strut(spring, damper,
                                   (vertices[(i-1)* ( subdivide + 2 ) + j - 1 ].location - current_loc).norm(),
                                   curr_vtx, vertices[(i-1)* ( subdivide + 2 ) + j - 1 ]);
            struts.push_back(
                             *strut
                             );
            curr_vtx.add_connect_strut(*strut);
          }
    
      }
    }
    
  }
  
  void surface::calculate_dynamics() {
    //for each vertices set force to 0
    for(int i = 0; i < this->vertices.size(); ++i) {
      vertices[i].force = Vector3d(0.0, 0.0, 0.0);
      
    }
    // for each vertices calculate its fs_ij and fd_ij
    for(int i = 0; i < this->struts.size(); ++i) {
      Struts_Vertice* a = this->struts[i].vertices_pair.first;
      Struts_Vertice* b = this->struts[i].vertices_pair.second;
      Vector3d uab = (a->location - b->location) /
                      (a->location - b->location).norm();
      Vector3d fsab = this->struts[i].K * (
        (a->location - b->location).norm() - this->struts[i].L0) * uab;
      Vector3d fdab = this->struts[i].D * (
        (b->velocity - a->velocity) * uab * uab
      );
      a->force = a->force + (fsab + fdab);
      b->force = b->force - (fsab + fdab);
    }
    
    // for all vertices add external force. here is zero
    
    this->vertices[subdividion_no].velocity.y = 10;
    
    for(int i = 0; i < this->vertices.size(); ++i) {
      vertices[i].accel = vertices[i].force / vertices[i].mass;
      
    }
  }
  

  
} //ns