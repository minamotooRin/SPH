#include "Particle.h"

Particle::Particle(const vector3D pos_, const Material &m):v(vector3D()),pos(pos_),grid(get_grid(m))
{

}

Particle::~Particle()
{

}

void Particle::update(FLOAT dt, const Material &m, const std::vector<Particle> &particles, const std::vector<PARTICLE_NUMBER> &p_number_nearby)
{
    grid = get_grid(m);
    

}

inline GRID Particle::get_grid(const Material &m) const
{
    return (pos / m.h).getfloor();
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

FLOAT distance_sqr(Particle &p1, Particle &p2)
{
    vector3D d =  p1.pos - p2.pos;
    return d * d;
}