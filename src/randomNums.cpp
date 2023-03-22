#include <chrono>
#include <random>
#include <iostream>

namespace randomNumbers
{
    std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count() +
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count() );

    std::mt19937 gen(seed);

    int getRandomNum(int min, int max)
    {
        std::uniform_int_distribution<unsigned> dist(min, max);
        return dist(gen);
    }
};