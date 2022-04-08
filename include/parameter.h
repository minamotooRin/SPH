#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <fstream>

#include "common.h"
#include "matrix3d.h"
#include "configor/json.hpp"

struct parameter
{
public:

    parameter(std::string ifile)
    {
        isReady = true;
        std::ifstream ifs(ifile);
        if(ifs.good())
        {
            ifs >> j;
            number      = j["paritcle_number"].get<UINT32>();
            part        = j["centralization"].get<UINT32>();
            volume[0]   = j["volume"]["x"].get<FLOAT>();
            volume[1]   = j["volume"]["y"].get<FLOAT>();
            if(DIM == 3) volume[2]   = j["volume"]["z"].get<FLOAT>();
            step        = j["step"].get<UINT32>();
            dt          = j["dt"].get<FLOAT>();
            g           = j["gravity"].get<FLOAT>();

            m           = j["mass"].get<FLOAT>();
            h           = j["kernel_radius"].get<FLOAT>();
            mu          = j["viscosity"].get<FLOAT>();
            k           = j["gas_constant"].get<FLOAT>();
            rho0        = j["rest_desity"].get<FLOAT>();
            sigma       = j["tension_coefficient"].get<FLOAT>();

            h_squre = h * h;
            h_cubic = h_squre * h;
        }
        else
        {
            isReady = false;
        }
    }

    std::stringstream get_info()
    {
        std::stringstream s;
        s << j;
        return s;
    }

    bool isReady;

    vector3D volume;
    UINT32 number;
    UINT32 part;
    UINT32 step;
    FLOAT dt; // integration timestep
    FLOAT g; // gravity

    FLOAT m; // mass
    FLOAT h; // kernel radius
    FLOAT mu; // viscosity
    FLOAT k; // gas constant
    FLOAT rho0; // rest desity
    FLOAT sigma; // tension coefficient

    FLOAT h_squre;
    FLOAT h_cubic;

private:

    configor::json j;
    
};

#endif