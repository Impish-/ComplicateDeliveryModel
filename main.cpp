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
    Car<RoadObject> t = start_road.startCar(end_road);
    Car<RoadObject> t3 = start_road.startCar(end_road);


     map.nextTick(1);
//    map.remove(10, 15);
//    cout << "ELEM: " << map.getElement(10, 15)._str() << endl;

//
    return 0;
}
