#include "Particle.h"

Particle::Particle(const vector3D pos_):v(vector3D()),pos(pos_)
{

}

Particle::~Particle()
{

}

void Particle::update(FLOAT dt, const Material &m, const std::vector<Particle> &particles, const std::vector<PARTICLE_NUMBER> &p_number_nearby)
{
}

FLOAT Particle::get_rho(const Material &m) const
{
    FLOAT ans;

    return ans;
}

vector3D Particle::get_F_pressure(const Material &m) const
{
    vector3D ans;

    return ans;
}
vector3D Particle::get_F_viscosity(const Material &m) const
{
    vector3D ans;

    return ans;
}
vector3D Particle::get_F_tension(const Material &m) const
{
    vector3D ans;

    return ans;
}

FLOAT distance_sqr(const Particle &p1, const Particle &p2)
{
    vector3D d =  p1.pos - p2.pos;
    return d * d;
}