cmake_minimum_required(VERSION 3.22)

# --- FIX FOR "CMP0146" WARNING ---
# Suppress the developer warning about the FindCUDA module
cmake_policy(SET CMP0146 OLD)
# --- END FIX ---

# ====================================================================
# --- FIX FOR "Unsupported gpu architecture" ERROR ---
# We explicitly set the architecture to match the "GeForce GTX 1060", which is 6.1 (Pascal).
set(CMAKE_CUDA_ARCHITECTURES 61)
# ====================================================================

# --- FIX: Enable CXX first to identify MSVC ---
project(Fast_Tries_and_Radix_Sort LANGUAGES CXX)

# Set standards for both languages
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# ====================================================================
# --- FIX: Explicitly setting the CUDA Toolkit Root to CUDA 11.8 ---
# This ensures CMake finds the toolkit components (bin, include, lib) from the correct version.
set(CUDAToolkit_ROOT "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8" CACHE PATH "Root directory for CUDA Toolkit installation")
# ====================================================================

# Find the CUDA toolkit
# This will now use the path provided in CUDAToolkit_ROOT
find_package(CUDAToolkit REQUIRED)

# ====================================================================
# --- FIX: Set CXX warning flags ---
if(MSVC)
    # MSVC /WX treats warnings as errors. /W3 is a common warning level.
    set(HOST_COMPILER_WARNING_FLAGS "/W3")
else()
    # GCC/Clang flags for Gradescope
    set(HOST_COMPILER_WARNING_FLAGS "-Wall -Werror")
endif()

# Safely append flags to CXX_FLAGS (avoids leading space)
if(CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${HOST_COMPILER_WARNING_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "${HOST_COMPILER_WARNING_FLAGS}")
endif()

# --- FIX: Set CUDA flags *BEFORE* enabling the language ---
# This ensures the -allow-unsupported-compiler flag is used
# during CMake's initial compiler check.
if(MSVC)
    if(CMAKE_CUDA_FLAGS)
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcompiler \"/W3\" -allow-unsupported-compiler")
    else()
        set(CMAKE_CUDA_FLAGS "-Xcompiler \"/W3\" -allow-unsupported-compiler")
    endif()
else()
    if(CMAKE_CUDA_FLAGS)
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcompiler \"-Wall -Werror\"")
    else()
        set(CMAKE_CUDA_FLAGS "-Xcompiler \"-Wall -Werror\"")
    endif()
endif()
# ====================================================================

# --- FIX: Enable CUDA language *after* flags are set ---
enable_language(CUDA)


##pull in desired version of catch through cmake automatically, make it available
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.2 # if you run into issues, try updating your editor or downgrading to a lower version of catch v3
)
FetchContent_MakeAvailable(Catch2)

include_directories(src)

add_executable(Main
        src/main.cpp # your main file
        # add your own header files below - should be automatically added in CLion
        # example (can also separate with newlines):
        # src/AVL.h src/AVL.cpp
)

# These tests can use the Catch2-provided main
add_executable(.Test
        test/test.cpp # your test file
        # add your own header files below - should be automatically added in CLion
        # example (can also separate with newlines):
        # src/AVL.h src/AVL.cpp
)
target_include_directories(Main PRIVATE "submodules/FunnelHashMap/src")
target_include_directories(.Test PRIVATE "submodules/FunnelHashMap/src")
target_link_libraries(.Test PRIVATE Catch2::Catch2WithMain) #link catch to test.cpp file
# the name here must match that of your testing executable (the one that has test.cpp)

# --- ADDED GPU TEST RUNNER ---
add_executable(gpu_test_runner
        submodules/FunnelHashMap/test/GPU_Funnel_Hash_Map_test.cu
)

# --- FIX: ADD THIS LINE ---
# Tell this specific target where to find the FunnelHashMap headers
target_include_directories(gpu_test_runner PRIVATE
        "submodules/FunnelHashMap/src"
)
# --- END FIX ---

# Link the GPU test runner against Catch2 and the CUDA Runtime
target_link_libraries(gpu_test_runner PRIVATE
        Catch2::Catch2WithMain
        CUDA::cudart
)
# --- END GPU TEST RUNNER ---


# comment everything below out if you are using CLion
#include(CTest)
#include(Catch)
#catch_discover_tests(Tests) # must be named the same as your test executable