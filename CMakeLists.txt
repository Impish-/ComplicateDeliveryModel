cmake_minimum_required(VERSION 3.16)
project(Mdilivery)

set(CMAKE_CXX_STANDARD 14)

add_executable(Mdilivery main.cpp classes/Map.h classes/Car.h classes/Order.h classes/Store.h classes/MapPoint.h models/product.h classes/OrderPart.h)