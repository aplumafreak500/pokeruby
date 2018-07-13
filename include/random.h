#ifndef GUARD_RANDOM_H
#define GUARD_RANDOM_H

extern u32 gRngValue;

//Returns a 16-bit pseudorandom number
u16 Random(void);
u16 Random_XD(void);
u16 Random_N64(void);
u16 Random_Generation4(void);

//Returns a 32-bit pseudorandom number
#define Random32() (Random() | (Random() << 16))

// Wishmaker Jirachi PID generation
#define Random32_Swapped() (Random() << 16 | (Random()))

//Sets the initial seed value of the pseudorandom number generator
void SeedRng(u16 seed);

#endif // GUARD_RANDOM_H

