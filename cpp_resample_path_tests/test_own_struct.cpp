
#include <string>
#include <array>
#include <sstream>
#include <iostream>

#include <cpp_resample_path/cpp_resample_path.h>
#include <cpp_resample_path/geo_to_enu.h>

#include "tests_common.h"

namespace ns_test_own_struct {

    struct MyGeoStruct
    {
        double latitude;
        double longitude;
        double altitude;
    };

    struct MyVecStruct
    {
        double x;
        double y;
        double z;
    };

} // namespace ns_test_own_struct


int test_own_struct(int argc, char* argv[])
{
    using namespace ns_test_own_struct;

    auto ReadGeo = [](auto inp, auto& arr) {
        arr[0] = inp.latitude;
        arr[1] = inp.longitude;
        arr[2] = inp.altitude;
    };
    auto WriteGeo = [](auto arr, auto& out) {
        out.latitude  = arr[0];
        out.longitude = arr[1];
        out.altitude  = arr[2];
    };

    auto ReadVec = [](auto inp, auto& arr) {
        arr[0] = inp.x;
        arr[1] = inp.y;
        arr[2] = inp.z;
    };
    auto WriteVec = [](auto arr, auto& out) {
        out.x = arr[0];
        out.y = arr[1];
        out.z = arr[2];
    };

    {
        std::vector<MyGeoStruct> input_path = {MyGeoStruct{52, 11, 100}, MyGeoStruct{52.1, 11, 100}};
        std::vector<MyGeoStruct> output_path;
        cpp_resample_path::resample<3,double>(
            input_path,
            0.1f,
            output_path,
            ReadGeo, WriteGeo
        );
        ASSERT(output_path.size() == 2);
        ASSERT(output_path[0].latitude  == 52);
        ASSERT(output_path[0].longitude == 11);
        ASSERT(output_path[0].altitude  == 100);
        ASSERT(output_path[1].latitude  == 52.1);
        ASSERT(output_path[1].longitude == 11);
        ASSERT(output_path[1].altitude  == 100);
    }

    {
        std::vector<MyVecStruct> input_path = {MyVecStruct{0, 0, 100}, MyVecStruct{0.1, 0, 100}};
        std::vector<MyVecStruct> output_path;
        cpp_resample_path::resample<3,double>(
            input_path,
            0.1f,
            output_path,
            ReadVec, WriteVec
        );
        ASSERT(output_path.size() == 2);
        ASSERT2(APPROX_EQUAL(output_path[0].x, 0),   output_path[0].x);
        ASSERT2(APPROX_EQUAL(output_path[0].y, 0),   output_path[0].y);
        ASSERT2(APPROX_EQUAL(output_path[0].z, 100), output_path[0].z);
        ASSERT2(APPROX_EQUAL(output_path[1].x, 0.1), output_path[1].x);
        ASSERT2(APPROX_EQUAL(output_path[1].y, 0),   output_path[1].y);
        ASSERT2(APPROX_EQUAL(output_path[1].z, 100), output_path[1].z);
    }


    {
        std::vector<MyGeoStruct> input_geopath = {MyGeoStruct{52, 11, 100}, MyGeoStruct{53, 11, 100}};
        std::vector<MyVecStruct> input_path;
        auto geohome = input_geopath.front();
        for(auto geo : input_geopath)
        {
            MyVecStruct enu;
            cpp_resample_path::geo_to_enu(
                enu.x, enu.y,
                geo.longitude, geo.latitude,
                geohome.longitude, geohome.latitude
            );
            enu.z = geo.altitude;
            input_path.push_back(enu);
        }
        std::vector<MyVecStruct> output_path;
        cpp_resample_path::resample<3,double>(
            input_path,
            0.1f,
            output_path,
            ReadVec, WriteVec,
            0, 0.1
        );
        ASSERT(output_path.size() == 2);
        ASSERT2(APPROX_EQUAL(output_path[0].x, 0),   output_path[0].x);
        ASSERT2(APPROX_EQUAL(output_path[0].y, 0),   output_path[0].y);
        ASSERT2(APPROX_EQUAL(output_path[0].z, 100), output_path[0].z);
        ASSERT2(APPROX_EQUAL(output_path[1].x, 0),   output_path[1].x);
        ASSERT2(APPROX_EQUAL(output_path[1].y, 0.1), output_path[1].y);
        ASSERT2(APPROX_EQUAL(output_path[1].z, 100), output_path[1].z);
    }

    return 0;
}
