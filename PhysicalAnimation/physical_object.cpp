//
//  physical_object.cpp
//  PhysicalAnimation
//
//  Created by Julian Wu on 11/25/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//
#include <vector>
#include "Utility.h"
#include "physical_object.h"

namespace physical_objects {
  inline int idx(int i, int j, int width) {
    return i * width + j;
  }
  BouncingMesh::BouncingMesh(float x, float y, float z,
                             float width, float height, int division,
                             float mass){
    this->mesh_particles_ = std::vector<Particle>();
    
    this->N = Sqr(2 + division);
    float step_w = width / (1.0 + division),
          step_h = height / (1.0 + division);
    array_width = 2 + division;
    bool is_p;
    for (int i = 0; i < array_width; ++i) {
      for (int j = 0; j < array_width; ++j) {
        is_p = (i == 0 || i == (array_width - 1) ||
                j == 0 || j == (array_width - 1));
        mesh_particles_.push_back(
             Particle( x + i * step_w, y, z - j * step_h, //location
                       0, 0, 0, // speed
                       mass, // mass
                       is_p
                    )
        );
              
      }
    }
    create_springs();
  }
  
  BouncingMesh::~BouncingMesh(){
  
  }
  
  void BouncingMesh::clear_force(){
    for (int i = 0; i < N; ++i) {
      mesh_particles_[i].f = Vector3d(0, 0, 0);
    }
  }
  
  void BouncingMesh::create_springs(){
    for (int i = 0; i < array_width; ++i) {
      for ( int j = 0; j < array_width; ++j) {
        std::vector<Particle*>& ref =
          mesh_particles_[idx(i, j, array_width)].connected_particles;
        if ( i - 1 > 0) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i - 1, j, array_width)]);
        }
        if ( j - 1 > 0 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i, j - 1, array_width)]);
        }
        if ( i + 1 < array_width - 1 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i + 1, j, array_width)]);
          
        }
        if ( j + 1 < array_width - 1) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i, j + 1, array_width)]);
        }
        
        
//        if ( i == 0 || j == 0 || i == array_width - 1 || j == array_width - 1 ) {
//          if ((i == 0 || i == array_width - 1)
//              &&
//              (j == 0 || j == array_width - 1)) {
//            mesh_particles_[idx(i, j, array_width)].N = 2;
//            
//            if ( i == 0 )
//              ref.push_back(&mesh_particles_[idx(i + 1, j, array_width)]);
//            else
//              ref.push_back(&mesh_particles_[idx(i - 1, j, array_width)]);
//            if ( j == 0 )
//              ref.push_back(&mesh_particles_[idx(i, j + 1, array_width)]);
//            else
//              ref.push_back(&mesh_particles_[idx(i, j - 1, array_width)]);
//          } else {
//            // else is on one edge but not corner
//            mesh_particles_[idx(i, j, array_width)].N = 3;
//            
//          }
//          
//          
//          
//          
//        } else {
//          // else is inner particle
//          mesh_particles_[idx(i, j, array_width)].N = 4;
//          ref.push_back(&mesh_particles_[idx(i - 1, j, array_width)]);
//          ref.push_back(&mesh_particles_[idx(i + 1, j, array_width)]);
//          ref.push_back(&mesh_particles_[idx(i, j - 1, array_width)]);
//          ref.push_back(&mesh_particles_[idx(i, j + 1, array_width)]);
//        }
      }
    }
  }
  
  StateVector& BouncingMesh::state_vector(){
    return this->state_vector_;
  }
  
  const std::vector<Particle>& BouncingMesh::mesh_particles(){
    return this->mesh_particles_;
  }
  
  void NumericalIntegrator::Integrate(StateVector& sv, float deltaT){
    
  }
  
  
}