CC := g++
#以下是指定编译需要的头文件
CFLAGS := -g -Wall -O0 -std=c++20#多个不同文件路径的话后面可以继续添加
CFLAGS += -I include
CFLAGS += -I/usr/local/include/json


#以下是源文件
SRCS = test.cpp sqlitetbl.cpp myjson.cpp #多个不同文件的话后面可以加空格继续添加
#以下是指定需要的库文件
LIBS := -L/lib -lsqlite3 #多个不同文件路径下的文件的话后面可以加空格继续添加
LIBS += -L/usr/local/lib -ljsoncpp

#以下是指定目标文件 所有当.c文件变成.o文件
OBJS = $(SRCS:.cpp=.o)
#以下是生成可执行文件
EXECUTABLE = test

.PHONY:all clean

#make all 执行生成可执行文件
# 1编译器 2编译选项 3输出 4生成的可执行文件 5需要的源文件 6需要当库文件
all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRCS) $(LIBS)

#make clean 删除所有的.o文件 和生成的可以执行文件
clean:
	rm -f $(OBJS) $(EXECUTABLE) mydb