//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_CAR_H
#define MDILIVERY_CAR_H

#include <iostream>
#include <iterator>
#include <list>
#include <map>


using namespace std;

template <class T>
class Car {
    private:
        list<T> path;
//        Store owner;
    public:
        bool arrived = false;

        int tickUpdated = -1;
        void setCurrent(T& icurrent){
            current = icurrent;
        };
        T current;
        T wayTo;
        bool operator == (const Car other){
            return this->name == other.name;
        };

        Car(){};
        Car(list<T> carPath){
            path = carPath;
            current = path.front();
            wayTo = path.back();
            tickUpdated = -1;
        };
        string name;

        T goToNext(T iCurrent, T roadPoint){
            iCurrent.outOffCar(*this);
            T cur = roadPoint.incomingCar(*this);
            return cur;
        };

        void nextTick(int tickCount, map<int, map<int, T >>& items, T& currentRoad){
            bool next = false;
            for ( auto road : path ){
                if (road == wayTo){
                    cout << ">>-------\t\t" << "ARRIVED  " << _str() << " "<< road._str() << endl;
                    arrived = true;
                    break;
                }

                if (next){
                    cout << "------->>>\t\t" << "My next point" <<items[road.XCoord] [road.YCoord]._str()<< _str() << endl;
                    goToNext(currentRoad, items[road.XCoord] [road.YCoord]);
                    break;
                }
                if (road == currentRoad){
                    cout << ">>-------\t\t" << "My current point" << road._str() << _str() << endl;
                    next = true;
                    continue;
                }
            }
            tickUpdated = tickCount;
            items[current.XCoord][current.YCoord] = current;
        };
        string _str(){
            auto s = "<Car " + name + ">";
            return s;
        };
};

#endif //MDILIVERY_CAR_H
