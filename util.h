#pragma once

#include <vector>
#include <algorithm>

#define VEC_CONTAINS(haystack, needle) (std::find(haystack.begin(), haystack.end(), needle) != haystack.end())

// shamelessly stolen from stackoverflow
// http://codereview.stackexchange.com/a/59998
template <typename T>
bool contains(std::vector<T> const &v, T const &x)
{ 
    return ! (v.empty() &&
		std::find(v.begin(), v.end(), x) == v.end());
}