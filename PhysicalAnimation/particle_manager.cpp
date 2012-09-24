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
//debug
#include "stdio.h"


namespace particle_manager {
  float get_rand(float low, float high) {
//    return ((rand() % 1000) / 1000.0) * (high - low) + low;
    return gauss( (high+low)/2, (high+low)/9, (int) time(NULL));
  }
  
  
  Vector3d ParticleManager::get_init_loc(Vector3d& gen_pl_orig) {
    return gen_pl_orig +
      get_rand(0.0, gen_pl_width_) *  Ux +
      get_rand(0.0, gen_pl_height_) * Uy;
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
    Vector3d& x0 = this->gen_pl_origin_;
    Vector3d plane_norm = Ux % Uy;
    
    
    Vector3d loc;
    for ( int i = 0 ; i < this->particle_size_; ++i ) {
//      Vector3d tm = x0 ;//+ get_rand(0.1, 0.5) * Uy;
//      printf("test rand: %f, %f, %f \n", tm.x, tm.y, tm.z );
      loc = get_init_loc(x0);
      //generate each particle
      
      physical_objects::Particle* particle =
        new physical_objects::Particle(
          Vector4d(0.0, 0.0, 1.0, 1.0), // color should be blue
           loc, // location
           plane_norm * gauss(speed_mean_, speed_var_, seed), // velocity
           PARTICLE_AGE // max_age
      );
      particle->age_ = 0.0 - ((rand() % 1000) / 1000.0f) * PARTICLE_AGE;
      this->particles_.push_back(particle);
      
//      printf("%dth part vol: %f, %f, %f \n", i,
//             particle->velocity().x,
//             particle->velocity().y,
//             particle->velocity().z);
    }
  }
  
  void ParticleManager::move_particles(float time_step, Vector3d& obs_ctr,
                                       float obsrad){
    int seed = (int) time(NULL);
    Vector3d plane_norm = Ux % Uy;
    for ( int i = 0 ; i < this->particles_.size(); ++i ) {
      physical_objects::Particle* par = this->particles_.at(i);
      par->move(time_step, obs_ctr, obsrad);
      if ( !par->is_alive ) {
        dead_particles_.push(par);
        par->location() = get_init_loc(this->gen_pl_origin_);
        par->velocity() = plane_norm * gauss(speed_mean_, speed_var_, seed);
//        if (dead_particles_.size() < particle_size_/10 ) {
//          par->age_ = 0.0;
//        } else { 
//          par->age_ = 0.0 - ((rand() % 1000) / 1000.0f) * PARTICLE_AGE;
//        }
        par->age_ = 0.0;
        if ( dead_particles_.size() != 0 ) {
          dead_particles_.front()->is_alive = true;
          dead_particles_.pop();
        }
      }
      
    }
  }
  
  
  
} // ns particle_manager