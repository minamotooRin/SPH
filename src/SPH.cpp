#include "SPH.h"

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

SPH_handle::SPH_handle(FLOAT x, FLOAT y, FLOAT z, UINT32 N, std::string m)
{
    ParitcleNumber  = N ;
    volume          = vector3D(x, y, z) ; 

    step_cnt        = 0 ; 
    dt              = 0.001 ; 

    if(gMaterialInfo.find(m) == gMaterialInfo.end())
    {
        matrial     = Material();
    }
    else
    {
        matrial     = gMaterialInfo[m];
    }

    fp              = nullptr;
    
    srand(time(0));
    for(int it = 0; it < N; it++)
    {
        FLOAT x_pos = x/3 + static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(x/3)));
        FLOAT y_pos = y/3 + static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(y/3)));
        FLOAT z_pos = z/3 + static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(z/3)));
        particles.push_back( Particle( vector3D(x_pos, y_pos, z_pos), matrial ) );
    }
}

SPH_handle::~SPH_handle()
{
    if(fp)
    {
        // How to read UINT32?
        fwrite(&ParitcleNumber, sizeof(UINT32), 1, fp);
        fwrite(&step_cnt, sizeof(UINT32), 1, fp);
        fclose(fp);
    }
}

std::vector<PARTICLE_NUMBER> SPH_handle::get_nearby_paticles(Particle &p) const
{
    std::vector<PARTICLE_NUMBER> ans;

    GRID dgs[7] = {GRID(0,0,0), GRID(1,0,0), GRID(1,0,0), GRID(0,-1,0), GRID(0,1,0), GRID(0,0,-1), GRID(0,0,1)}
    for(GRID& dg : dgs)
    {
        GRID g = p.grid + g;
        for(PARTICLE_NUMBER it : grid_2_particles[g] )
        {
            if(distance_sqr(particles[it], p) < matrial.h_squre)
            {
                ans.insert(it);
            }
        }
    }

    return ans;
}

ERROR_CODE SPH_handle::set_export_file(const char * path)
{
    if(fp)
    {
        return ERROR_CODE::ERR_FILE_IS_OPENED;
    }
    FILE * fp = fopen(path, "wb");
    //const size_t data_len = sizeof(UINT32);
    //fwrite(&data_len, sizeof(size_t), 1, fp);
    if(fp == nullptr)
    {
        return ERROR_CODE::ERR_FILE_OPEN_FAILED;
    }
    return ERROR_CODE::SUCCESS;
}

void SPH_handle::run(UINT32 step)
{
    for(UINT32 s = 0 ; s < step; s ++)
    {
        for(PARTICLE_NUMBER it = 0 ; it < particles.size(); it ++)
        {
            Particle &p = particles[it];

            const std::vector<PARTICLE_NUMBER> particles_numbers = get_nearby_paticles(p); // move construct

            grid_2_particles[p.grid].erase(it);
            p.update(dt, matrial, particles, particles_numbers);
            grid_2_particles[p.grid].insert(it);
        }

        if(fp)
        {
            for(Particle &i : particles)
            {
                FLOAT * data = i.pos.getData();
                fwrite(data, sizeof(FLOAT), DIM, fp);
            }
        }
    }

    if(fp)
    {
        step_cnt += step;
    }
}