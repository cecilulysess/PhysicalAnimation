//
//  physical_object.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 11/25/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef __PhysicalAnimation__physical_object__
#define __PhysicalAnimation__physical_object__

#include <vector>
#include <utility>
#include "Vector.h"


namespace physical_objects{
  
  // StateVector store value as Vector3d or 4d in its own terms.
  // This is collaboration with the NumericalIntegrator
  typedef struct StateVector {
  public:
    explicit StateVector(int size);
    ~StateVector();
    
    std::vector<Vector3d> state;
    int size;
//    StateVector operator*(double s) const{
//      StateVector res(size);
//      for (int i = 0 ; i < 2 * size; ++i ) {
//        res.state[i] = this->state[i] * s;
//      }
//      return res;
//    }
    
    friend StateVector operator*(double l, const StateVector& r);
    friend StateVector operator*(const StateVector& l, double r);
    
    const StateVector& operator=(const StateVector& v2) {
      for ( int i = 0 ; i < size; ++i ) {
        this->state[i] = v2.state[i];
      }
      return *this;
    }
    
    StateVector operator+(const StateVector& v2) {
      StateVector res(size);
      for (int i = 0 ; i < size; ++i ) {
        res.state[i] = (this->state[i] + v2.state[i]);
      }
      return res;
    }
    
  void print(){
    for (int i = 0; i < this->size; ++i ){
      printf("\tStateV, %d: (%f, %f, %f)\n",
             i, this->state[i].x, this->state[i].y, this->state[i].z);
    }
    printf("============END============\n");
  }
  }StateVector;

  // a particle in the particle system
  typedef struct Particle {
    Particle(float x, float y, float z,
             float vx, float vy, float vz,
             float m, bool is_pivot){
      this->x = Vector3d(x, y, z);
      this->v = Vector3d(vx, vy, vz);
      this->m = m;
      this->f = Vector3d(0,0,0);
      this->is_pivot = is_pivot;
      N = 0;
      connected_particles = std::vector<Particle*>();
    }
    
    //locaiton in space
    Vector3d x;
    // speed in space
    Vector3d v;
    // force applied to this particle
    Vector3d f;
    // mass;
    float    m;
    // is static particle that never applied any force
    bool     is_pivot;
    
    std::vector<Particle*> connected_particles;
    // number of particles
    int N;
    
  } Particle;
  
  // a strut contains the information of the spring
  typedef struct Strut {
  public:
    Strut(float spring, float damper, float original_len,
          Particle* a, Particle* b): K(spring), D(damper), L0(original_len){
      this->vertice_pair = std::make_pair(a, b);
    }
    
    
    float K, D, L0; //spring, damper, L_0
    std::pair<Particle*, Particle*> vertice_pair;
  }Strut;
  
  // BouncingMesh is a bouncing object that bounce everything from its surface
  class BouncingMesh {
  public:
    // construct a new bouncing mesh with upper-left point (x, y, z) and
    // normal as (0, 1, 0);
    BouncingMesh(float x, float y, float z,
                 float width, float height, int division,
                 float mass, float strut_spring, float strut_damp);
    ~BouncingMesh();
    
    // compute force to each particle with state X and at time t
    void compute_force(StateVector& X, float t);
    
    // X' = dynamic(X, t);
    StateVector dynamic(StateVector X, float t);
    
    //get the state vector of this object
    StateVector& state_vector();
    // update the location and velocity of particles by given state
    void update_particles(StateVector& state);
    
    const std::vector<Particle>& mesh_particles();
    const std::vector<Strut> struts();
    
  private:
    
    // clear the force for all particle
    void clear_force();
    
    // create springs when get all particle setted, with spring and damping
    // coefficient
    void create_springs(float spring, float damping);
    
    StateVector state_vector_;
    
    std::vector<Particle> mesh_particles_;
    std::vector<Strut> struts_;
    // number of particles
    int N;
    // array_width
    int array_width;
  };
  
  class NumericalIntegrator {
  public:
    // RK4 integrate solve the differential equation
    // X' = sv->dynamic(X, t) using timestep deltaT
    // X_{t+deltaT} = X_{t} + deltaT * (K1 + 2K2 + 2K3 + K4)/ 6
    // this function return the state vector of X_t + deltaT
    static StateVector RK4Integrate(BouncingMesh& sv, float t,
                                     float deltaT);
  };

}//ns physical_objects
#endif /* defined(__PhysicalAnimation__physical_object__) */
