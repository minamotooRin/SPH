#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <cstring>
#include <cfloat>
#include <iostream>

#include "common.h"

// #define DIM 3 // Define in Cmakelists

struct GRID
{
    GRID();
    GRID(UINT32 v[DIM]);
    UINT32 grid[DIM];
    bool operator<(const GRID & g) const;
    GRID operator+ (const GRID & g) const;
    UINT32& operator[](const UINT32 it);
    const UINT32& operator[](const UINT32 it) const;
};

struct vector3D
{
    FLOAT data[DIM];

    vector3D();
    vector3D(const vector3D &v);
    vector3D(FLOAT v[DIM]);

    vector3D operator/ (const vector3D & v) const;
    vector3D operator/ (const FLOAT d) const;
    vector3D operator+ (const vector3D & v) const;
    vector3D& operator+= (const vector3D & v) ;
    vector3D operator- (const vector3D & v) const;
    FLOAT  operator* (const vector3D & v) const;
    vector3D operator* (const FLOAT d) const;
    vector3D& operator*= (const FLOAT d) ;
    FLOAT& operator[] (const int it);
    const FLOAT& operator[] (const int it) const;

    void print();
    FLOAT abs();
    FLOAT min_ele();
    bool isnormal();

    FLOAT * getData();
};

#endif