//
//  physical_object.cpp
//  PhysicalAnimation
//
//  Created by Julian Wu on 11/25/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#include <set>

#include "Utility.h"
#include "physical_object.h"



namespace physical_objects {
  StateVector::StateVector(int size){
    
    this->size = size;
    for (int i = 0; i < size; ++i) {
      this->state.push_back(Vector3d(0,0,0));
    }
  }
  
  StateVector::~StateVector(){
//    delete &state; no need, delete auto
  }
  
  StateVector operator*(double l, const StateVector& r){
    StateVector res(r.size);
    for (int i = 0 ; i < r.size; ++i ) {
      res.state[i] = r.state[i] * l;
    }
    return res;
  }
  
  StateVector operator*(const StateVector& l, double r){
    StateVector res(l.size);
    for (int i = 0 ; i < l.size; ++i ) {
      res.state[i] = l.state[i] * r;
    }
    return res;
  }
  
  void BouncingMesh::compute_force(StateVector& X, float t){
    Vector3d g(0, -0.98, 0);
    clear_force();
    float mass = 0.0;
    // apply unary force to each particle with state X
    for (int i = 0; i < this->mesh_particles_.size(); ++i) {
      if ( this->mesh_particles_[i].is_pivot ) {
        this->mesh_particles_[i].f = Vector3d(0, 0, 0);
      } else {
        mass = mesh_particles_[i].m;
//        if ( i == this->mesh_particles().size() / 2) mass = 10 * mesh_particles_[i].m;
        this->mesh_particles_[i].f =  g * mass;
//        if (mass > 0.2) this->mesh_particles_[i].f = g * mass;
      }
    }
    // apply nary force 
    for (int i = 0; i < this->struts().size(); ++i) {
      Particle* a = this->struts_[i].vertice_pair.first;
      Particle* b = this->struts_[i].vertice_pair.second;
      Vector3d L = a->x - b->x;
      Vector3d uab = L / L.norm();
      float delta_L = (L.norm() - this->struts_[i].L0);
      //            printf("d_L(%d, %d) :%f\n", a->vertice_id, b->vertice_id, delta_L);
      Vector3d fsab = - this->struts_[i].K * delta_L * uab;
      //            printf("%d: fsab: %f\n", i, fsab.norm());
      Vector3d fdab = this->struts_[i].D * (
                                            //Vb-Va
                                            (b->v - a->v) * uab * uab
                                            );
      //      printf("%d: fs: (%f,%f,%f), fd: (%f,%f,%f)\n", i, fsab.x, fsab.y, fsab.z,
      //                         fdab.x, fdab.y, fdab.z);
      
      if (!a->is_pivot)
        a->f = a->f + (fsab + fdab);
      if (!b->is_pivot)
        b->f = b->f - (fsab + fdab);
    }

  }
  
  inline int idx(int i, int j, int width) {
    return i * width + j;
  }
  BouncingMesh::BouncingMesh(float x, float y, float z,
                             float width, float height, int division,
                             float mass, float strut_spring, float strut_damp)
  : state_vector_(2 * Sqr(2 + division)){
    
    this->mesh_particles_ = std::vector<Particle>();
    this->struts_ = std::vector<Strut>();
    
    this->N = Sqr(2 + division);
    array_width = 2 + division;
    // finished init members
    
    float step_w = width / (1.0 + division),
          step_h = height / (1.0 + division);
    
    bool is_p;
    for (int i = 0; i < array_width; ++i) {
      for (int j = 0; j < array_width; ++j) {
        is_p = (i == 0 || i == (array_width - 1) ||
                j == 0 || j == (array_width - 1));
        mesh_particles_.push_back(
             Particle( x + j * step_w, y, z + i * step_h, //location
             0, 0, 0, // speed
             mass,    //(i == array_width / 2 && j == array_width / 2 ? 100* mass : mass), // mass
             is_p
                    )
        );
              
      }
    }
    create_springs(strut_spring, strut_damp);
  }
  
  BouncingMesh::~BouncingMesh(){
//    delete &this->struts_;
//    delete &this->mesh_particles_;
//    delete &this->state_vector_;
  }
  
  void BouncingMesh::clear_force(){
    for (int i = 0; i < N; ++i) {
      mesh_particles_[i].f = Vector3d(0, 0, 0);
    }
  }
  
