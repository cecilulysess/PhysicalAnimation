//
//  physical_object.cpp
//  PhysicalAnimation
//
//  Created by Julian Wu on 11/25/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#include <set>
#include <assert.h>

#include "Utility.h"
#include "physical_object.h"

#define ISDIAGONAL 

namespace physical_objects {
  
  //============================State Vector============================
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
  //============================State Vector============================
  
  
  //============================BouncingMesh============================
  void BouncingMesh::compute_force(StateVector& X, float t){
    Vector3d g(0, -9.8, 0);
    clear_force();
    float mass = 0.0;
    // apply unary force to each particle with state X
    for (int i = 0; i < this->mesh_particles_.size(); ++i) {
      if ( this->mesh_particles_[i].is_pivot ) {
        this->mesh_particles_[i].f = Vector3d(0, 0, 0);
      } else {
        mass = mesh_particles_[i].m;
        this->mesh_particles_[i].f =  g * mass;
      }
    }
    for (int i = 0; i < this->temporary_vertice_size; ++i) {
      // this->temporary_particles[i]->p->v.print(); printf("\n");
      if ( this->temporary_particles[i]->isDetached) {
        // printf("I have the gravity\n");
        this->temporary_particles[i]->p->f = g * this->temporary_particles[i]->p->m;
        
      }
    }
    // apply nary force 
    for (int i = 0; i < this->struts().size(); ++i) {
      Particle* a = this->struts_[i].vertice_pair.first;
      Particle* b = this->struts_[i].vertice_pair.second;
      Vector3d L = a->x - b->x;
      Vector3d uab = L / L.norm();
      float delta_L = (L.norm() - this->struts_[i].L0);
      //       printf("d_L(%d, %d) :%f\n", a->vertice_id, b->vertice_id, delta_L);
      Vector3d fsab = - this->struts_[i].K * delta_L * uab;
      //            printf("%d: fsab: %f\n", i, fsab.norm());
      Vector3d fdab = this->struts_[i].D * (
                                            //Vb-Va
                                            (b->v - a->v) * uab * uab
                                            );
//    printf("%d: fs: (%f,%f,%f), fd: (%f,%f,%f)\n", i, fsab.x, fsab.y, fsab.z,
//               fdab.x, fdab.y, fdab.z);
      
      if (!a->is_pivot)
        a->f = a->f + (fsab + fdab);
      if (!b->is_pivot)
        b->f = b->f - (fsab + fdab);
    }
    
    // apply temporary vertices force
    for (int i = 0 ; i < this->temporary_particles.size(); ++ i) {
//      printf("Detached? %d\n", temporary_particles[i]->isDetached);
      if (temporary_particles[i]->isDetached) {
        // printf("I don't bouncing\n");
        continue;
      }
      for(int j = 0; j < 3; ++j) {
        Strut* st = this->temporary_particles[i]->struts[j];
        Particle* a = st->vertice_pair.first;
        Particle* b = st->vertice_pair.second;
        Vector3d L = a->x - b->x;
//        L.print();printf("L\t");
        Vector3d uab = L / L.norm();
        float delta_L = (L.norm() - st->L0);
        Vector3d fsab = - st->K * delta_L * uab;
        Vector3d fdab = st->D * (
                                              //Vb-Va
                                              (b->v - a->v) * uab * uab
                                              );
        
        if (!a->is_pivot)
          a->f = a->f + (fsab + fdab);
        if (!b->is_pivot)
          b->f = b->f - (fsab + fdab);
      }
       Particle* tp = this->temporary_particles[i]->p;
       printf("\t\tF_TP = (%f, %f, %f), V_TP = (%f, %f, %f)\n", tp->f.x,
              tp->f.y,tp->f.z, tp->v.x, tp->v.y, tp->v.z);
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
//    //
//    this->push(0, 0, Vector3d(0,0,0));
    
    
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
    create_faces();
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
    for (int i = 0; i < this->temporary_particles.size(); ++i) {
      this->temporary_particles[i]->p->f = Vector3d(0, 0, 0);
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
#ifdef ISDIAGONAL
        // add diagonal ============================================
        if ( i + 1 < array_width - 1 && j + 1 < array_width - 1) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i + 1, j + 1, array_width)]);
        }
        //==========================================================
#endif
        if ( j + 1 < array_width - 1) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i, j + 1, array_width)]);
        }
#ifdef ISDIAGONAL
        // add diagonal ============================================
        if ( i - 1 > 0 && j + 1 < array_width - 1 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i - 1, j + 1, array_width)]);
        }
#endif
        // =========================================================
        if ( i - 1 > 0) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i - 1, j, array_width)]);
        }
#ifdef ISDIAGONAL
        // add diagonal ============================================
        if ( i - 1 > 0 && j - 1 > 0 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i - 1, j - 1, array_width)]);
        }
        // =========================================================
