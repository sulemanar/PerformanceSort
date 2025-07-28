// This file is part of the "PerformanceSort" project by Suleman.
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include "heapsort.h"
#include "PerformanceSort.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <iomanip>

using namespace std;

template <typename IterI, typename IterE>
auto to_string(IterI i, IterE e) -> string
{
    if (i == e)
        return string{};

    stringstream sstr;
    sstr << *i;
    while (++i != e)
        sstr << ", " << *i;
    return sstr.str();
}

template <typename T>
string to_string(vector<T> const& a)
{
    return to_string(begin(a), end(a));
}

template <typename T, size_t N>
string to_string(const T (&a)[N])
{
    return to_string(begin(a), end(a));
}

template <typename Vector, typename Sorter>
void test_sort(const char* name, Vector a, Sorter sorter)
{
    auto const astats = sorter(a);

    cout << setw(10) << name << ": " << astats << '\n';

    // cout << "    -> " << to_string(a) << '\n';

    // TODO: fix bug in quicksort ;-(
    for (size_t n = 1; n < a.size(); ++n)
        assert(a[n - 1] <= a[n]);
}

template <typename T>
auto createRandomVector(size_t n) -> vector<T>
{
    auto a = vector<T>{};
    auto rng = mt19937{};
    rng.seed(static_cast<unsigned>(time(nullptr)));
    generate_n(back_inserter(a), n, [&]() { return static_cast<T>(rng() % n); });
    return a;
}

int main(int argc, char const** argv)
{
    auto const a = createRandomVector<long>(argc >= 2 ? stoi(argv[1]) : 16 * 1024);
    using C = remove_const<decltype(a)>::type;

    //cout << "unsorted: " << to_string(a) << '\n';
    test_sort("performance_sort", a, sorting::performance_sort::sort<C>);
    test_sort("performance_sort", a, sorting::performance_sort::sort<C>);

	// auto const a3 = vector{1, 2, 0};
	// auto a3_ = a3;
	// auto stats = sorting::performance_sort::sort(a3_);
	// cout << "a3_ -> " << to_string(a3_) << '\n';
	// assert(a3_ == (vector{0, 1, 2}));

    return EXIT_SUCCESS;
}
