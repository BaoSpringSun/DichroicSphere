# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (myjson)

# 查找当前目录下的所有源文件
# 并将名称保存到 LIB_SRCS 变量
aux_source_directory(. LIB_SRCS)

include_directories("/usr/local/include/json")


# 添加链接库路径
# link_directories(/lib)


# target_link_libraries(mathPowerDemo3 power)


# 指定生成的链接库
# add_library (myjson ${LIB_SRCS})