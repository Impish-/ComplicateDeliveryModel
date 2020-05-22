//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_MAPPOINT_H
#define MDILIVERY_MAPPOINT_H

#include <string>
#include <list>
#include "Store.h"
#include "Car.h"
#include <iostream>

using namespace std;


class CException
{
public:
    void virtual PrintMessage() {}
};

class RoadPointIsFull : public CException
{
public:
    void virtual PrintMessage()
    {
        cout << "Hallo from exception"; // **
    }
};

class Barrier{
public:
    string name;
    int blockInterval;
    void changeTransmit(){};
    void setChangeTimeout(int timeout){};
};

class MapPoint{
public:
    int XCoord;
    int YCoord;
    Store store;

    void addStore(){

    };
    void saveTo(){};
    void load(){};

};


class RoadObject:MapPoint{


private:
    Barrier activeBarrier;
    int carMaxCount = 1;
    list<Car<RoadObject>> cars;
    list<Car<RoadObject>>::iterator carsIter = cars.begin();

public:
    using MapPoint::store;
    using MapPoint::XCoord;
    using MapPoint::YCoord;


    RoadObject() {XCoord=0;YCoord=0;};
    RoadObject(int x, int y){XCoord = x;YCoord = y;};

    void nearRoadMapPoints(RoadObject exclude){};

    void canIncome(){};
    void serBarrier(){};
    void deleteBarrier(){};

    void incomingCar(Car<RoadObject> incomeCar, bool startFromHere=false){
        if (cars.size() > carMaxCount){
            throw RoadPointIsFull();
        }
        cars.push_back(incomeCar);
        cout << "~" << _str() << "INSERTED CAR size("<< cars.size()<<")"<<endl;
    }

    Car<RoadObject> startCar(RoadObject endPoint){
        Car<RoadObject> newCar = Car<RoadObject>(endPoint, endPoint);
        incomingCar(newCar);
        return newCar;
    };

    string _str(){
        auto s = "<MapPoint X:" + to_string(XCoord) + ", Y:" + to_string(YCoord) + "> - STORE: " + store.name;

        return s;
    };

    void nextTick(int tickCount){
        cout << "\t" << _str() << " .tick handler" <<endl;
//        carsIter.
    };
};



#endif //MDILIVERY_MAPPOINT_H
