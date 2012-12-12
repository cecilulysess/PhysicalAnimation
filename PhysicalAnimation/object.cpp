//
//  object.cpp
//  PhysicalAnimation
//
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//  
//  Author: cosmobserver@gmail.com (Julian Wu )
//    9/9/12.


#include "object.h"
//#include "definitions.h"
#include "Quaternion.h"

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
    float wstep = width / (subdivide + 1), hstep = height / (subdivide + 1);
    int vertice_no = (subdivide + 2) * (subdivide + 2);
    //int strut_no = (subdivide + 1) * (3 * subdivide + 5);
    int strut_no = (subdivide + 1) * (4 * subdivide + 6); // for all connection
    subdividion_no = subdivide;
    
    for(int i = 0 ; i < subdivide + 2; ++i) {
      for(int j = 0; j < subdivide + 2; ++j) {
        Struts_Vertice str(mass, // mass
                           Vector3d(0.0, 0.0, 0.0), // force
                           Vector3d((float)Center.x - width / 2.0 + (wstep * j),
                                    0.0,
                                    (float)Center.z - height / 2.0 + (hstep * i)
                                    ), //location
                           Vector3d(0.0, 0.0, 0.0) // velocity
                           
                           );
        if( i == 0 || j ==0 || i == subdivide + 1 || j == subdivide + 1) {
          printf("Set true\n");
          str.is_static_vertice = true;
        }
        vertices.push_back(str);
        

      }
      
    }
    
    for(int i = 0; i < vertices.size(); ++i ) {
      vertices[i].vertice_id = i;
    }
//    this->vertices[vertices.size()/2].velocity.y = 5;
    
    
    for(int i = 0; i < N; ++i){
      X.s[i] = vertices[i].location;
      X.s[i+N] = vertices[i].velocity;
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
        if ( i + 1 < subdivide + 2 && j + 1 < subdivide  + 2){
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
        //left down i + 1, j - 1
        if ( j - 1 >= 0 && i + 1  < subdivide + 2 ){
          Strut* strut = new Strut(spring, damper,
                                   (vertices[(i+1)* ( subdivide + 2 ) + j - 1 ].location - current_loc).norm(),
                                   curr_vtx, vertices[(i+1)* ( subdivide + 2 ) + j - 1 ]);
            struts.push_back(
                             *strut
                             );
            curr_vtx.add_connect_strut(*strut);
          }
    
      }
    }
//    printf("stru_S %d", struts.size());
  }
  
  
  void surface::calculate_a(StateVector X, float dt) {
    for(int i = 0; i < this->vertices.size(); ++i) {
      vertices[i].force = Vector3d(0.0, 0.0, 0.0);
    }
    for(int i = 0; i < this->struts.size(); ++i) {
      Struts_Vertice* a = this->struts[i].vertices_pair.first;
      Struts_Vertice* b = this->struts[i].vertices_pair.second;
      Vector3d L = (X.s[a->vertice_id] - X.s[b->vertice_id]);
      Vector3d uab =  L / L.norm();
      
      float delta_L = (L.norm() - this->struts[i].L0);
//            printf("d_L(%d, %d) :%f\n", a->vertice_id, b->vertice_id, delta_L);
      Vector3d fsab = - this->struts[i].K * delta_L * uab;
//            printf("%d: fsab: %f\n", i, fsab.norm());
      Vector3d fdab = this->struts[i].D * (
                                           //Vb-Va
               (X.s[b->vertice_id + N] - X.s[a->vertice_id + N]) * uab * uab
               );
//      printf("%d: fs: (%f,%f,%f), fd: (%f,%f,%f)\n", i, fsab.x, fsab.y, fsab.z,
//                         fdab.x, fdab.y, fdab.z);

      
      a->force = a->force + (fsab + fdab);
      b->force = b->force - (fsab + fdab);
    }
    for(int i = 0; i < this->vertices.size(); ++i ) {
      vertices[i].force = vertices[i].force + vertices[i].external_force;
      vertices[i].external_force = Vector3d(0,0,0);
      if (vertices[i].is_static_vertice) {
        vertices[i].force = Vector3d(0.0, 0.0, 0.0);
      }
    }
    
    for(int i = 0; i < this->vertices.size(); ++i) {
      vertices[i].accel = vertices[i].force / vertices[i].mass;
      if (vertices[i].is_static_vertice) {
        vertices[i].accel = Vector3d(0.0, 0.0, 0.0);
      }
    }
  }
  
  StateVector surface::calculate_dynamics( StateVector X, double dt ) {
    StateVector Xp;
    for(int i = 0; i < this->vertices.size(); ++i) {
      Xp.s[i] = X.s[i+N];
    }
    calculate_a(X, dt);
    for(int i = 0; i < N; ++i ) {
      Xp.s[i+N] = vertices[i].accel;
    }
    return Xp;
  }
  
