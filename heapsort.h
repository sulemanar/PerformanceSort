// This file is part of the "PerformanceSort" project by Suleman
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include "stats.h"

#include <utility>

namespace sorting::heapsort {

// To heapify a subtree rooted with node i which is
// an index in a[]. n is size of heap
template <typename Container>
void heapify(Container& a, int n, int i, AlgorithmStats& stats)
{
    auto largest = i;          // Initialize largest as root
    auto const l = 2 * i + 1;  // left = 2*i + 1
    auto const r = 2 * i + 2;  // right = 2*i + 2

    stats.enter();

    // If left child is larger than root
    if (l < n)
    {
        stats.compareCount++;
        if (a[l] > a[largest])
            largest = l;
    }

    // If right child is larger than largest so far
    if (r < n)
    {
        stats.compareCount++;
        if (a[r] > a[largest])
            largest = r;
    }

    // If largest is not root
    if (largest != i)
    {
        std::swap(a[i], a[largest]);
        stats.swapCount++;

        // Recursively heapify the affected sub-tree
        heapify(a, n, largest, stats);
    }

    stats.leave();
}

template <typename Container>
AlgorithmStats sort(Container& a)
{
    AlgorithmStats stats;
    auto const n = static_cast<int>(std::size(a));  // We need signedness in order to decrement from 0 to -1.
    if (n < 2)
        return stats;

    stats.iterationCount++;
    // Build heap (rearrange array)
    for (auto i = n / 2 - 1; i >= 0; i--, stats.iterationCount++)
        heapify(a, n, i, stats);

    // One by one extract an element from heap
    for (auto i = n - 1; i >= 0; i--)
    {
        stats.iterationCount++;

        // Move current root to end
        std::swap(a[0], a[i]);
        stats.swapCount++;

        // call max heapify on the reduced heap
        heapify(a, i, 0, stats);
    }

    return stats;
}

}  // namespace sorting::heapsort
