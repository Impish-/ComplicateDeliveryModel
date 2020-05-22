//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_MAPPOINT_H
#define MDILIVERY_MAPPOINT_H

#include <string>
#include <list>
#include "RoadObject.h"
#include "Store.h"
#include <iostream>

using namespace std;


class MapPoint:RoadObject{
public:
    int XCoord;
    int YCoord;
    Store store;

    MapPoint() {XCoord=0;YCoord=0;};
    MapPoint(int x, int y){XCoord = x;YCoord = y;};
    void addStore(){

    };
    void saveTo(){};
    void load(){};


    string _str(){
        auto s = "<MapPoint X:" + to_string(XCoord) + ", Y:" + to_string(YCoord) + "> - STORE: " + store.name;
        return s;
    };

    void nextTick(int tickCount){
        cout << "\t" << _str() << " .tick handler" <<endl;
//        carsIter
    };
};

#endif //MDILIVERY_MAPPOINT_H
