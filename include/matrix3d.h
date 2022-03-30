#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <cstring>

#include "common.h"

#define DIM 3

struct GRID
{
    GRID();
    GRID(UINT32 x_, UINT32 y_, UINT32 z_);
    UINT32 grid[DIM];
    bool operator<(const GRID & g) const;
    GRID operator+ (const GRID & g) const;
    UINT32& operator[](const UINT32 it);
};

struct vector3D
{
    FLOAT data[DIM];

    vector3D();
    vector3D(const vector3D &v);
    vector3D(FLOAT x_, FLOAT y_, FLOAT z_);

    vector3D operator/ (const vector3D & v) const;
    vector3D operator/ (const FLOAT d) const;
    vector3D operator+ (const vector3D & v) const;
    vector3D operator- (const vector3D & v) const;
    FLOAT  operator* (const vector3D & v) const;
    FLOAT& operator[] (const UINT32 it);

    FLOAT abs();

    FLOAT * getData();
};

#endif