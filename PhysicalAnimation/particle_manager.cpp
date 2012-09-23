//
//  particle_manager.cpp
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/23/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#include "particle_manager.h"
#include "gauss.h"
#include "stdlib.h"

namespace particle_manager {
  float get_rand(float low, float high) {
    return ((rand() % 1000) / 1000.0) * (high - low) + low;
  }
  
  //
  //   P1-------width--------P2
  //   |                      |
  //   | height               |
  //   | Uy                   |
  //   |                      |
  //   P0-Ux-----------------P3
  //
  void ParticleManager::init() {
    int seed = (int) time(NULL);
    Vector3d& x0 = this->gen_pl_origin;
    Vector3d Ux(1.0, 0.0, 0.0), Uy(0.0, 1.0, 0.0);
    Vector3d plane_norm = Ux % Uy;
    
    float width = this->gen_pl_width, height = this->gen_pl_height;
    Vector3d loc;
    for ( int i = 0 ; i < this->particle_size_; ++i ) {
      loc = x0 + get_rand(0.0, width) *  Ux + get_rand(0.0, height) * Uy;
      //generate each particle
      
      physical_objects::Particle* particle =
        new physical_objects::Particle(
          Vector4d(0.0, 0.0, 1.0, 1.0), // color should be blue
           loc, // location
           plane_norm * gauss(speed_mean_, speed_var_, seed)
      );
      this->particles_.push_back(particle);
      
    }
  }
  
  
  
} // ns particle_manager