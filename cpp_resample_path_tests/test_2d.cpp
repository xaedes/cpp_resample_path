
#include <string>
#include <array>
#include <sstream>
#include <iostream>

#include <cpp_resample_path/cpp_resample_path.h>

#include "tests_common.h"

int test_2d(int argc, char* argv[])
{
    using V = std::array<float, 2>;
    {
        std::vector<V> input_path = {V{0,0}, V{1,0}, V{3,0}};
        std::vector<V> output_path;
        cpp_resample_path::resample<2,float>(
            input_path,
            0.5f,
            output_path,
            [](auto inp, auto& arr){ arr = inp; },
            [](auto arr, auto& out){ for (int i=0; i<2; ++i) { out[i] = arr[i]; } }
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[[0, 0], [0.5, 0], [1, 0], [1.5, 0], [2, 0], [2.5, 0], [3, 0]]");
    }
    {
        std::vector<V> input_path = {V{0,0}, V{1,0}, V{3,0}};
        std::vector<V> output_path;
        cpp_resample_path::resample<2,float>(
            input_path,
            1.0f,
            output_path,
            [](auto inp, auto& arr){ arr = inp; },
            [](auto arr, auto& out){ for (int i=0; i<2; ++i) { out[i] = arr[i]; } }
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[[0, 0], [1, 0], [2, 0], [3, 0]]");
    }
    {
        std::vector<V> input_path = {V{0,0}, V{1,0}, V{3,0}};
        std::vector<V> output_path;
        cpp_resample_path::resample<2,float>(
            input_path,
            2.0f,
            output_path,
            [](auto inp, auto& arr){ arr = inp; },
            [](auto arr, auto& out){ for (int i=0; i<2; ++i) { out[i] = arr[i]; } },
            0, 0, true, false
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[[0, 0], [2, 0], [3, 0]]");
    }
    {
        std::vector<V> input_path = {V{0,0}, V{1,0}, V{3,0}};
        std::vector<V> output_path;
        cpp_resample_path::resample<2,float>(
            input_path,
            2.0f,
            output_path,
            [](auto inp, auto& arr){ arr = inp; },
            [](auto arr, auto& out){ for (int i=0; i<2; ++i) { out[i] = arr[i]; } },
            0, 0, true, true
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[[0, 0], [2, 0], [4, 0]]");
    }
    {
        std::vector<V> input_path = {V{0,0}, V{1,1}, V{2,2}};
        std::vector<V> output_path;
        cpp_resample_path::resample<2,float>(
            input_path,
            1.0f,
            output_path,
            [](auto inp, auto& arr){ arr = inp; },
            [](auto arr, auto& out){ for (int i=0; i<2; ++i) { out[i] = arr[i]; } },
            0, 0, true, false
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[[0, 0], [0.707107, 0.707107], [1.41421, 1.41421], [2, 2]]");
    }
    {
        std::vector<V> input_path = {V{0,0}, V{1,1}, V{2,2}};
        std::vector<V> output_path;
        cpp_resample_path::resample<2,float>(
            input_path,
            0.5f,
            output_path,
            [](auto inp, auto& arr){ arr = inp; },
            [](auto arr, auto& out){ for (int i=0; i<2; ++i) { out[i] = arr[i]; } },
            0, 0, true, false
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[[0, 0], [0.353553, 0.353553], [0.707107, 0.707107], [1.06066, 1.06066], [1.41421, 1.41421], [1.76777, 1.76777], [2, 2]]");
    }

    return 0;
}
