#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <array>

#define LOG(msg) std::cout << "failed: " << (msg) << "\n";
#define LOG2(msg, value) std::cout << "failed: " << (msg) << "\n" << (#value) << " == " << (value) << "\n";
#define LOG3(msg, value, value2) std::cout << "failed: " << (msg) << "\n" << (#value) << " == " << (value) << "\n" (#value2) << " == " << (value2) << "\n";
#define ASSERT(condition) if(!(condition)) { LOG(#condition); return -1; }
#define ASSERT2(condition, value) if(!(condition)) { LOG2(#condition, value); return -1; }
#define ASSERT3(condition, value, value2) if(!(condition)) { LOG3(#condition, value, value2); return -1; }

template<class T, int N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& arr)
{
    os << "[";
    for (int i = 0; i < arr.size(); ++i)
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
    os << "[";
    for (int i = 0; i < vec.size(); ++i)
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
