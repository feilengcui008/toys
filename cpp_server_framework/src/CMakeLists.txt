# project name
PROJECT(cppapp)

# version requirement
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# source files in subdirectories
# ADD_SUBDIRECTORY(src)

# then in subdirectory src we create the CMakeLists.txt
# AUX_SOURCE_DIRECTORY(. SUBDIR_SRC)
# ADD_LIBRARY(test ${SUBDIR_SRC})

# header file path
# INCLUDE_DIRECTORIES(include1 include2)

# set env variables 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# set extern libraries
# SET(LIBRARIES liba.so libb.so)

# add source files 
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

#SET(SOURCE_FILES 
#    main_test.cpp 
#    AbstractServer.cpp 
#    Connection.cpp 
#    ConnectionHandler.h
#    ConnectionProcessHandler.cpp 
#    ConnectionThreadHandler.cpp
#    ConnectionEpollHandler.cpp
#    EpollThread.cpp 
#    Request.cpp 
#    Response.cpp 
#    Thread.cpp 
#    Utils.cpp)


# gen executable file 
add_executable(./example_server ${SOURCE_FILES})
# add link libs 
# TARGET_LINK_LIBRARIES(./example_server test )
