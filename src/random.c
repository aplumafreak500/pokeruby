#include "global.h"
#include "random.h"

// The number 1103515245 comes from the example implementation of rand and srand
// in the ISO C standard.

//u32 gRngValue;
u32 gRngValues[4];

u16 Random(void)
{
    gRngValues[RNG_VALUE_LCRNG] = 1103515245 * gRngValues[RNG_VALUE_LCRNG] + 24691;
    return gRngValues[RNG_VALUE_LCRNG] >> 16;
}

u16 Random_XD(void)
{
    gRngValues[RNG_VALUE_CXD] = 214013 * gRngValues[RNG_VALUE_CXD] + 2531011;
    return gRngValues[RNG_VALUE_CXD] >> 16;
}

u16 Random_N64(void)
{
    gRngValues[RNG_VALUE_N64] = 1103515245 * gRngValues[RNG_VALUE_N64] + 12347;
    return gRngValues[RNG_VALUE_N64] >> 16;
}

u16 Random_Generation4(void)
{
    gRngValues[RNG_VALUE_DS] = 1812433253 * gRngValues[RNG_VALUE_DS] + 1;
    return gRngValues[RNG_VALUE_DS] >> 16;
}

void SeedRng(u16 seed)
{
    gRngValues[RNG_VALUE_LCRNG] = seed;
    gRngValues[RNG_VALUE_CXD] = seed;
    gRngValues[RNG_VALUE_N64] = seed;
    gRngValues[RNG_VALUE_DS] = seed;
}

//Returns a 32-bit pseudorandom number
u32 Random32(void) {
	return (Random() | (Random() << 16));
}

// Wishmaker Jirachi PID generation
u32 Random32_Swapped(void) {
	return (Random() << 16 | (Random()));
}

