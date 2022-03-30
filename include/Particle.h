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

    void update(FLOAT dt, const Material &m, const std::vector<Particle> &particles, const std::vector<PARTICLE_NUMBER> &p_number_nearby);

private:

    inline GRID get_grid(const Material &m) const;
    FLOAT get_rho(const Material &m) const;
    vector3D get_F_pressure(const Material &m) const;
    vector3D get_F_viscosity(const Material &m) const;
    vector3D get_F_tension(const Material &m) const;

};

FLOAT distance_sqr(const Particle &p1, const Particle &p2);

#endif