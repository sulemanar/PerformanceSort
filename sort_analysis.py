#! /usr/bin/env python3
# This file is part of the "PerformanceSort" project by Suleman.
#   (c) 2019 Suleman Arshad <algernonk4@gmail.com>
#
# Licensed under the MIT License (the "License"); you may not use this
# file except in compliance with the License. You may obtain a copy of
# the License at: http://opensource.org/licenses/MIT

import time

class _StatsBuilder:
    def __init__(self):
        self.compares_ = 0
        self.swaps_ = 0
        self.calls_ = 0
        self.iterations_ = 0
        self.recursion_depth_ = 0
        self.current_recursion_depth_ = 0
        self.start_time_ = time.time()

    def compare(self):
        self.compares_ += 1

    def swap(self):
        self.swaps_ += 1

    def call(self):
        self.calls_ += 1

    def iterate(self):
        self.iterations_ += 1

    def enter(self):
        self.current_recursion_depth_ = self.current_recursion_depth_ + 1
        if self.current_recursion_depth_ > self.recursion_depth_:
            self.recursion_depth_ = self.current_recursion_depth_

    def leave(self):
        self.current_recursion_depth_ += 1

    def elapsed(self):
        return time.time() - self.start_time_

    def summarize(self):
        return {"compares": self.compares_,
                "swaps": self.swaps_,
                "calls": self.calls_,
                "iterations": self.iterations_,
                "recursion_depth": self.recursion_depth_,
                "time": self.elapsed()}

    def __str__(self):
        fmt = "{{compares: {}, swaps: {}, calls: {}, iterations: {}, recursion: {}, elapsed: {}}}"
        return fmt.format(self.compares_, self.swaps_, self.calls_, self.iterations_,
                          self.recursion_depth_, self.elapsed())

class HeapSort:
    @staticmethod
    def heapify(a, n, i, stats):
        largest = i
        l = 2 * i + 1
        r = 2 * i + 2

        stats.enter()

        if l < n:
            stats.compare()
            if a[l] > a[largest]:
                largest = l

        if r < n:
            stats.compare()
            if a[r] > a[largest]:
                largest = r

        if largest != i:
            stats.swap()
            a[i], a[largest] = a[largest], a[i]
            HeapSort.heapify(a, n, largest, stats)

        stats.leave()

    @staticmethod
    def sort(a):
        stats = _StatsBuilder()
        n = len(a)

        # short-circuit trivial inputs
        if n < 2:
            return stats

        # build heap by rearranging array
        i = n // 2 - 1
        while i >= 0:
            stats.iterate()
            HeapSort.heapify(a, n, i, stats)
            i = i - 1

        # one-by-one, extract an element from the heap
        i = n - 1
        while i >= 0:
            stats.iterate()

            # move current root to the end
            stats.swap()
            a[0], a[i] = a[i], a[0]

            # call max heapify on the reduced heap
            HeapSort.heapify(a, i, 0, stats)

            i = i - 1

        return stats.summarize()

class QuickSort:
    @staticmethod
    def partition(p, low, high, stats):
        def is_less(a, b):
            stats.compare()
            return a < b

        def inc_i_and_swap_at(i, j):
            """ increments i and swaps p[i] with p[j] """
            i = i + 1
            stats.swap()
            p[i], p[j] = p[j], p[i]
            return i

        i = low - 1
        pivot = p[high]

        for j in range(low, high):
            stats.iterate()
            if is_less(p[j], pivot):
                i = inc_i_and_swap_at(i, j)

        return inc_i_and_swap_at(i, high)

    @staticmethod
    def sort_range(p, low, high, stats):
        stats.enter()

        if low < high:
            p_i = QuickSort.partition(p, low, high, stats)
            if p_i > 0:
                QuickSort.sort_range(p, low, p_i - 1, stats)
            QuickSort.sort_range(p, p_i + 1, high, stats)

        stats.leave()

    @staticmethod
    def sort(a):
        stats = _StatsBuilder()
        n = len(a)
        if n != 0:
            QuickSortAlgorithm.sort_range(a, 0, n - 1, stats)
        return stats.summarize()

# as per home-assignment
def sort_A(a):
    stats = QuickSortAlgorithm.sort(a)
    return (stats["compares"] + stats["swaps"], stats["time"])

# as per home-assignment
def sort_B(a):
    stats = HeapSort.sort(a)
    return (stats["compares"] + stats["swaps"], stats["time"])

def read_words_from_file(filename):
    with open(filename, mode='r', encoding='utf-8') as f:
        return f.read().split()

def _private_test():
    def test_algo(name, sort, words):
        stats = sort(words)
        print("{}: {}".format(name, stats))
        for w in words:
            print("  word: {}".format(w))

    words = ["F", "A", "C", "B"] # read_words_from_file("test.txt")
    print("input list:")
    for w in words:
        print("  word: {}".format(w))

    test_algo("PerformanceSort", QuickSortAlgorithm.sort, words[:])
    test_algo("heapsort", HeapSort.sort, words[:])

if __name__ == "__main__":
    _private_test()
