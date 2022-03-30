#include "matrix3d.h"

GRID::GRID()
{
    memset(grid, 0, sizeof(grid));
}
GRID::GRID(UINT32 x_, UINT32 y_, UINT32 z_)
{
    grid[0] = x_;
    grid[1] = y_;
    grid[2] = z_;
}

bool GRID::operator < (const GRID & g) const
{
    for(auto it = 0; it < DIM; ++it)
    {
        if( grid[it] == g[it]) continue;
        return grid[it] < g[it];
    }
    return false;
}

GRID GRID::operator+ (const GRID & g) const
{
    GRID ans = *this;
    for(auto it = 0 ; it < DIM ; ++it)
        ans[i] = ans[i] + g[i];
    return ans;
}

UINT32& GRID::operator[](const UINT32 it)
{
    return grid[it];
}

vector3D::vector3D()
{
    memset(data, 0, sizeof(data));
}
vector3D::vector3D(const vector3D &v)
{
    for(auto i = 0 ; i < DIM ; ++i)
    {
        data[i] = v[i];
    }
}
vector3D::vector3D(FLOAT x_, FLOAT y_, FLOAT z_)
{
    data[0] = x_;
    data[1] = y_;
    data[2] = z_;
}

vector3D vector3D::operator/ (const vector3D & v) const
{
    vector3D ans = *this;
    for(auto i = 0 ; i < DIM ; ++i)
        ans[i] = ans[i] / v[i];
    return ans;
}
vector3D vector3D::operator/ (const FLOAT d) const
{
    vector3D ans = *this;
    for(auto i = 0 ; i < DIM ; ++i)
        ans[i] = ans[i] / d;
    return ans;
}

vector3D vector3D::operator+ (const vector3D & v) const
{
    vector3D ans = *this;
    for(auto i = 0 ; i < DIM ; ++i)
        ans[i] = ans[i] + v[i];
    return ans;
}

vector3D vector3D::operator- (const vector3D & v) const
{
    vector3D ans = *this;
    for(auto i = 0 ; i < DIM ; ++i)
        ans[i] = ans[i] - v[i];
    return ans;
}
FLOAT vector3D::operator* (const vector3D & v) const
{
    FLOAT ans = 0;
    for(auto i = 0 ; i < DIM ; ++i)
        ans += data[i] * v.data[i];
    return ans;
}

FLOAT& vector3D::operator[] (const UINT32 it)
{
    return data[it];
}

FLOAT vector3D::abs()
{
    FLOAT ans = 0;
    for(FLOAT it : data)
        ans += it * it;
    return sqrt(ans);
} 

FLOAT * vector3D::getData()
{
    return data;
}