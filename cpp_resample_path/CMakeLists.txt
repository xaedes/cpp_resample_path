cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME cpp_resample_path)
project( ${PROJECT_NAME} )


add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_14)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
