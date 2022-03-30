#ifndef SPH_H
#define SPH_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <functional>
#include <random>
#include <cmath>

#include "matrix3d.h"
#include "Particle.h"
#include "material.h"
#include "Timer.h"

class SPH_handle
{
public:

    SPH_handle(FLOAT x, FLOAT y, FLOAT z, UINT32 N = 1000, std::string m = "water");
    ~SPH_handle();

    ERROR_CODE set_export_file(const char * path);

    void run(UINT32 step);

private:

    // Parameters
    const UINT32 Dimension = DIM;
    UINT32 ParitcleNumber;
    vector3D volume;
    FLOAT dt; // integration timestep
    Material matrial;

    // Statistics
    UINT32 step_cnt;
    Timer timer;

    // Facilities
    FILE * fp;
    std::vector<Particle> particles;
    std::map< GRID, std::set<PARTICLE_NUMBER> > grid_2_particles;

    std::vector<PARTICLE_NUMBER> get_nearby_paticles(Particle &p) const;

    static FLOAT kernel_poly6(vector3D r, FLOAT h);
    static vector3D kernel_spiky(vector3D r, FLOAT h);
    static FLOAT kernel_viscosity(FLOAT r, FLOAT h);

};

#endif