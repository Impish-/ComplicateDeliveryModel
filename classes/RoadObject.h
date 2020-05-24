//
// Created by Соколов Илья on 23.05.2020.
//

#ifndef MDILIVERY_ROADOBJECT_H
#define MDILIVERY_ROADOBJECT_H

#include "MapPoint.h"
#include "../json.hpp"

using nlohmann::json;


class RoadObject: MapPoint{
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
    using MapPoint::MapPoint;
    using MapPoint::setCoords;

    RoadObject() {XCoord=-1; YCoord=-1;};
    RoadObject(int x, int y){XCoord = x;YCoord = y; activeOrder = NULL;};


    bool operator == (const RoadObject other){
        return this->XCoord == other.XCoord && this->YCoord == other.YCoord;
    };

    Order<RoadObject> * activeOrder;
    void addOrder(  int deliveryTick,
                    list<pair<list<int>,
                    list<RoadObject>>> deals,
                    map<int, map<int, RoadObject >>& items){
        activeOrder = new Order<RoadObject>(deliveryTick, pair<int,int> (XCoord, YCoord), deals, items);
    };

    void closeOrder(){
//            activeOrder = NULL;
    };

    void nearRoads(){

    };

    void canIncome(){};
    void serBarrier(){};
    void deleteBarrier(){};

    bool carExist(Car<RoadObject> checkingCar){
        for ( auto car : cars ){
            if (checkingCar == car){
                return true;
            };
        };
        return false;
    };

//    Car<RoadObject> getCar(Car<RoadObject> checkingCar){
//        for ( auto car : cars ) {
//            if (checkingCar == car) {
//                return car;
//            };
//        };
//    };

    void updateCar(Car<RoadObject> checkingCar){
        list<Car<RoadObject>> newCars;
        for ( auto car : cars ){
            if (checkingCar == car){
                newCars.push_back(checkingCar);

            }else{newCars.push_back(car);}

            cars = newCars;
            return;
        };
    };

    void leftCar(Car<RoadObject> outCar){
        list<Car<RoadObject>> newCars;
        for ( auto car : cars ){
            if (outCar == car){
                continue;
            };
            newCars.push_back(car);
        };
        cars = newCars;
    }

    RoadObject outOffCar(Car<RoadObject> outCar){
        leftCar(outCar);
        return *this;
    }

    RoadObject incomingCar(Car<RoadObject>& incomeCar, bool startFromHere=false){
        if (cars.size() > carMaxCount){
            throw RoadPointIsFull();
        }
        if(!carExist(incomeCar)) cars.push_back(incomeCar); // if not exist
        incomeCar.setCurrent(*this);
//        cars.push_back(incomeCar);

//        cout << "\t\t~ RoadObject" << _str() << "INSERTED CAR size("<< cars.size()<<")"<<endl;
        return *this;
    }

    RoadObject startCar(list<RoadObject> carPath, OrderPart & orderPart, Store & store){
        Car<RoadObject> newCar = Car<RoadObject>(carPath, orderPart, store);
        newCar.name = rnd();
        return incomingCar(newCar, true);
    };

    bool haveActions(int tickCount){
        bool result = true;
        for ( auto car : cars ){
            result = result * (car.tickUpdated == tickCount);
        };
        return result;
    }

    string _str() {
        auto s = "<MapPoint X:" + to_string(XCoord) + ", Y:" + to_string(YCoord) + ">";
        return s;
    };


    void nextTick(int tickCount, map<int, map<int, RoadObject >>& items){
        if ((cars.size() < 1)) return;
        if (haveActions(tickCount)) return;
        cout << "\t POINT HAVE ACTIONS:" << _str() << ", CARS HERE:" << cars.size()<<endl;

        for ( auto car : cars ){
            if (car.tickUpdated == tickCount){
                continue;
            }

            if(car.tickUpdated >= tickCount){
                continue;
            };
            car.nextTick(tickCount, items, *this);
        };
    };
    json serialize(){
        json point;
        point["x"] = this->XCoord;
        point["y"] = this->YCoord;
        point["cars"] = json::array();

        if (this->activeOrder != NULL) (point["order"] = this->activeOrder->serialize());

        if (this->store != NULL){point["store"] = this->store->serialize();}

        for (auto car: this->cars){
            point["cars"].push_back(car.name);
        };
        return point;
    }
};


#endif //MDILIVERY_ROADOBJECT_H
