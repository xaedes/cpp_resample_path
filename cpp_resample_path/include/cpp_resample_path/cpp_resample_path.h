// MIT License
// 
// Copyright (c) 2022 xaedes
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#pragma once

#include <type_traits>
#include <vector>
#include <array>
#include <cmath>

namespace cpp_resample_path {

    template<
        std::size_t N,
        class F,
        class T,
        class U,
        class RI,
        class MO
    >
    std::size_t resample(
        const std::vector<T>& input_path, 
        F resample_interval,
        std::vector<U>& output_path,
        RI read_input,
        MO make_output,
        F start = 0, 
        F end = 0, // if end <= 0 total length is added
        bool true_ceil_false_floor = true,
        bool true_extrapolate_false_border = false,
        F eps = 1e-9
    )
    {
        static_assert(std::is_floating_point<F>::value == true, "std::is_floating_point<F>::value == true");
        using I = std::size_t;
        using V = std::array<F,N>;

        auto Length2 = [](const F* ptr){
            F result = 0;
            for(I i=0; i<N; ++i) {
                result += ptr[i]*ptr[i];
            }
            return result;
        };
        auto Length = [&Length2](const F* ptr){
            return sqrt(Length2(ptr));
        };

        auto SubV = [](const F* a, const F* b, F* result){
            for(I i=0; i<N; ++i) {
                result[i] = a[i] - b[i];
            }
        };
        auto MulVF = [](const F* a, F b, F* result){
            for(I i=0; i<N; ++i) {
                result[i] = a[i] * b;
            }
        };
        auto DivFOr = [eps](F a, F b, F when_b_zero) {
            return (std::abs(b) < eps) ? when_b_zero : (a/b);
        };

        const auto input_size = input_path.size();

        if (input_size == 0)
        {
            return 0;
        }

        if (input_size == 1)
        {
            V input_array;
            U output;
            read_input(input_path.front(), input_array);
            make_output(input_array, output);
            output_path.push_back(output);
            return 1;
        }

        using FCPtr = const F*;
        using FPtr = F*;

        static thread_local std::vector<V> s_input_array;
        static thread_local std::vector<FCPtr> s_input;

        static thread_local std::vector<V> s_diff_arrs;
        static thread_local std::vector<FPtr> s_diffs;

        static thread_local std::vector<V> s_dir_arrs;
        static thread_local std::vector<FPtr> s_dirs;

        static thread_local std::vector<F> s_cumlen;
        static thread_local std::vector<F> s_distances;

        s_input_array.clear();
        s_input.clear();

        s_diff_arrs.clear();
        s_diffs.clear();

        s_dir_arrs.clear();
        s_dirs.clear();

        s_cumlen.clear();
        s_distances.clear();

        s_input_array.resize(input_size);
        s_input.resize(input_size);

        s_diff_arrs.resize(input_size);
        s_diffs.resize(input_size);

        s_dir_arrs.resize(input_size);
        s_dirs.resize(input_size);

        s_cumlen.resize(input_size);
        s_distances.resize(input_size-1);

        auto* input_array = s_input_array.data();
        auto* input = s_input.data();

        auto* diff_arrs = s_diff_arrs.data();
        auto* diffs = s_diffs.data();

        auto* dir_arrs = s_dir_arrs.data();
        auto* dirs = s_dirs.data();

        auto* cumlen = s_cumlen.data();
        auto* distances = s_distances.data();

        auto* in_path = input_path.data();

        for(I i=0; i<input_size; ++i)
        {
            read_input(in_path[i], input_array[i]);
            input[i] = input_array[i].data();
            diffs[i] = diff_arrs[i].data();
            dirs[i] = dir_arrs[i].data();
        }

        cumlen[0] = 0;
        for(I i=0; i<input_size-1; ++i)
        {
            SubV(input[i+1], input[i], diffs[i]);
            distances[i] = Length(diffs[i]);
            F norm = DivFOr(1, distances[i], 0);
            MulVF(diffs[i], norm, dirs[i]);
            cumlen[i+1] = cumlen[i] + distances[i];
        }

        F total_length = cumlen[input_size-1];

        if (total_length > 0)
        {
            while (end <= 0) end += total_length;
        }

        if (end < start) end = start;
        F output_length = end - start;

        // std::cout << "----------------------------------------------------\n";
        // std::cout << "eps: " << eps << "\n";
        // std::cout << "input_size: " << input_size << "\n";
        // std::cout << "start: " << start << "\n";
        // std::cout << "end: " << end << "\n";
        // std::cout << "output_length: " << output_length << "\n";
        // std::cout << "resample_interval: " << resample_interval << "\n";

        auto CeilOrFloor = [true_ceil_false_floor](auto v)
        {
            return true_ceil_false_floor ? std::ceil(v) : std::floor(v);
        };

        I num_output_items = 1 + static_cast<I>(CeilOrFloor(DivFOr(output_length, resample_interval, 0)));

        // std::cout << "num_output_items: " << num_output_items << "\n";

        V resampled_arr;
        U resampled_output;
        F* resampled = resampled_arr.data();

        I segment = 0;
        // std::cout << "------------\n";
        for (I i=0; i<num_output_items; ++i)
        {
            F here = i * resample_interval;
            // std::cout << "here: " << here<< "\n";
            while (
                (segment+1 < input_size-1)
                 && (here > cumlen[segment+1])
            )
            {
                ++segment;
            }
            // std::cout << "segment: " << segment<< "\n";
            // std::cout << "cumlen[segment]: " << cumlen[segment]<< "\n";
            // std::cout << "cumlen[segment+1]: " << cumlen[segment+1]<< "\n";
            // std::cout << "distances[segment]: " << distances[segment]<< "\n";
            F dist = distances[segment];
            F here_in_seg = here - cumlen[segment];
            if ((true_extrapolate_false_border == false) && (here_in_seg > dist)) 
            {
                here_in_seg = dist;
            }

            // std::cout << "here_in_seg: " << here_in_seg<< "\n";
            // F k = DivFOr(here_in_seg, dist, 1);
            
            // // std::cout << "k: " << k<< "\n";

            const F* input_segment = input[segment];
            const F* dir_segment = dirs[segment];

            for (I j=0; j<N; ++j)
            {
                resampled[j] = input_segment[j] + dir_segment[j] * here_in_seg;
                // std::cout << "input_segment[j]: " << input_segment[j]<< "\n";
                // std::cout << "dir_segment[j]: " << dir_segment[j]<< "\n";
                // std::cout << "resampled[j]: " << resampled[j]<< "\n";
            }
            make_output(resampled, resampled_output);
            output_path.push_back(resampled_output);
            // std::cout << "---\n";
        }

        return num_output_items;
    }

} // namespace cpp_resample_path
