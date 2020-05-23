//
// Created by Соколов Илья on 23.05.2020.
//

#ifndef MDILIVERY_ROADOBJECT_H
#define MDILIVERY_ROADOBJECT_H

#include "MapPoint.h"

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

    RoadObject() {XCoord=-1; YCoord=-1;};
    RoadObject(int x, int y){XCoord = x;YCoord = y;};


    bool operator == (const RoadObject other){
        return this->XCoord == other.XCoord && this->YCoord == other.YCoord;
    };

    Order<RoadObject> activeOrder;
    void addOrder(  int deliveryTick,
                    list<pair <Store, pair<list<int>,
                    list<RoadObject>>>> deals,
                    map<int, map<int, RoadObject >>& items){
        activeOrder = Order<RoadObject>(deliveryTick, pair<int,int> (XCoord, YCoord), deals, items);
    };

    void closeOrder(){
//            activeOrder = NULL;
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

    RoadObject startCar(list<RoadObject> carPath){
        Car<RoadObject> newCar = Car<RoadObject>(carPath);
        newCar.name = rnd();
        return incomingCar(newCar, true);
    };

    string _str(){
        auto s = "<MapPoint X:" + to_string(XCoord) + ", Y:" + to_string(YCoord) + ">";
        return s;
    };

    void nextTick(int tickCount, map<int, map<int, RoadObject >>& items){
        if(cars.size()>0){
            cout << "\t POINT HAVE ACTIONS:" << _str() << ", CARS HERE:" << cars.size()<<endl;
        }

        for ( auto car : cars ){
            if(car.current == car.wayTo){
                //// уничтожить
                cout << "\t POINT:" << _str() << ", CARS HERE:" << cars.size()<<endl;
                continue;
            };
            if(car.tickUpdated >= tickCount){
                continue;
            };
            car.nextTick(tickCount, items, *this);
        }

    };
};


#endif //MDILIVERY_ROADOBJECT_H
