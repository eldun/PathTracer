#ifndef RANDOMH
#define RANDOMH

#include <cstdlib>
#include <random>

double random_double(double min, double max) {
    max *= 1000.0;
    std::random_device dev;
    std::mt19937 rng(dev()); //Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max); // distribution in range [min, max]

    return dist6(rng) / 1000.0;
}

#endif // !RANDOMH

