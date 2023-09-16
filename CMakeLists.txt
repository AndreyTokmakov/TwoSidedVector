cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(DVector)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -pthread")
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

if(Boost_VERSION LESS 106500)
    message(STATUS "Boost version: Boost_VERSION=${Boost_VERSION}")
    message(FATAL "Required Boost version >= 1.65.0")
endif()
message (STATUS "BOOST VERSION: ${Boost_VERSION}")

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_compile_options(-c -Wall -Werror -Wextra -O3 -std=c++23)

# include all components
add_executable(DVector
        main.cpp
        DVector.h
)

TARGET_LINK_LIBRARIES(DVector boost_unit_test_framework)