// This file is part of the "PerformanceSort" project by Suleman
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include "stats.h"

#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>

namespace sorting::performance_sort {

template <typename Container>
auto partition(Container& p, size_t low, size_t high, AlgorithmStats& stats) -> size_t
{
    auto i = low - 1;
    auto const pivot = p[high];  // Here we could optimize by choosing different pivot elements.

    auto const isLess = [&](auto const& a, auto const& b) -> bool {
        stats.compareCount++;
        return a < b;
    };

    auto const incAndSwap = [&](size_t j) {
        ++i;
        std::swap(p[i], p[j]);
        stats.swapCount++;
    };

    for (auto j = low; j < high; ++j, stats.iterationCount++)
        if (isLess(p[j], pivot))
            incAndSwap(j);

    incAndSwap(high);

    return i;
}

template <typename Container>
void sortRange(Container& p, size_t low, size_t high, AlgorithmStats& stats)
{
    stats.enter();

    if (low < high)
    {
        auto const pi = partition(p, low, high, stats);
        if (pi > 0)
            sortRange(p, low, pi - 1, stats);
        sortRange(p, pi + 1, high, stats);
    }

    stats.leave();
}

template <typename Container>
auto sort(Container& p) -> AlgorithmStats
{
    AlgorithmStats stats;
    if (auto size = std::size(p); size != 0)
        sortRange(p, 0, size - 1, stats);
    return stats;
}

}  // namespace sorting::performance_sort
