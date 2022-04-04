#include "SPH_handle.h"

SPH_handle::SPH_handle(std::string input, std::string output)
: isReady(true), ifile(input), ofile(output), para(parameter(input))
{
    if(!para.isReady)
    {
        isReady = false;
    }

    ofs = std::ofstream(ofile);
    if(!ofs.good())
    {
        isReady = false;
    }

    step_cnt        = 0 ; 
    
    srand(time(0));
    for(int it = 0; it < para.number; it++)
    {
        FLOAT pos[DIM];
        for(auto d = 0 ; d < DIM; d++)
        {
            pos[d] = para.volume[d]/3 + static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(para.volume[d]/3)));
        }
        particles.push_back( Particle(para, vector3D(pos) ) );
        grid_2_particles[particles.back().grid].insert(it);
        ofs.write(reinterpret_cast<const char*>(pos), sizeof(FLOAT) * DIM);
    }
}

SPH_handle::~SPH_handle()
{

}

std::set<PARTICLE_NUMBER> SPH_handle::get_nearby_paticles(PARTICLE_NUMBER p)
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
        GRID g = particles[p].grid + dg;
        for( PARTICLE_NUMBER it : grid_2_particles[g] )
        {
            if(distance_sqr(particles[it], particles[p]) < para.h_squre)
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

    std::map<PARTICLE_NUMBER, std::set<PARTICLE_NUMBER> > P_2_nearP;
    
    for(UINT32 s = 0 ; s < step; s ++)
    {
        if (s % 100 == 0)
        {
            std::cout << s + step_cnt << " / " << step + step_cnt;
            std::cout << "  cost time: " << timer.elapsed() << "ms";
            std::cout << std::endl;
        }

        for(PARTICLE_NUMBER it = 0 ; it < particles.size(); it ++)
        {
            P_2_nearP[it] = get_nearby_paticles(it); // move assign
            particles[it].update_rho(para, particles, P_2_nearP[it]);
        }

        for(PARTICLE_NUMBER it = 0 ; it < particles.size(); it ++)
        {
            P_2_nearP[it].erase(it);
            Particle &p = particles[it];
            grid_2_particles[p.grid].erase(it);
            p.update(para, particles, P_2_nearP[it]);
            grid_2_particles[p.grid].insert(it);
        }

        for(Particle &p : particles)
        {
            ofs.write(reinterpret_cast<const char*>(p.pos.getData()), sizeof(FLOAT) * DIM);
        }
    }
    std::cout << step + step_cnt << " / " << step + step_cnt;
    std::cout << "  cost time: " << timer.elapsed() << "ms";
    std::cout << std::endl;

    step_cnt += step;
}