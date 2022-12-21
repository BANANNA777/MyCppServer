cmake_minimum_required(VERSION 2.8.0)  #声明使用的cmake最小版本（要小于我们linux安装的那个版本）
project(Socket)                     #给C++项目起个名字（ADD）
include_directories(inc)     #声明项目的头文件的目录
aux_source_directory(./src SRC_FILE)#把目录下的所有源文件都添加到源列表变量里面去
add_executable(main_cmake ${SRC_FILE})  #将main.cpp和src/add.cpp编译链接成main_cmake可执行文件

add_compile_options(-Wall -std=c++11 -g)
set(CMAKE_BUILD_TYPE "Debug")
#add_executable(main_cmake server.cppsrc/Server.cpp src/Acceptor.cpp src/Channel.cpp src/Connection.cppsrc/Epoll.cpp src/EventLoop.cpp  src/Socket.cpp src/SockAddress.cpp src/util.cpp) 