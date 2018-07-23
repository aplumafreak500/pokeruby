#ifndef GUARD_RANDOM_H
#define GUARD_RANDOM_H

//extern u32 gRngValue;
extern u32 gRngValues[4];

#define RNG_VALUE_LCRNG 0
#define RNG_VALUE_CXD   1
#define RNG_VALUE_N64   2
#define RNG_VALUE_DS    3

//Returns a 16-bit pseudorandom number
u16 Random(void);
u16 Random_XD(void);
u16 Random_N64(void);
u16 Random_Generation4(void);

//Sets the initial seed value of the pseudorandom number generator
void SeedRng(u16 seed);

//Returns a 32-bit pseudorandom number
u32 Random32();

// Wishmaker Jirachi PID generation
u32 Random32_Swapped();

#endif // GUARD_RANDOM_H

