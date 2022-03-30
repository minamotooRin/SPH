#include "matrix3d.h"

bool GRID::operator < (const GRID & g) const
{
    return x == g.x ? x < g.x : y == g.y ? y < g.y : z == g.z ? z < g.z : false;
}

GRID GRID::operator+ (const GRID & g) const
{
    return GRID(x + g.x, y + g.y, z + g.z);
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

GRID vector3D::getfloor(FLOAT r) const
{
    return GRID(
        static_cast<UINT32>(floor(data[0] / r)), 
        static_cast<UINT32>(floor(data[1] / r)), 
        static_cast<UINT32>(floor(data[2] / r))
    );
}