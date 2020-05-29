cmake_minimum_required(VERSION 3.16)
project(Mdilivery)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run
˓→conan install first")
endif()

find_package(restinio CONFIG REQUIRED)



set(CMAKE_CXX_STANDARD 14)

add_executable(Mdilivery main.cpp classes/Map.h classes/Car.h
        classes/Order.h classes/Store.h classes/MapPoint.h
        classes/OrderPart.h classes/RoadObject.h  classes/exceptions.h)

target_link_libraries(Mdilivery restinio::restinio)
target_link_libraries(Mdilivery ${CONAN_LIBS})