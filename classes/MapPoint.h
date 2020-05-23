//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_MAPPOINT_H
#define MDILIVERY_MAPPOINT_H

#include <string>
#include <list>

#include "Car.h"
#include <iostream>
#include <map>
#include "Store.h"
#include "Order.h"

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
        Order activeOrder;

        void addStore(string name, list<int> iproductIds){
            store = Store(name, iproductIds);
        };

        void addOrder(int deliveryTick, list<pair <Store, list<int>>> deals){
            activeOrder = Order(deliveryTick, deals);
        };
        void closeOrder(){
//            activeOrder = NULL;
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
    using MapPoint::addStore;
    using MapPoint::addOrder;
    using MapPoint::closeOrder;

    RoadObject() {XCoord=-1; YCoord=-1;};
    RoadObject(int x, int y){XCoord = x;YCoord = y;};

    bool operator== (const RoadObject other){
        return this->XCoord == other.XCoord && this->YCoord == other.YCoord;
    };


    void nearRoads(){

    };

    void canIncome(){};
    void serBarrier(){};
    void deleteBarrier(){};

    RoadObject outOffCar(Car<RoadObject> outCar){
//        cout << "\t\t~ RoadObject" <<_str() << "     " << outCar._str() << "TRY DELETE CAR size("<< cars.size()<<")"<<endl;
        list<Car<RoadObject>> newCars;

        for ( auto car : cars ){
            if (outCar == car){
                continue;
            };
            newCars.push_back(car);

        };
        cars = newCars;
        return *this;
    }

    RoadObject incomingCar(Car<RoadObject>& incomeCar, bool startFromHere=false){
        if (cars.size() > carMaxCount){
            throw RoadPointIsFull();
        }
        cars.push_back(incomeCar);
        incomeCar.setCurrent(*this);
//        cars.push_back(incomeCar);

//        cout << "\t\t~ RoadObject" << _str() << "INSERTED CAR size("<< cars.size()<<")"<<endl;
        return *this;
    }

    Car<RoadObject> startCar(list<RoadObject> carPath){
        Car<RoadObject> newCar = Car<RoadObject>(carPath);
        newCar.name = rnd();
        incomingCar(newCar, true);
        return newCar;
    };

    string _str(){
        auto s = "<MapPoint X:" + to_string(XCoord) + ", Y:" + to_string(YCoord) + ">";
        return s;
    };

    void nextTick(int tickCount, map<int, map<int, RoadObject >>& items){
        if(cars.size()>0){
            cout << "\t" << _str() << "CARS HERE:" << cars.size()<<endl;
            cout << "\n" <<endl;
        }

        for ( auto car : cars ){
            if(car.arrived){
                //// уничтожить
                continue;
            };
            if(car.tickUpdated >= tickCount){
                continue;
            };
            car.nextTick(tickCount, items, *this);
        }
        store.nextTick(tickCount);
    };
};




#endif //MDILIVERY_MAPPOINT_H
