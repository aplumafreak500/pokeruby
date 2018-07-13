#include "global.h"
#include "random.h"

// The number 1103515245 comes from the example implementation of rand and srand
// in the ISO C standard.

u32 gRngValue;

u16 Random(void)
{
    gRngValue = 1103515245 * gRngValue + 24691;
    return gRngValue >> 16;
}

u16 Random_XD(void)
{
    gRngValue = 214013 * gRngValue + 2531011;
    return gRngValue >> 16;
}

u16 Random_N64(void)
{
    gRngValue = 1103515245 * gRngValue + 12347;
    return gRngValue >> 16;
}

u16 Random_Generation4(void)
{
    gRngValue = 1812433253 * gRngValue + 1;
    return gRngValue >> 16;
}

void SeedRng(u16 seed)
{
    gRngValue = seed;
}
