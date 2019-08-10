// Copyright (c) 2018 Martyn Afford
// Licensed under the MIT licence

#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include "diamond_square.hpp"

int
main()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine engine{seed};
    std::uniform_real_distribution<float> distribution{0, 1};

    constexpr auto size = 513;
    constexpr auto edge = size - 1;

    uint8_t map[size][size] = {{0}};
    map[0][0] = map[edge][0] = map[0][edge] = map[edge][edge] = 128;

    heightfield::diamond_square_no_wrap(
        size,
        // random
        [&engine, &distribution](float range)
        {
            return distribution(engine) * range;
        },
        // variance
        [](int level) -> float
        {
            return 64.0f * std::pow(0.5f, level);
        },
        // at
        [&map](int x, int y) -> uint8_t&
        {
            return map[y][x];
        }
    );

    // Output PGM (Netpbm greyscale image format)
    std::cout << "P2 " << size << ' ' << size << " 255\n";

    for (auto& row : map) {
        for (auto& cell : row) {
            std::cout << static_cast<int>(cell) << '\n';
        }
    }

    return EXIT_SUCCESS;
}
