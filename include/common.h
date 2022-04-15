#ifndef COMMON_H
#define COMMON_H

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

typedef float FLOAT;
typedef unsigned short UINT8;
typedef unsigned int UINT32;

typedef UINT32 PARTICLE_NUMBER ;

constexpr FLOAT ZERO = 1e-7;

#endif