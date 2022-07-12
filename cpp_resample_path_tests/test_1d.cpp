
#include <string>
#include <sstream>
#include <iostream>

#include <cpp_resample_path/cpp_resample_path.h>

#include "tests_common.h"

int test_1d(int argc, char* argv[])
{
    {
        std::vector<float> input_path = {0, 1, 3};
        std::vector<float> output_path;
        cpp_resample_path::resample<1,float>(
            input_path,
            0.5f,
            output_path,
            [](auto inp, auto& arr){ arr[0] = inp; },
            [](auto arr, auto& out){ out = arr[0]; }
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[0, 0.5, 1, 1.5, 2, 2.5, 3]");
    }
    {
        std::vector<float> input_path = {0, 1, 3};
        std::vector<float> output_path;
        cpp_resample_path::resample<1,float>(
            input_path,
            1.0f,
            output_path,
            [](auto inp, auto& arr){ arr[0] = inp; },
            [](auto arr, auto& out){ out = arr[0]; }
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[0, 1, 2, 3]");
    }
    {
        std::vector<float> input_path = {0, 1, 3};
        std::vector<float> output_path;
        cpp_resample_path::resample<1,float>(
            input_path,
            2.0f,
            output_path,
            [](auto inp, auto& arr){ arr[0] = inp; },
            [](auto arr, auto& out){ out = arr[0]; },
            0, 0, true, false
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[0, 2, 3]");
    }
    {
        std::vector<float> input_path = {0, 1, 3};
        std::vector<float> output_path;
        cpp_resample_path::resample<1,float>(
            input_path,
            2.0f,
            output_path,
            [](auto inp, auto& arr){ arr[0] = inp; },
            [](auto arr, auto& out){ out = arr[0]; },
            0, 0, true, true
        );
        std::cout << "input_path: " << input_path << "\n";
        std::cout << "output_path: " << output_path << "\n";
        ASSERT(to_string(output_path) == "[0, 2, 4]");
    }
    return 0;
}
