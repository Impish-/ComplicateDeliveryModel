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
        Car(list<T> & carPath){
            this->path = carPath;
            this->current = path.front();
            this->wayTo = path.back();
            this->tickUpdated = -1;
        };
        string name;

        T goToNext(T iCurrent, T roadPoint){
            iCurrent.outOffCar(*this);
            T cur = roadPoint.incomingCar(*this);
            return cur;
        };

        void nextTick(int tickCount, map<int, map<int, T >>& items, T& placedRoad){
            this->current = placedRoad;
            if (current == wayTo){
                cout << "\t\t"<< _str() <<"Waiting in target point" <<endl;
                return;
            };
            bool next = false;
            for ( auto road : path ){
                if (next){
                    goToNext(placedRoad, items[road.XCoord] [road.YCoord]);
                    break;
                }
                if (road == placedRoad){
                    next = true;
                    continue;
                }
            }
            cout << "\t\t>>------- " << _str() << " CAR MOVIE: from  " << placedRoad._str() << " to ---->>  " << current._str() << "\n" << endl;
            tickUpdated = tickCount;

            items[current.XCoord][current.YCoord] = current;
        };
        string _str(){
            auto s = "<Car " + name + ">";
            return s;
        };
};

#endif //MDILIVERY_CAR_H