  void BouncingMesh::create_springs(float spring, float damping){
    for (int i = 0; i < array_width; ++i) {
      for ( int j = 0; j < array_width; ++j) {
        std::vector<Particle*>& ref =
          mesh_particles_[idx(i, j, array_width)].connected_particles;
        if ( i + 1 < array_width - 1 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i + 1, j, array_width)]);
          
        }
        // add diagonal ============================================
        if ( i + 1 < array_width - 1 && j + 1 < array_width - 1) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i + 1, j + 1, array_width)]);
        }
        //==========================================================
        if ( j + 1 < array_width - 1) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i, j + 1, array_width)]);
        }
        
        // add diagonal ============================================
        if ( i - 1 > 0 && j + 1 < array_width - 1 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i - 1, j + 1, array_width)]);
        }
        // =========================================================
        if ( i - 1 > 0) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i - 1, j, array_width)]);
        }
        // add diagonal ============================================
        if ( i - 1 > 0 && j - 1 > 0 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i - 1, j - 1, array_width)]);
        }
        // =========================================================
        if ( j - 1 > 0 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i, j - 1, array_width)]);
        }
        // add diagonal ============================================
        if ( i + 1 < array_width - 1 && j - 1 > 0 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i + 1, j - 1, array_width)]);
        }
        // =========================================================
        
        // ------------------ finished add adjencent particle------------
       
      }
    }
    
    // ============================create struts=========================
    for (int i = 0; i < array_width; ++i) {
      for ( int j = 0; j < array_width; ++j) {
        Particle& ref =
          mesh_particles_[idx(i, j, array_width)];
        if (i + 1 < array_width) {
          Particle& next = mesh_particles_[idx(i + 1, j, array_width)];
          this->struts_.push_back(Strut(spring, damping,
                                        (ref.x - next.x).norm(), &ref, &next));
        }
        
        // add diagonal ============================================
        if ( i + 1 < array_width && j + 1 < array_width ) {
          Particle& next = mesh_particles_[idx(i + 1, j + 1, array_width)];
          this->struts_.push_back(Strut(spring, damping,
                                        (ref.x - next.x).norm(), &ref, &next));
        }
        // =========================================================
        if ( j + 1 < array_width ) {
          Particle& next = mesh_particles_[idx(i, j + 1, array_width)];
          this->struts_.push_back(Strut(spring, damping,
                                        (ref.x - next.x).norm(), &ref, &next));
        }
        // add diagonal ============================================
        if ( i - 1 >= 0 && j + 1 < array_width ) {
          Particle& next = mesh_particles_[idx(i - 1, j + 1, array_width)];
          this->struts_.push_back(Strut(spring, damping,
                                        (ref.x - next.x).norm(), &ref, &next));
        }
        // =========================================================
        
      }
    }
  }
  
  StateVector& BouncingMesh::state_vector(){
    this->state_vector_.state.clear();
    for (int i = 0; i < this->mesh_particles().size(); ++i) {
      this->state_vector_.state.push_back(this->mesh_particles()[i].x);
    }
    for (int i = 0; i < this->mesh_particles().size(); ++i) {
      this->state_vector_.state.push_back(this->mesh_particles()[i].v);
    }
    
    return this->state_vector_;
  }
  
  void BouncingMesh::update_particles(physical_objects::StateVector &state){
    int N = (int) this->mesh_particles_.size();
    for (int i = 0; i < this->mesh_particles().size(); ++i ) {
      this->mesh_particles_[i].x.x = state.state[i].x;
      this->mesh_particles_[i].x.y = state.state[i].y;
      this->mesh_particles_[i].x.z = state.state[i].z;
      // updated location
      this->mesh_particles_[i].v.x = state.state[i + N].x;
      this->mesh_particles_[i].v.y = state.state[i + N].y;
      this->mesh_particles_[i].v.z = state.state[i + N].z;
    }
//    printf("Updated %d particles\n", this->mesh_particles().size());
  }
  
  const std::vector<Particle>& BouncingMesh::mesh_particles(){
    return this->mesh_particles_;
  }
  
  const std::vector<Strut> BouncingMesh::struts(){
    return this->struts_;
  }
  
  
  StateVector BouncingMesh::dynamic(StateVector X, float t){
    this->compute_force(X, t);
    int N = (int) this->mesh_particles_.size();
    StateVector Xp(X.size);
    for (int i = 0; i < N; ++i) {
//      Xp.state[i].x = X.state[i].x;
//      Xp.state[i].y = X.state[i].y;
//      Xp.state[i].z = X.state[i].z;
      Xp.state[i] = X.state[i + N];
      Xp.state[i + N] = this->mesh_particles_[i].f / this->mesh_particles_[i].m;
    }
//    Xp.print();
    return Xp;
  }
  
  StateVector NumericalIntegrator::RK4Integrate(BouncingMesh& sv, float t,
                                                 float deltaT){
    StateVector& x = sv.state_vector();
    
    StateVector K1 = sv.dynamic(x, t);
    StateVector K2 = sv.dynamic(x + (0.5 * deltaT) * K1, t + 0.5 * deltaT);
    StateVector K3 = sv.dynamic(x + 0.5 * deltaT * K2, t + 0.5 * deltaT);
    StateVector K4 = sv.dynamic(x + deltaT * K3, t + deltaT);
    
    StateVector res = x + 1.0/6.0 * deltaT * (K1 + 2 * K2 + 2 * K3 + K4);
    return res;
  }
  
  
}