//  
//  Vector3d f(Vector3d X, double t, double T, int i) {
//    Vector3d dxdt;
//    double ctx = obs2ctr.x, cty = obs2ctr.y, ctz = obs2ctr.z;
//    Vector3d ctr(ctx, cty, ctz);
//    Vector3d r(X.x - ctx, X.y - cty, X.z - ctz);//X - ctr;
//    if (r.norm() > 25 * obs2rad ) {
//      dxdt = - r;
//    } else {
//      if (r.norm() > 15 * obs2rad ) {
//        dxdt = - 1.2 * flockmass[i] * curr_X.s[i + N] * curr_X.s[i + N] * r / r.norm();
//  //      dxdt = - r * ((rand() % 10) / 10.0 );
//      } else {
//        if ( r.norm() < 8 * obs2rad) {
//          dxdt = 2 * r;
//        } else
//          dxdt = - flockmass[i] * curr_X.s[i + N] * curr_X.s[i + N] * r / r.norm() ;
//      }
//    }
//    for (int i = 0 ; i < N; ++i ) {
//      if ( (X - curr_X.s[i]).norm() < 3 * RECTSIZE ) {
//        Vector3d tmp (0.1, 0.1, 0.1);
//        tmp = tmp + X - curr_X.s[i];
//        tmp = (X - curr_X.s[i]) % tmp;
//        dxdt = dxdt + tmp;
//      }
//    }
  //  dxdt = - (curr_X.s[i + N] * r) * r ;
  //  dxdt = -r;
  //  dxdt.x = 0.01;
  //  dxdt.y = X.y + (rand() % 2) / 2000.0;
  //  dxdt.z = X.z + (rand() % 2) / 2000.0;
  //  double omega = 2 * PI / T;
  //
  //  dxdt.x = X.y;
  //  dxdt.y = -Sqr(omega) * X.x;
//    return dxdt;
//  }

//  StateVector F(StateVector X, double t) {
//    StateVector Xp;
//    for ( int i = 0 ; i < N ; ++i ) {
//      Xp.s[i] = X.s[i + N];
//    }
//    for ( int i = 0;  i < N ; ++i ) {
//      Xp.s[i + N] = 1 / flockmass[i] * f(X.s[i], t, dT, i);
//    }
//    return Xp;
//  }

  StateVector surface::NumInt ( StateVector X, StateVector Xp,
                               float t, float dt) {
    StateVector K1, K2, K3, K4;
    K1 = Xp * dt;
//    K1.print("K1");
    K2 = calculate_dynamics(X + K1 * 0.5, t + 0.5 * dt) * dt;
    K3 = calculate_dynamics(X + K2 * 0.5, t + 0.5 * dt) * dt;
    K4 = calculate_dynamics(X + K3, t + dt) * dt;
    StateVector res = X + (K1 + K2 * 2 + K3 * 2 + K4) * (1.0 / 6.0);
//    StateVector res = X + K1;
//    printf("RES0: %f\n", res.s[0].norm());
    print_surface();
    return res;
  }
  
  void surface::update_State(StateVector& Xnew){
    for(int i = 0; i < N; ++i){
      this->vertices[i].location = Xnew.s[i];
      this->vertices[i].velocity = Xnew.s[i+N];
    }
    this->X = Xnew;
    
  }
  