#endif
        if ( j - 1 > 0 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i, j - 1, array_width)]);
        }
#ifdef ISDIAGONAL
        // add diagonal ============================================
        if ( i + 1 < array_width - 1 && j - 1 > 0 ) {
          mesh_particles_[idx(i, j, array_width)].N++;
          ref.push_back(&mesh_particles_[idx(i + 1, j - 1, array_width)]);
        }
        // =========================================================
#endif
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
#ifdef ISDIAGONAL
        // add diagonal ============================================
        if ( i + 1 < array_width && j + 1 < array_width ) {
          Particle& next = mesh_particles_[idx(i + 1, j + 1, array_width)];
          this->struts_.push_back(Strut(spring, damping,
                                        (ref.x - next.x).norm(), &ref, &next));
        }
        // =========================================================
#endif
        if ( j + 1 < array_width ) {
          Particle& next = mesh_particles_[idx(i, j + 1, array_width)];
          this->struts_.push_back(Strut(spring, damping,
                                        (ref.x - next.x).norm(), &ref, &next));
        }
#ifdef ISDIAGONAL
        // add diagonal ============================================
        if ( i - 1 >= 0 && j + 1 < array_width ) {
          Particle& next = mesh_particles_[idx(i - 1, j + 1, array_width)];
          this->struts_.push_back(Strut(spring, damping,
                                        (ref.x - next.x).norm(), &ref, &next));
        }
        // =========================================================
#endif
        
      }
    }
  }
  
  StateVector& BouncingMesh::state_vector(){
    this->temporary_particles.clear();
    temporary_vertice_size = 0;
    this->state_vector_.state.clear();
    
    // add mesh particles
    for (int i = 0; i < this->mesh_particles().size(); ++i) {
      this->state_vector_.state.push_back(this->mesh_particles()[i].x);
    }
    
    //=========Add temporary particles=============
    for (int i = 0; i < this->faces_.size(); ++i) {
      for (int j = 0; j < this->faces_[i].temporary_vertices.size(); ++j ) {
        this->state_vector_.state.push_back(
          this->faces_[i].temporary_vertices[j].p->x);
        temporary_vertice_size ++;
        this->temporary_particles.push_back(
                    &(this->faces_[i].temporary_vertices[j])
                                            );
      }
    }
    //=============================================
  
    for (int i = 0; i < this->mesh_particles().size(); ++i) {
      this->state_vector_.state.push_back(this->mesh_particles()[i].v);
    }
    
    //=========Add temporary particles=============
    for (int i = 0; i < this->faces_.size(); ++i) {
      for (int j = 0; j < this->faces_[i].temporary_vertices.size(); ++j ) {
        this->state_vector_.state.push_back(
          this->faces_[i].temporary_vertices[j].p->v);
      }
    }
    //=============================================

    this->state_vector_.updateSize();
    return this->state_vector_;
  }
  
  void BouncingMesh::update_particles(physical_objects::StateVector &state){
    int N = (int) this->mesh_particles_.size() + temporary_vertice_size;
    for (int i = 0; i < this->mesh_particles().size(); ++i ) {
      this->mesh_particles_[i].x.x = state.state[i].x;
      this->mesh_particles_[i].x.y = state.state[i].y;
      this->mesh_particles_[i].x.z = state.state[i].z;
      // updated location
      this->mesh_particles_[i].v.x = state.state[i + N].x;
      this->mesh_particles_[i].v.y = state.state[i + N].y;
      this->mesh_particles_[i].v.z = state.state[i + N].z;
    }
    // update temp vertices
    int offset = (int) this->mesh_particles_.size();
    
    for (int i = 0; i < this->temporary_vertice_size; ++i) {
      
      this->temporary_particles[i]->p->x.x = state.state[offset + i].x;
      this->temporary_particles[i]->p->x.y = state.state[offset + i].y;
      this->temporary_particles[i]->p->x.z = state.state[offset + i].z;
//      state.state[offset + i + N].print();
      this->temporary_particles[i]->p->v.x =
        state.state[offset + i + N].x;
      this->temporary_particles[i]->p->v.y =
        state.state[offset + i + N].y;
      this->temporary_particles[i]->p->v.z =
        state.state[offset + i + N].z;
    }
    update_faces();
    
//    printf("Updated %d particles\n", this->mesh_particles().size());
  }
  
  void BouncingMesh::update_faces(){
    for (int i = 0; i < this->faces_.size(); ++i) {
      faces_[i].updateFace();
    }
    
  }
  
  
  
  const std::vector<Face> BouncingMesh::faces(){
    return this->faces_;
  }
  
  const std::vector<Particle>& BouncingMesh::mesh_particles(){
    return this->mesh_particles_;
  }
  
  const std::vector<Strut> BouncingMesh::struts(){
    return this->struts_;
  }
  
  StateVector BouncingMesh::dynamic(StateVector X, float t){
    this->compute_force(X, t);
    int N = (int) this->mesh_particles_.size() + temporary_vertice_size;
//    printf("N:%d, X:%d\n", N, X.size);
    assert( 2 * N == X.size);
    StateVector Xp(X.size);
    // from 0 to offset is the original particles
    int offset = (int) this->mesh_particles_.size();
    for (int i = 0; i < offset; ++i) {
      Xp.state[i] = X.state[i + N];
      Xp.state[i + N] =
        this->mesh_particles_[i].f / this->mesh_particles_[i].m;
    }
    
    for (int i = 0; i < this->temporary_vertice_size; ++i) {
      Xp.state[offset + i] = X.state[offset + i + N];
      Xp.state[offset + i + N] =
        this->temporary_particles[i]->p->f / this->temporary_particles[i]->p->m;
    }
    return Xp;
  }

  
  ParticleStrutPair* BouncingMesh::add_temp_spring(Vector3d location,
                                     Vector3d velocity,
                                     float spring, float damping,
                                     float mass){
    int faceIdx = 65;
    this->faces_[faceIdx].add_tmp_vertices(location, velocity, mass, spring, damping);
    return &this->faces_[faceIdx].
          temporary_vertices[this->faces_[faceIdx].temporary_vertices.size() - 1];
  }
  //============================BouncingMesh============================
  
  
  //=================================Strut==============================  
  ParticleStrutPair::ParticleStrutPair(Particle *p,
                                       Strut* a, Strut* b, Strut* c){
    
    this->p = p;
    this->struts[0] = a;
    this->struts[1] = b;
    this->struts[2] = c;
    this->isDetached = false;
  }
  
