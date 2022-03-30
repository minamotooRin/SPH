#include "SPH_handle.h"

#define _USE_MATH_DEFINES

FLOAT SPH_handle::kernel_poly6(vector3D r, FLOAT h)
{
    FLOAT ans = 0;

    FLOAT r_abs = r.abs();
    if( h >= r_abs ) 
        ans = 315*(h*h - r_abs*r_abs)*(h*h - r_abs*r_abs)*(h*h - r_abs*r_abs) / ( 64*M_PI*pow(h,9) )  ;

    return ans;
}

vector3D SPH_handle::kernel_spiky(vector3D r, FLOAT h)
{
    vector3D ans();

    FLOAT r_abs = r.abs();
    if ( h >= r_abs )
        ans = r * (-45*(h - r_abs)*(h - r_abs) / (r_abs * M_PI * pow(h,6))) ;

    return ans;
}

FLOAT SPH_handle::kernel_viscosity(FLOAT r, FLOAT h)
{
    FLOAT ans = 0;

    if ( h >= r.abs())
        ans = 45 * (h - r.abs()) / (M_PI * pow(h, 6));

    return ans;
}

SPH_handle::SPH_handle(std::string input, std::string output)
{
    isReady = true;

    ifile = input;
    ofile = output;

    para = parameter(input);
    if(!para.isReady)
    {
        isReady = false;
    }

    ofs = ofstream(ofile);
    if(!ofs.good())
    {
        isReady = false;
    }

    step_cnt        = 0 ; 
    
    srand(time(0));
    for(int it = 0; it < para.number; it++)
    {
        FLOAT x_pos = para.volume[0]/3 + static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(para.volume[0]/3)));
        FLOAT y_pos = para.volume[1]/3 + static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(para.volume[1]/3)));
        FLOAT z_pos = para.volume[2]/3 + static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(para.volume[2]/3)));
        particles.push_back( Particle( vector3D(x_pos, y_pos, z_pos) ) );
    }
}

SPH_handle::~SPH_handle()
{

}

std::set<PARTICLE_NUMBER> SPH_handle::get_nearby_paticles(Particle &p) const
{
    std::set<PARTICLE_NUMBER> ans;

    static GRID dgs[7] = {
        GRID(0,0,0), 
        GRID(1,0,0), 
        GRID(0,1,0), 
        GRID(0,0,1),
        GRID(-1,0,0), 
        GRID(0,-1,0),
        GRID(0,0,-1)
        };
    for( GRID & dg : dgs)
    {
        GRID g = p.grid + g;
        for(PARTICLE_NUMBER it : grid_2_particles[g] )
        {
            if(distance_sqr(particles[it], p) < para.h_squre)
            {
                ans.insert(it);
            }
        }
    }

    return ans;
}


void SPH_handle::run()
{
    run(para.step);
}

void SPH_handle::run(UINT32 step)
{
    if(!isReady)
    {
        return ;
    }

    step_cnt += step;

    for(UINT32 s = 0 ; s < step; s ++)
    {
        // update rho
        for(PARTICLE_NUMBER it = 0 ; it < particles.size(); it ++)
        {
            Particle &p = particles[it];

            const std::vector<PARTICLE_NUMBER> particles_numbers = get_nearby_paticles(p); // move construct

            grid_2_particles[p.grid].erase(it);
            p.update(dt, matrial, particles, particles_numbers);
            grid_2_particles[p.grid].insert(it);
        }

        for(PARTICLE_NUMBER it = 0 ; it < particles.size(); it ++)
        {
            Particle &p = particles[it];

            const std::vector<PARTICLE_NUMBER> particles_numbers = get_nearby_paticles(p); // move construct

            grid_2_particles[p.grid].erase(it);
            p.update(dt, matrial, particles, particles_numbers);
            grid_2_particles[p.grid].insert(it);
        }

        for(Particle &p : particles)
        {
            ofs.write(reinterpret_cast<const char*>(p.pos.getData()), sizeof(FLOAT) * DIM);
        }
    }
}

GRID SPH_handle::get_grid(const vector3D &pos) const
{   
    return GRID(
        static_cast<UINT32>(floor(pos[0] / para.h)), 
        static_cast<UINT32>(floor(pos[1] / para.h)), 
        static_cast<UINT32>(floor(pos[2] / para.h))
    );
}