#include "Particle.h"

#define _USE_MATH_DEFINES

FLOAT Particle::kernel_poly6(vector3D r, FLOAT h)
{
    FLOAT ans = 0;

    FLOAT r_abs_sqr = r * r;
    if( h * h >= r_abs_sqr ) 
        ans = 315*(h*h - r_abs_sqr)*(h*h - r_abs_sqr)*(h*h - r_abs_sqr) / ( 64*M_PI*pow(h,9) )  ;

    return ans;
}

vector3D Particle::kernal_poly6_gradient(vector3D r , FLOAT h )
{
    return r * -945.0/ ( 32.0 * M_PI * pow(h,9) ) * pow((h*h - r * r ), 2) ;
}

FLOAT Particle::kernal_poly6_laplacian(vector3D r , FLOAT h )
{
    FLOAT r_abs_sqr = r * r;
    return - 945.0 / ( 32.0 * M_PI * pow(h, 9) ) * ( r_abs_sqr - h * h ) * ( 4 * r_abs_sqr + 3 * (r_abs_sqr - h * h )); 
}

vector3D Particle::kernel_spiky_gradient(vector3D r, FLOAT h)
{
    vector3D ans;

    FLOAT r_abs = r.abs();
    if ( h >= r_abs )
        ans = r * -45 * (h - r_abs) * (h - r_abs) / (r_abs * M_PI * pow(h,6)) ;

    return ans;
}

FLOAT Particle::kernel_viscosity_laplacian(vector3D r, FLOAT h)
{
    FLOAT ans = 0;

    FLOAT r_abs = r.abs();
    if ( h >= r_abs)
        ans = 45 * (h - r_abs) / (M_PI * pow(h, 6));

    return ans;
}


Particle::Particle(const vector3D pos_):pos(pos_),v(vector3D())
{

}

Particle::~Particle()
{

}

void Particle::update(const parameter &m, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby)
{
}

FLOAT Particle::update_rho(const parameter &m, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby)
{
    FLOAT ans = 0;

    return ans;
}

vector3D Particle::get_F_pressure(const parameter &m) const
{
    vector3D ans;

    return ans;
}
vector3D Particle::get_F_viscosity(const parameter &m) const
{
    vector3D ans;

    return ans;
}
vector3D Particle::get_F_tension(const parameter &m) const
{
    vector3D ans;

    return ans;
}

FLOAT distance_sqr(const Particle &p1, const Particle &p2)
{
    vector3D d =  p1.pos - p2.pos;
    return d * d;
}