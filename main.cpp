#include <iostream>
#include "classes/MapPoint.h"
#include "classes/Map.h"

using namespace std;

int main() {
    Map<RoadObject> map = Map<RoadObject>(100,100);

    //test insert
    RoadObject start_road = map.insert(10, 15);
    map.insert(11, 15);
    RoadObject end_road = map.insert(12, 15);

    // обход
    map.insert(11, 16);
    map.insert(11, 17);
    map.insert(12, 17);
    map.insert(12, 16);

    map.getPathList(start_road, end_road);

    Car<RoadObject> t = start_road.startCar(end_road);
    Car<RoadObject> t3 = start_road.startCar(end_road);
    map.update(10, 15, start_road);
    map.update(12, 15, end_road);


     map.nextTick(1);
    map.remove(10, 15);

//
    return 0;
}