//  ParticleStrutPair::~ParticleStrutPair(){
////    for (int i = 0; i < 3; ++i ) {
////      delete struts[i];
////    }
//    delete[] struts;
//  }
  
  
  void BouncingMesh::create_faces(){
//    int N = (this->array_width - 1) * (this->array_width - 1);
    this->faces_.clear();
    for (int i = 0; i < this->array_width - 1; ++i) { // no face in last row
      for (int j = 0; j < this->array_width - 1; ++j) { // no face in last col
                                                        // update the vertice for first face
        Particle *a = &this->mesh_particles_[idx(i, j, array_width)];
        Particle *b = &this->mesh_particles_[idx(i + 1, j, array_width)];
        Particle *c = &this->mesh_particles_[idx(i + 1, j + 1, array_width)];
        Particle *d = &this->mesh_particles_[idx(i, j + 1, array_width)];
        this->faces_.push_back(Face(a, b, c));
        this->faces_.push_back(Face(a, c, d));
      }
    }
  }
  
  //=================================Strut==============================
  
  
  //=================================Face===============================
  Face::Face(Particle *a, Particle *b, Particle *c){
    this->a = a;
    this->b = b;
    this->c = c;
    if (a != NULL && b != NULL && c != NULL){
      this->normal = ((b->x - a->x) % (c->x - a->x)).normalize();
    }
  }
  
  bool Face::isAboveface(Particle* p){
//    p->x.print();
//    this->normal.print();
////    this->normal.normalize().print();
//    printf("Above face %f \n",(p->x * this->normal));
//    double res = (p->x * this->normal);
//    if ( res < 0.0 && p->x.y > 0)
//       res = (p->x * this->normal);
    return ((p->x - this->a->x) * this->normal) > 0.0;
  }
  
  void Face::updateFace(){
    this->normal = ((b->x - a->x) % (c->x - a->x)).normalize();
//    this->normal.print();printf("\n");
    for (int i = 0; i < this->temporary_vertices.size(); ++i) {
      if ( isAboveface(this->temporary_vertices[i].p) ) {
//        printf("I detached\n");
        this->temporary_vertices[i].isDetached = true;
      } else {
//        printf("I attached\n");
        this->temporary_vertices[i].isDetached = false;
      }
    }

  }
  
  ParticleStrutPair* Face::add_tmp_vertices(Vector3d& loc,
                                            Vector3d& velocity,
                                            float mass,
                                            float spring,
                                            float damping){
    Particle* p = new Particle(loc.x, loc.y, loc.z,
                               velocity.x, velocity.y, velocity.z,
                               mass, false);
//    float L0 = ??;
    Strut* a = new Strut(spring, damping,
                         (p->x - this->a->x).norm(), p, this->a);
    Strut* b = new Strut(spring, damping,
                         (p->x - this->b->x).norm(), p, this->b);
    Strut* c = new Strut(spring, damping,
                         (p->x - this->c->x).norm(), p, this->c);
    ParticleStrutPair* pair = new ParticleStrutPair(p, a, b, c);
    this->temporary_vertices.push_back(*pair);
    return pair;
  }
  
  //=================================Face===============================
  
    
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