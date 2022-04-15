#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#include "matrix3d.h"
#include "parameter.h"

class Particle
{
public:

    Particle(const parameter &para, const vector3D pos_);
    ~Particle();

    vector3D pos;
    vector3D v; // velocity
    GRID grid;
    FLOAT rho;

    FLOAT update_rho(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby);
    void update(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby);

private:

    GRID update_grid(const parameter &para);

    vector3D get_F_pressure(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby) const;
    vector3D get_F_viscosity(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby) const;
    vector3D get_F_tension(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby) const;

    static FLOAT kernel_poly6(vector3D &&r, FLOAT h, FLOAT h2, FLOAT h6, FLOAT h9);
    static vector3D kernal_poly6_gradient(vector3D &&r , FLOAT h, FLOAT h2, FLOAT h6, FLOAT h9);
    static FLOAT kernal_poly6_laplacian(vector3D &&r , FLOAT h, FLOAT h2, FLOAT h6, FLOAT h9);
    static vector3D kernel_spiky_gradient(vector3D &&r, FLOAT h, FLOAT h2, FLOAT h6, FLOAT h9);
    static FLOAT kernel_viscosity_laplacian(vector3D &&r, FLOAT h, FLOAT h2, FLOAT h6, FLOAT h9);

};

FLOAT distance_sqr(const Particle &p1, const Particle &p2);

#endif