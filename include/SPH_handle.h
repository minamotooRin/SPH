#ifndef SPH_H
#define SPH_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <functional>
#include <random>
#include <cmath>
#include <fstream>
#include <iostream>
#include <atomic>

#include "Particle.h"
#include "Timer.h"
#include "threadPool.h"

enum class ERROR_CODE
{
    SUCCESS, // 0
    ERR_FILE_OPEN_FAILED
};

class SPH_handle
{
public:

    SPH_handle(std::string input, std::string output);
    ~SPH_handle();

    void run();

private:

    bool isReady;

    // I/O
    std::string ifile;
    std::string ofile;
    parameter para;
    std::ofstream ofs;

    // Statistics
    UINT32 step_cnt;
    Timer timer;

    // Facilities
    std::vector<Particle> particles;
    std::map< GRID, std::set<PARTICLE_NUMBER> > grid_2_particles;
    std::set<PARTICLE_NUMBER> get_nearby_paticles(PARTICLE_NUMBER p);

    // Functions
    void run(UINT32 step);

    // thread pool
    std::threadpool pool;
    static constexpr UINT32 thread_cnt = THREAD_CNT;

};

#endif