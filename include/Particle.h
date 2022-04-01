#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#include "matrix3d.h"
#include "parameter.h"

class Particle
{
public:

    Particle(const vector3D pos_);
    ~Particle();

    vector3D pos;
    vector3D v; // velocity
    GRID grid;
    FLOAT rho;

    FLOAT update_rho(const parameter &m, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby);
    void update(const parameter &m, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby);

private:

    vector3D get_F_pressure(const parameter &m) const;
    vector3D get_F_viscosity(const parameter &m) const;
    vector3D get_F_tension(const parameter &m) const;

    static FLOAT kernel_poly6(vector3D r, FLOAT h);
    static vector3D kernal_poly6_gradient(vector3D r , FLOAT h );
    static FLOAT kernal_poly6_laplacian(vector3D r , FLOAT h );
    static vector3D kernel_spiky_gradient(vector3D r, FLOAT h);
    static FLOAT kernel_viscosity_laplacian(vector3D r, FLOAT h);

};

FLOAT distance_sqr(const Particle &p1, const Particle &p2);

#endif