#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <string>

#include "common.h"

struct Material
{
    FLOAT m; // mass
    FLOAT h; // kernel radius
    FLOAT mu; // viscosity
    FLOAT k; // gasConstant
    FLOAT rho0; // rest desity
    FLOAT sigma; // tension coefficient

    FLOAT h_squre;
    FLOAT h_cubic;

    Material(FLOAT m_ = 1, FLOAT h_ = 1, FLOAT mu_ = 1, FLOAT k_ = 1e-20, FLOAT rho0_ = 1000, FLOAT sigma_ = 100):
    m(m_),h(h_),mu(mu_),k(k_),rho0(rho0_),sigma(sigma_)
    {
        h_squre = h * h;
        h_cubic = h_squre * h;
    }
};

extern std::map<std::string, Material> gMaterialInfo;

#endif