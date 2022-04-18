#include "SPH_handle.h"

SPH_handle::SPH_handle(std::string input, std::string output)
: isReady(true), ifile(input), ofile(output), para(parameter(input)), pool(std::threadpool(thread_cnt))
{
    if(!para.isReady)
    {
        isReady = false;
        return ;
    }

    if(ofile.empty())
    {
        ofile = std::to_string(para.number) + std::string("_") 
            + std::to_string(para.step) 
            + std::string(".bin");
    }
    ofs = std::ofstream(ofile);
    if(!ofs.good())
    {
        isReady = false;
    }

    char meta_info[2] = {DIM, sizeof(FLOAT)};
    ofs.write(meta_info, sizeof(meta_info));

    step_cnt        = 0 ; 

    auto power = [](UINT32 b, UINT32 x) -> UINT32
    {
        UINT32 ans = 1;
        constexpr UINT32 m = 1e7+ 7;
        while (x > 0)
        {
            if (x & 1) ans = ans * b % m;
            b = b * b % m;
            x >>= 1;
        }
        return ans;
    };
    UINT32 bottom_number = para.number / (power( 2 * para.part + 1, DIM - 1) + 1);
    
    srand(time(0));
    for(PARTICLE_NUMBER it = 0; it < para.number - bottom_number; it++)
    {
        FLOAT pos[DIM];
        for(auto d = 0 ; d < DIM - 1; d++)
        {
            pos[d] =  static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(para.volume.min_ele())));
        }
        pos[DIM - 1] =  static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(para.volume.min_ele()/(2.0 * para.part + 1))));
    
        particles.emplace_back( Particle(para, vector3D(pos) ) );
        grid_2_particles[particles.back().grid].insert(it);
        ofs.write(reinterpret_cast<const char*>(pos), sizeof(FLOAT) * DIM);
    }
    for(PARTICLE_NUMBER it = para.number - bottom_number; it < para.number; it++)
    {
        FLOAT pos[DIM];
        for(auto d = 0 ; d < DIM; d++)
        {
            // why not uniform ???
            pos[d] = para.part * para.volume[d] / (2.0 * para.part + 1)
                + static_cast <FLOAT> (rand()) /( static_cast <FLOAT> (RAND_MAX/(para.volume.min_ele()/(2.0 * para.part + 1))));
        }
        particles.emplace_back( Particle(para, vector3D(pos) ) );
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

    const static std::vector<GRID> dgs = [&](){
        std::vector<GRID> ans;

        UINT32 arr[DIM] = {0};
        ans.emplace_back(GRID(arr)); // 0 0 0 ...
        for(int i = 0; i < DIM; ++i)
        {
            memset(arr, 0, sizeof(arr));
            arr[i] = 1;
            ans.emplace_back(GRID(arr));
            arr[i] = -1;
            ans.emplace_back(GRID(arr));
        }

        return ans;
    }();
    for( const GRID & dg : dgs)
    {
        GRID g = particles[p].grid + dg;
        for( PARTICLE_NUMBER it : grid_2_particles[g] )
        {
            if(distance_sqr(particles[it], particles[p]) < para.h2)
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
    
    std::vector<std::set<PARTICLE_NUMBER>> P_2_nearbyP(para.number);
    std::vector<Particle> particles_dup;

    std::queue<std::future<void> > results;
    std::atomic<PARTICLE_NUMBER> progress = 0;
    
    for(UINT32 s = 0 ; s < step; s ++)
    {
        if (s % 100 == 0)
        {
            std::cout << timer.elapsed() << "ms\t";
            std::cout << s + step_cnt << " / " << step + step_cnt << std::endl;
        }

        progress = 0;
        auto process_rho = [&]() -> void
        {
            PARTICLE_NUMBER it = 0;
            while(progress < para.number)
            {
                it = progress++;
                if(unlikely(it >= para.number)) break;
                P_2_nearbyP[it] = get_nearby_paticles(it);
                particles[it].update_rho(para, particles, P_2_nearbyP[it]);
            }
        };
        for(UINT32 t_cnt = 0; t_cnt < thread_cnt; t_cnt++)
        {
            results.emplace(pool.commit(process_rho));
        }
        while(!results.empty())
        {
            results.front().get();
            results.pop();
        }

        particles_dup = particles;

        progress = 0;
        auto process_update = [&]() -> void
        {
            PARTICLE_NUMBER it = 0;
            while(progress < para.number)
            {
                it = progress++;
                if(unlikely(it >= para.number)) break;
                particles[it].update(para, particles_dup, P_2_nearbyP[it]);
            }
        };
        for(UINT32 t_cnt = 0; t_cnt < thread_cnt; t_cnt++)
        {
            results.emplace(pool.commit(process_update));
        }
        while(!results.empty())
        {
            results.front().get();
            results.pop();
        }

        for(auto & it : grid_2_particles)
        {
            it.second.clear();
        }
        for(PARTICLE_NUMBER it = 0 ; it < particles.size(); it ++)
        {
            Particle &p = particles[it];
            grid_2_particles[p.grid].insert(it);
            ofs.write(reinterpret_cast<const char*>(p.pos.getData()), sizeof(FLOAT) * DIM);
        }
    }
    std::cout << timer.elapsed() << "ms\t";
    std::cout << step + step_cnt << " / " << step + step_cnt << std::endl;

    step_cnt += step;
}