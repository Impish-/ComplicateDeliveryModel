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
#include <map>


using namespace std;

static const char alphanum[] =
        "0123456789"
        "!@#$%^&*"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

char genRandom()
{

    return alphanum[rand() % stringLength];
}

string rnd(){
    string Str;
    for(unsigned int i = 0; i < 20; ++i)
    {
        Str += genRandom();

    }
    return Str;
}

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


    RoadObject() {XCoord=-1;YCoord=-1;};
    RoadObject(int x, int y){XCoord = x;YCoord = y;};

    bool operator== (const RoadObject other){
        return this->XCoord == other.XCoord && this->YCoord == other.YCoord;
    };


    void nearRoads(){

    };

    void canIncome(){};
    void serBarrier(){};
    void deleteBarrier(){};

    void outOffCar(Car<RoadObject> incomeCar){
        cout << "\t\t~ RoadObject" <<_str() << "     " << incomeCar._str() << "TRY DELETE CAR size("<< cars.size()<<")"<<endl;
        int i = 0;
        for ( auto car : cars ){
            if (incomeCar == car){
                advance(carsIter, i);
                cars.erase(carsIter);
                cout << "~" << _str() << "DELETE CAR size("<< cars.size()<<")"<<endl;
                break;
            };
            i++;
        };
    }

    Car<RoadObject> incomingCar(Car<RoadObject> incomeCar, bool startFromHere=false){
        if (cars.size() > carMaxCount){
            throw RoadPointIsFull();
        }
        cars.push_back(incomeCar);
        incomeCar.setCurrent(*this);
//        cars.push_back(incomeCar);

        cout << "\t\t~ RoadObject" << _str() << "INSERTED CAR size("<< cars.size()<<")"<<endl;
        return incomeCar;
    }

    Car<RoadObject> startCar(list<RoadObject> carPath){
        Car<RoadObject> newCar = Car<RoadObject>(carPath);
        newCar.name = rnd();
        return incomingCar(newCar, true);;
    };

    string _str(){
        auto s = "<MapPoint X:" + to_string(XCoord) + ", Y:" + to_string(YCoord) + "> - STORE: " + store.name;
        return s;
    };

    void nextTick(int tickCount, map<int, map<int, RoadObject >> items){
        if(cars.size()>0){
            cout << "\t" << _str() << "CARS HERE:" << cars.size()<<endl;
            cout << "\n" <<endl;
        }

        for ( auto car : cars ){
            car.nextTick(tickCount, items);
        }
    };
};




#endif //MDILIVERY_MAPPOINT_H
