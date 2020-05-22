//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_ROADOBJECT_H
#define MDILIVERY_ROADOBJECT_H
#include <string>
#include "Car.h"
#include <list>

using namespace std;


class Barrier{
public:
    string name;
    int blockInterval;
    void changeTransmit(){};
    void setChangeTimeout(int timeout){};
};

class RoadObject{
private:
    Barrier activeBarrier;
    list<Car> cars;
    list<Car>::iterator carsIter = cars.begin();
    int carMaxCount = 1;
public:
    void canIncome(){};
    void serBarrier(){};
    void deleteBarrier(){};
    void startCar(RoadObject endPoint){
        return Car(this, endPoint);
    };
};

#endif //MDILIVERY_ROADOBJECT_H
