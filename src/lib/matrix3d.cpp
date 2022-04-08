#include "matrix3d.h"

GRID::GRID()
{
    memset(grid, 0, sizeof(grid));
}
GRID::GRID(UINT32 v[DIM])
{
    memcpy(grid, v, sizeof(UINT32) * DIM);
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
        ans[it] = ans[it] + g[it];
    return ans;
}

UINT32& GRID::operator[](const UINT32 it)
{
    return grid[it];
}

const UINT32& GRID::operator[](const UINT32 it) const
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
vector3D::vector3D(FLOAT v[DIM])
{
    memcpy(data, v, sizeof(FLOAT) * DIM);
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

vector3D& vector3D::operator+= (const vector3D & v)
{
    for(auto i = 0 ; i < DIM ; ++i)
        data[i] += v[i];
    return *this;
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
 vector3D vector3D::operator* (const FLOAT d) const
 {
    vector3D ans = *this;
    for(auto i = 0 ; i < DIM ; ++i)
        ans[i] = ans[i] * d;
    return ans;
 }

vector3D& vector3D::operator*= (const FLOAT d)
{
    for(auto i = 0 ; i < DIM ; ++i)
        data[i] *= d;
    return *this;
}

FLOAT& vector3D::operator[] (const int it)
{
    return data[it];
}

const FLOAT& vector3D::operator[] (const int it) const
{
    return data[it];
}


void vector3D::print()
{
    std::cout << "(";
    for(FLOAT it : data)
        std::cout << it << ", ";
    std::cout << ")" << std::endl;
}

FLOAT vector3D::abs()
{
    FLOAT ans = 0;
    for(FLOAT it : data)
        ans += it * it;
    return sqrt(ans);
} 

FLOAT vector3D::min_ele()
{
    FLOAT ans = data[0];
    for(FLOAT it : data)
        ans = ans < it ? ans : it;
    return ans;
}

bool vector3D::isnormal()
{
    for(FLOAT it : data)
        if(!std::isnormal(it))
            return false;
    return true;
}

FLOAT * vector3D::getData()
{
    return data;
}