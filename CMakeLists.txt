# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8.12)

# 项目信息
project(main)

# set 来定义 相当于#define VAR_A XXXX
# set(VAR_A XXXX)
# set(PRJNAME main)


# 设定c++标准
# add_definitions(-std=c++20)
set(CMAKE_CXX_STANDARD 20)

# 定义生成的目标文件的类型：
# Debug：用于开发和调试阶段，会启用调试信息并关闭优化。
# Release：用于发布版本，会启用优化并关闭调试信息。
# RelWithDebInfo：用于发布版本，同时保留调试信息和优化。
# MinSizeRel：用于发布版本，最小化生成的可执行文件的大小。
set(CMAKE_BUILD_TYPE Debug)


# 指定 make install 时目标文件生成的路径
set(CMAKE_INSTALL_PREFIX ../output)

# 当需要交叉编译时，所需要指定的交叉编译的工具链的编译环境的指定，如果不需要交叉编译或者用指定的编译环境，则可以不用写”xxxx.cmake“文件；
set(CMAKE_TOOLCHAIN_FILE ../toolchain.cmake)


# 设定编译参数
# set(CMAKE_CXX_FLAGS "-pthread -g -Wall -Wl,-rpath=.")
# set(CMAKE_CXX_FLAGS "-Wl,-rpath=.")

# 利用增加 BUILD_ALL 宏定义条件编译代码
add_compile_definitions(BUILD_DLL)

# 添加 math 子目录
# add_subdirectory(arrange)
# add_subdirectory(daletou)
# add_subdirectory(demo)
# add_subdirectory(myjson)
# add_subdirectory(observer)
# add_subdirectory(pailie3)
# add_subdirectory(pailie5)
# add_subdirectory(promise)
# add_subdirectory(qilecai)
# add_subdirectory(qixingcai)
# add_subdirectory(shuangseqiu)
# add_subdirectory(sqlitetbl)
# add_subdirectory(task)
# add_subdirectory(task2)
# add_subdirectory(threadpool)
# add_subdirectory(zmq)

include_directories("/usr/local/include/json")
include_directories("./arrange")
include_directories("./daletou")
include_directories("./demo")
include_directories("./myjson")
include_directories("./observer")
include_directories("./pailie3")
include_directories("./pailie5")
include_directories("./promise")
include_directories("./qilecai")
include_directories("./qixingcai")
include_directories("./shuangseqiu")
include_directories("./sqlitetbl")
include_directories("./task")
include_directories("./task2")
include_directories("./threadpool")
include_directories("./zmq")

# 查找当前目录 A 下的所有源文件
# 并将名称保存到 DIR_SRCS_A 变量;中间有个空格
# aux_source_directory(./A/ DIR_SRCS_A)
aux_source_directory(. DIR_SRCS)

aux_source_directory(./arrange arrange)
aux_source_directory(./daletou daletou)
aux_source_directory(./demo demo)
aux_source_directory(./myjson myjson)
aux_source_directory(./observer observer)
aux_source_directory(./pailie3 pailie3)
aux_source_directory(./pailie5 pailie5)
aux_source_directory(./promise promise)
aux_source_directory(./qilecai qilecai)
aux_source_directory(./qixingcai qixingcai)
aux_source_directory(./shuangseqiu shuangseqiu)
aux_source_directory(./sqlitetbl sqlitetbl)
aux_source_directory(./task task)
aux_source_directory(./task2 task2)
aux_source_directory(./threadpool threadpool)
aux_source_directory(./zmq zmq)

# 添加连接库路径
link_directories(/lib)
link_directories(/usr/local/lib)

# 指定生成目标
add_executable( ${PROJECT_NAME}
                main.cpp
                ${arrange}
                ${daletou}
                ${demo}
                ${myjson}
                ${observer}
                ${pailie3}
                ${pailie5}
                ${promise}
                ${qilecai}
                ${qixingcai}
                ${shuangseqiu}
                ${sqlitetbl}
                ${task}
                ${task2}
                ${threadpool}
                ${zmq}
                )

# 添加链接库
# target_link_libraries(Demo MathFunctions)
target_link_libraries(${PROJECT_NAME} jsoncpp)
target_link_libraries(${PROJECT_NAME} sqlite3)
