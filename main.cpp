#include "classes/MapPoint.h"
#include "classes/Map.h"
#include "models/product.h"
#include <iostream>

using namespace std;


int main() {
    ns::product p {60, "Ned Flanders", "744 Evergreen Terrace", };
    json j = p;
    std::cout << j << std::endl;

    auto p2 = j.get<ns::product>();
//
//    to_json(j, p2)
//    std::cout << ns::.id << std::endl;


    Map<RoadObject> map = Map<RoadObject>(100,100);

    //test insert
    RoadObject start_road = map.insert(10, 15);
    map.insert(11, 15);
    RoadObject end_road = map.insert(12, 15);

    map.insert(11, 16);
    RoadObject second_start_road = map.insert(11, 17);
    map.insert(12, 17);
    map.insert(12, 16);

//    map.addStore(10, 15, "TestStore");
//    map.addStore(20, 15, "TestStore2");

//    map.update(11, 17, second_start_road);
//    map.update(10, 15, start_road);
//    map.update(12, 15, end_road);
//
//    list<RoadObject> path1 = map.getPathList(start_road, end_road);
//    list<RoadObject> path2 = map.getPathList(second_start_road, end_road);
//
//    Car<RoadObject> t = start_road.startCar(path1);
//    Car<RoadObject> t3 = second_start_road.startCar(path2);

    map.update(11, 17, second_start_road);
    map.update(10, 15, start_road);
    map.update(12, 15, end_road);

    int tick = 0;
//
//    cout << "TICK :" << tick <<endl;
//    map.nextTick(tick ++);
//    sleep(5);
//    cout << "TICK :" << tick <<endl;
//    map.nextTick(tick ++);
//    sleep(5);
//    cout << "TICK :" << tick <<endl;
//    map.nextTick(tick ++);
//    sleep(5);
//    cout << "TICK :" << tick <<endl;
//    map.nextTick(tick ++);
    return 0;
}
