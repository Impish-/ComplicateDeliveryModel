#include <iostream>
#include "classes/MapPoint.h"
#include "classes/Map.h"


using namespace std;

int main() {
    Map<MapPoint> map = Map<MapPoint>(100,100);

    //test insert
    MapPoint start_road = map.insert(10, 15);
    map.insert(11, 15);
    MapPoint end_road = map.insert(12, 15);


    Car testcar = start_road.startCar(end_road)


    map.nextTick(1);
//    map.remove(10, 15);
//    cout << "ELEM: " << map.getElement(10, 15)._str() << endl;

//
    return 0;
}
