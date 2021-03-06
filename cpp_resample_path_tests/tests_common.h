#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <cmath>
#include <iomanip>

#define LOG(msg) std::cout << "failed: " << (msg) << "\n";
#define LOG2(msg, value) std::cout << "failed: " << (msg) << "\n" << (#value) << " == " << (value) << "\n";
#define LOG3(msg, value, value2) std::cout << "failed: " << (msg) << "\n" << (#value) << " == " << (value) << "\n" (#value2) << " == " << (value2) << "\n";
#define ASSERT(condition) if(!(condition)) { LOG(#condition); return -1; }
#define ASSERT2(condition, value) if(!(condition)) { LOG2(#condition, value); return -1; }
#define ASSERT3(condition, value, value2) if(!(condition)) { LOG3(#condition, value, value2); return -1; }
#define APPROX_EQUAL(a, b) (std::abs(a-b) <= 1e-6)
template<class T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& arr)
{
    os << std::setprecision(6);
    os << "[";
    for (std::size_t i = 0; i < arr.size(); ++i)
    {
        if (i > 0) 
        {
            os << ", ";
        }
        os << arr[i];
    }
    os << "]";
    return os;
}


template<class... Args>
std::ostream& operator<<(std::ostream& os, const std::vector<Args...>& vec)
{
    os << std::setprecision(6);
    os << "[";
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        if (i > 0) 
        {
            os << ", ";
        }
        os << vec[i];
    }
    os << "]";
    return os;
}

template<class... Args>
std::string to_string(const std::vector<Args...>& vec)
{
    static thread_local std::stringstream s;
    s.str("");
    s << vec;
    return s.str();
}
