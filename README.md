# cpp_resample_path
A small library to resample paths


[Build Status](https://web.martin-huenermund.de/cistatus/xaedes/cpp_resample_path)

![build status linux docker ubuntu xenial x64 release](https://web.martin-huenermund.de/cistatus/xaedes/cpp_resample_path/x64_ubuntu_xenial_Release_status.svg)
![build status linux docker ubuntu bionic x64 release](https://web.martin-huenermund.de/cistatus/xaedes/cpp_resample_path/x64_ubuntu_bionic_Release_status.svg)
![build status linux docker ubuntu focal x64 release](https://web.martin-huenermund.de/cistatus/xaedes/cpp_resample_path/x64_ubuntu_focal_Release_status.svg)
![build status linux docker ubuntu jammy x64 release](https://web.martin-huenermund.de/cistatus/xaedes/cpp_resample_path/x64_ubuntu_jammy_Release_status.svg)
![build status windows x64 release](https://web.martin-huenermund.de/cistatus/xaedes/cpp_resample_path/x64_windows_Release_status.svg)

## Example

```cpp
    struct MyVecStruct
    {
        double x;
        double y;
        double z;
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

    std::vector<MyVecStruct> input_path = {MyVecStruct{0, 0, 100}, MyVecStruct{10, 0, 100}};
    std::vector<MyVecStruct> output_path;
    cpp_resample_path::resample<3,double>(
        input_path,
        0.1,
        output_path,
        ReadVec, WriteVec
    );

    // output_path is now {{0, 0, 100}, {0.1, 0, 100}, {0.2, 0, 100}, ...}
```
