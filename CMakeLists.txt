cmake_minimum_required (VERSION 2.6)
project (MNIST-CPP)

set(CMAKE_VERBOSE_MAKEFILE ON)

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

FIND_LIBRARY( stbi NAMES stbi )

add_library(Matrix matrix.cpp)

add_executable(MNIST mnist.cpp)

add_executable(Tests tests.cpp)

TARGET_LINK_LIBRARIES( MNIST LINK_PUBLIC ${Boost_LIBRARIES} stbi Matrix)
TARGET_LINK_LIBRARIES( Tests LINK_PUBLIC stbi Matrix )