//
//  void mainloop(){
//    StateVector Xp = F(curr_X, curr_t);
//    StateVector Xnew = NumInt(curr_X, Xp, curr_t, dt);
//    // collision
//    curr_X = Xnew;
//    curr_t = curr_t + dt;
//  }

  void surface::print_surface() {
    printf("\nVertices:\n");
    for(int i = 0; i < N; ++i) {
      printf("\tV_%d:(%5.2f, %5.2f, %5.2f) v(%5.2f,%5.2f,%5.2f) Stat:%d\n", vertices[i].vertice_id, vertices[i].location.x,
             vertices[i].location.y, vertices[i].location.z,
             //velocity
             vertices[i].velocity.x, vertices[i].velocity.y,
             vertices[i].velocity.z, vertices[i].is_static_vertice);
      
    }
    printf("Struts:\n");
    for(int i = 0; i < this->struts.size(); ++i) {
      printf("\tSt(%d,%d)\n", struts[i].vertices_pair.first->vertice_id,
             struts[i].vertices_pair.second->vertice_id);
    }
    this->X.print("X");
  }

  void ModelObject::rotate(float degree, float x, float y, float z) {
    Quaternion qt(degree, x, y, z);
    for (int i = 0 ; i < vertices.size(); i += 3) {
      Vector3d v(vertices[i], vertices[i+1], vertices[i+2]);
      Quaternion qout = qt * Quaternion(v) * qt.inv();
      Vector3d vout = Vector3d(qout);
      vertices[i] = vout.x;
      vertices[i+1] = vout.y;
      vertices[i+2] = vout.z;
    }
    make_array();
  }
  
  
  void RigidBody::State_to_Array(RigidBody *rb, double *y) {
    *y++ = rb->x.x;
    *y++ = rb->x.y;
    *y++ = rb->x.z;
    
    *y++ = rb->q.q.x;
    *y++ = rb->q.q.y;
    *y++ = rb->q.q.z;
    *y++ = rb->q.q.w;
    
    *y++ = rb->P.x;
    *y++ = rb->P.y;
    *y++ = rb->P.z;
    
    *y++ = rb->L.x;
    *y++ = rb->L.y;
    *y++ = rb->L.z;
    
  }
  
  void RigidBody::Array_to_State(RigidBody *rb, double *y) {
    rb->x.x = *y++;
    rb->x.y = *y++;
    rb->x.z = *y++;
    
    //..rotation matrix
    
    rb->P.x = *y++;
    rb->P.y = *y++;
    rb->P.z = *y++;
    
    rb->L.x = *y++;
    rb->L.y = *y++;
    rb->L.z = *y++;
    
    rb->v = rb->P / rb->mass;
    rb->R = rb->q.normalize().rotation3x3();
    rb->Iinv = rb->R * rb->Ibody * rb->R.transpose();
    
    rb->omega = rb->Iinv * rb->L;
  }

  
  Matrix3x3 Star(Vector3d a){
    Matrix3x3 res(
                  0, -a.z, a.y,
                  a.z, 0, -a.x,
                  -a.y, a.x, 0
    );
    
    return res;
  }
  
  static Vector3d gravity(0, -0.98, 0);
  void Compute_Force_and_torque(double t, RigidBody *rb){
    rb->force = gravity;
    
  }
  
  // y*k
  double* array_time(double *y, int len, float k, double *result){
    
    for(int i = 0; i < len; ++i) {
      result[i] = y[i] * k;
    }
    return result;
  }
  
  // add b to a
  double * array_sum(double *a, double *b, int len, double *result) {
    for(int i = 0; i < len; ++i ) {
      result[i] = a[i] + b[i];
    }
    return result;
  }
  
  void copy_state(ModelObject* obj, double *res, int len){
    RigidBody::Array_to_State(&obj->rbody, res);
  }
  
  void ode(double y0[], double yend[], int len, double t0,
           double t1, dydt_func dydt,
           ModelObject* obj) {
    double* X = (double*)malloc(sizeof(double) * STATE_SIZE);
    double* K1 = (double*)malloc(sizeof(double) * STATE_SIZE);
    double* K2 = (double*)malloc(sizeof(double) * STATE_SIZE);
    double* K3 = (double*)malloc(sizeof(double) * STATE_SIZE);
    double* K4 = (double*)malloc(sizeof(double) * STATE_SIZE);
    double* tmp = (double*)malloc(sizeof(double) * STATE_SIZE);
    copy_state(obj, X, STATE_SIZE);
    
    //Get K1
    dydt(t1 - t0, obj, K1);
    
    //Get K2
    array_time(K1, STATE_SIZE, 0.5, tmp);
    array_sum(tmp, X, STATE_SIZE, tmp);
    dydt( (t1-t0) * 0.5 + t0, obj, K2);
    array_time(K2, STATE_SIZE, (t1-t0), K2);
    
    //Get K3
    array_time(K2, STATE_SIZE, 0.5, tmp);
    array_sum(tmp, X, STATE_SIZE, tmp);
    dydt( (t1-t0) * 0.5 + t0, obj, K3);
    array_time(K3, STATE_SIZE, (t1-t0), K3);
    
    // get K4
    array_sum(K3, X, STATE_SIZE, tmp);
    dydt( t1, obj, K4 );
    array_time(K4, STATE_SIZE, (t1-t0),K4);
    
    array_time(K2, STATE_SIZE, 2, K2);
    array_time(K3, STATE_SIZE, 3, K3);
    array_sum(K1, K2, STATE_SIZE, tmp);
    array_sum(tmp, K3, STATE_SIZE, tmp);
    array_sum(tmp, K4, STATE_SIZE, tmp);
    array_time(tmp, STATE_SIZE, 1.0/6, tmp);
    array_sum(X, tmp, STATE_SIZE, yend);
    
    
  }
  
  void dydt(double t, ModelObject* obj, double ydot[]) {
    RigidBody::Array_to_State(&obj->rbody, obj->body_array);
    for(int i = 0 ; i < NBODY; ++i) {
      Compute_Force_and_torque(t, &obj->rbody);
      ddt_State_to_Array(&obj->rbody, obj->body_array_dot);
    }
  }
    
  void ddt_State_to_Array(RigidBody *rb, double *ydot){
    *ydot++ = rb->v.x;
    *ydot++ = rb->v.y;
    *ydot++ = rb->v.z;
    
    //    Matrix3x3 Rdot = Star(rb->omega) * rb->R;
    //
    //    for(int i = 0 ; i < 3; ++i ) {
    //      *ydot++ = Rdot.row[i].x;
    //      *ydot++ = Rdot.row[i].y;
    //      *ydot++ = Rdot.row[i].z;
    //
    //    }
    Quaternion qdot = .5 * (rb->omega * rb->q);
    *ydot++ = qdot.q.x;
    *ydot++ = qdot.q.y;
    *ydot++ = qdot.q.z;
    *ydot++ = qdot.q.w;
    
    // dP(t)/dt = F(t)
    *ydot++ = rb->force.x;
    *ydot++ = rb->force.y;
    *ydot++ = rb->force.z;
    // dL(t)/dt = t(t);
    *ydot++ = rb->torque.x;
    *ydot++ = rb->torque.y;
    *ydot++ = rb->torque.z;
  }
  
//  void InitState(ModelObject* obj, double *y0, double *yfinal) {
//    
//  }
//  
//  void RunSimulation(ModelObject* obj){
//    double y0[STATE_SIZE * NBODY];
//    double yfinal[STATE_SIZE * NBODY];
//    
  
    
//    init
//  }
} //ns