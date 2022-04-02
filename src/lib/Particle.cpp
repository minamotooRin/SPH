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

Particle::Particle(const parameter &para, const vector3D pos_):pos(pos_),v(vector3D())
{
    update_grid(para);
}

Particle::~Particle()
{

}

void Particle::update(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby)
{ 
    // accelerate
    vector3D g;
    g[DIM - 1] = -para.g;
    
    vector3D F = get_F_pressure(para, particles , p_number_nearby)
        + get_F_viscosity(para, particles , p_number_nearby)
        + get_F_tension(para, particles , p_number_nearby) 
        + (g * rho) ;
    
    vector3D a  = F / rho ;
    
    // move
    pos         = pos + v * para.dt + a * para.dt * para.dt / 2; 
    v           = v + a * para.dt ; 

    // bouce back
    for(auto i = 0 ; i < DIM ; ++i)
    {
        if ( pos[i] >= para.volume[i] )
        {
            v[i] = - v[i] ;
            pos[i] = 2 * para.volume[i] - pos[i];
        } 
        if ( pos[i] <= 0 )
        {
            v[i] = - v[i] ;
            pos[i] = - pos[i];
        } 
    }

    update_grid(para);
}

FLOAT Particle::update_rho(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby)
{
    rho = 0;

    for(PARTICLE_NUMBER p : p_number_nearby)
    {
        rho += para.m * kernel_poly6(pos - particles[p].pos , para.h );
    }

    return rho;
}

GRID Particle::update_grid(const parameter &para)
{   
    grid = GRID(
        static_cast<UINT32>(floor(pos[0] / para.h)), 
        static_cast<UINT32>(floor(pos[1] / para.h)), 
        static_cast<UINT32>(floor(pos[2] / para.h))
    );
    return grid;
}

vector3D Particle::get_F_pressure(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby) const
{
    vector3D ans;

    for (PARTICLE_NUMBER p : p_number_nearby)
    {
        const Particle &pa = particles[p];
        ans += kernel_spiky_gradient( pos - pa.pos , para.h ) 
            * para.m * (para.k * (pa.rho - para.rho0) + para.k*(rho - para.rho0)) / (2*pa.rho) ; 
    }

    ans = ans * -1 ; 

    return ans;
}
vector3D Particle::get_F_viscosity(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby) const
{
    vector3D ans;

    for (PARTICLE_NUMBER p : p_number_nearby)
    {
        const Particle &pa = particles[p];
        ans += ((pa.v - v) / pa.rho) * kernel_viscosity_laplacian(pos - pa.pos, para.h);
    }

    ans *= para.mu * para.m;

    return ans;
}
vector3D Particle::get_F_tension(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby) const
{
    vector3D cs_grad; 
    FLOAT cs_lap = 0;

    for (PARTICLE_NUMBER p : p_number_nearby)
    {
        const Particle & pa = particles[p];
        cs_grad += kernal_poly6_gradient( pos - pa.pos , para.h ) * para.m / pa.rho ;
        cs_lap  += kernal_poly6_laplacian( pos - pa.pos , para.h ) * para.m / pa.rho ; 
    }

    return (cs_grad / cs_grad.abs()) * ( -para.sigma ) * cs_lap ;
}

FLOAT distance_sqr(const Particle &p1, const Particle &p2)
{
    vector3D d =  p1.pos - p2.pos;
    return d * d;
}
