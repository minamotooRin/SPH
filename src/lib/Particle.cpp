#include "Particle.h"

#define _USE_MATH_DEFINES

FLOAT Particle::kernel_poly6(vector3D &&r, FLOAT h2, FLOAT h9)
{
    FLOAT ans = 0;
    FLOAT r_abs_sqr = r * r;
    if(likely( h2 > r_abs_sqr ) )
        ans = 315*(h2 - r_abs_sqr)*(h2 - r_abs_sqr)*(h2 - r_abs_sqr) / ( 64*M_PI* h9 )  ;
    return ans;
}

vector3D Particle::kernal_poly6_gradient(vector3D &&r, FLOAT h2, FLOAT h9)
{
    vector3D ans;
    FLOAT r_abs_sqr = r * r;
    if(likely(h2 >  r_abs_sqr))
        ans = r * - 945.0/ ( 32.0 * M_PI * h9 ) * (h2 - r_abs_sqr) * (h2 - r_abs_sqr) ;
    return ans;
}

FLOAT Particle::kernal_poly6_laplacian(vector3D &&r, FLOAT h2, FLOAT h9)
{
    FLOAT ans = 0;
    FLOAT r_abs_sqr = r * r;
    if(likely(h2 > r_abs_sqr))
        ans =  945.0 / ( 32.0 * M_PI * h9 ) * (h2 - r_abs_sqr) * ( 7 * r_abs_sqr - 3 * h2 ); 
    return ans;
}

vector3D Particle::kernel_spiky_gradient(vector3D &&r, FLOAT h, FLOAT h6)
{
    vector3D ans;
    FLOAT r_abs = r.abs();
    if (likely( r_abs > ZERO && h > r_abs ))
        ans = r * -45 * (h - r_abs) * (h - r_abs) / (r_abs * M_PI * h6) ;
    return ans;
}

FLOAT Particle::kernel_viscosity_laplacian(vector3D &&r, FLOAT h, FLOAT h6)
{
    FLOAT ans = 0;
    FLOAT r_abs = r.abs();
    if (likely( h >= r_abs))
        ans = 45 * (h - r_abs) / (M_PI * h6);
    return ans;
}

Particle::Particle(const parameter &para, const vector3D pos_):pos(pos_)
{
    FLOAT vv[DIM] = {para.g * para.dt * 10};
    v = vector3D(vv);
    update_grid(para);
}

Particle::~Particle()
{

}

void Particle::update(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby)
{ 
    // accelerate
    static vector3D g = [&]() -> vector3D{
        vector3D g; 
        g[DIM - 1] = -para.g;
        return g;
    }();

    vector3D F_pressure = get_F_pressure(para, particles , p_number_nearby);
    vector3D F_viscosity = get_F_viscosity(para, particles , p_number_nearby);
    vector3D F_tension = get_F_tension(para, particles , p_number_nearby);
    // vector3D F_G = g * rho;

    // std::cout << F_pressure.abs() << "\t" << F_viscosity.abs() << "\t" << F_tension.abs() << "\t" << F_G.abs() << std::endl;
    
    vector3D a  = (F_pressure + F_viscosity + F_tension) / rho + g;
    
    // move
    pos         = pos + v * para.dt + a * para.dt2 / 2; 
    v           = v + a * para.dt ; 

    // bouce back
    for(auto i = 0 ; i < DIM ; ++i)
    {
        pos[i] = fmod( pos[i], 2 * para.volume[i]);
        if (pos[i] < ZERO )
        {
            pos[i] += 2 * para.volume[i];
        } 
        if (pos[i] > para.volume[i] )
        {
            v[i] *= -1;
            pos[i] = 2 * para.volume[i] - pos[i];
        } 
    }

    update_grid(para);
}

FLOAT Particle::update_rho(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby)
{
    rho = 0;

    for(PARTICLE_NUMBER p : p_number_nearby)
    {
        rho += kernel_poly6(pos - particles[p].pos ,para.h2, para.h9);
    }

    rho *= para.m;

    return rho;
}

GRID Particle::update_grid(const parameter &para)
{   
    for(int i = 0; i < DIM; ++i)
    {
        grid[i] = static_cast<UINT32>(floor(pos[i] / para.h));
    }
    return grid;
}

vector3D Particle::get_F_pressure(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby) const
{
    vector3D ans;

    for (PARTICLE_NUMBER p : p_number_nearby)
    {
        const Particle &pa = particles[p];
        ans += kernel_spiky_gradient( pos - pa.pos , para.h, para.h6) * (pa.rho + rho - para.rho0_2) / (2*pa.rho) ; 
    }

    ans *= - para.k_m;

    return ans;
}
vector3D Particle::get_F_viscosity(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby) const
{
    vector3D ans;

    for (PARTICLE_NUMBER p : p_number_nearby)
    {
        const Particle &pa = particles[p];
        ans += ((pa.v - v) / pa.rho) * kernel_viscosity_laplacian(pos - pa.pos, para.h, para.h6);
    }

    ans *= para.mu_m;

    return ans;
}
vector3D Particle::get_F_tension(const parameter &para, const std::vector<Particle> &particles, const std::set<PARTICLE_NUMBER> &p_number_nearby) const
{
    vector3D ans;

    vector3D cs_grad; 
    FLOAT cs_lap = 0;

    for (PARTICLE_NUMBER p : p_number_nearby)
    {
        const Particle & pa = particles[p];
        cs_grad += kernal_poly6_gradient( pos - pa.pos, para.h2, para.h9 ) / pa.rho ;
        cs_lap  += kernal_poly6_laplacian( pos - pa.pos, para.h2, para.h9 ) / pa.rho ; 
    }

    cs_grad *= para.m;
    cs_lap *= para.m;

    if (likely(cs_grad.abs() > ZERO)) ans = (cs_grad / cs_grad.abs()) * ( -para.sigma ) * cs_lap ;

    return ans;
}

FLOAT distance_sqr(const Particle &p1, const Particle &p2)
{
    vector3D d =  p1.pos - p2.pos;
    return d * d;
}
