# This is required. target system name
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# specify the cross compiler
# SET(TOOLCHAIN_PATH /work/sdk/nvidia/nvidia_6.0.1.0/toolchains/aarch64--glibc--stable-2020.08-1)
# SET(CMAKE_SYSROOT ${TOOLCHAIN_PATH}/aarch64-buildroot-linux-gnu/sysroot)
# SET(CMAKE_C_COMPILER   ${TOOLCHAIN_PATH}/bin/aarch64-linux-gcc)
# SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/aarch64-linux-g++)
SET(CMAKE_C_COMPILER   /usr/bin/gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)

# SET(CMAKE_INSTALL_PREFIX ../output)
# SET(CMAKE_TOOLCHAIN_FILE ../toolchain.cmake)
# cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake -DCMAKE_INSTALL_PREFIX=../output ..

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
