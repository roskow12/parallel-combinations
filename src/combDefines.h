#ifndef _COMB_DEFINES_H_
#define _COMB_DEFINES_H_

#include <cstdint> 

constexpr uint8_t N = 100;
constexpr uint8_t K = 8;
constexpr uint8_t NUM_THREADS = 4;

constexpr uint8_t LAST_INDEX = K-1;
constexpr uint8_t K_MINUS_2 = K-2;
constexpr uint8_t N_MINUS_1 = N-1; 
constexpr uint8_t N_MINUS_K = N-K; 

#endif // _COMB_DEFINES_H_