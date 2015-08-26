#pragma once

#include <vector>
#include <algorithm>

// shamelessly stolen from stackoverflow
// http://codereview.stackexchange.com/a/59998
template <typename T>
bool contains(std::vector<T> const &v, T const &x)
{ 
    return ! (v.empty() &&
		std::find(v.begin(), v.end(), x) == v.end());
}