#include "material.h"

// maybe a JSON is more proper

std::map<std::string, Material> gMaterialInfo = {
    {"water", Material(1, 1, 1, 1.38 * 1e-23, 998, 72.75)},
    {"default", Material(1, 1, 1, 1.38 * 1e-23, 998, 72.75)}
};