#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cstring>

#define DIM 3

typedef double FLOAT;
typedef unsigned short UINT8;
typedef unsigned int UINT32;

typedef UINT32 PARTICLE_NUMBER ;

enum class ERROR_CODE
{
    SUCCESS, // 0
    ERR_FILE_OPEN_FAILED,
    ERR_FILE_IS_OPENED
};

struct GRID
{
    GRID():x(0),y(0),z(0){}
    GRID(UINT32 x_, UINT32 y_, UINT32 z_):x(x_),y(y_),z(z_){}
    UINT32 x;
    UINT32 y;
    UINT32 z;
    bool operator<(const GRID & g) const;
    GRID operator+ (const GRID & g) const;
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
    
    GRID getfloor( / r) const;
};

#endif