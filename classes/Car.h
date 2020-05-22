//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_CAR_H
#define MDILIVERY_CAR_H


#include <iterator>
#include <list>
#include <map>


using namespace std;

template <class T>
class Car {
    private:

        list<T> path;
        Store owner;

    public:
//        void setCurrent(T icurrent){
//            current = icurrent;
//        };
        T current;
        bool operator == (const Car other){
            return this->name == other.name;
        };

        Car(){};
        Car(list<T> carPath){
            path = carPath;
            current = path.front();
        };
        string name;

        void goToNext(T iCurrent, T roadPoint){
            iCurrent.outOffCar(*this);
            roadPoint.incomingCar(*this);
        }

        void nextTick(int tickCount, map<int, map<int, T >> items, T currentRoad){
            this->current = currentRoad;
            bool next = false;
            for ( auto road : path ){
                if (next){
                    cout << "------->>>\t\t" << "My next point" << currentRoad._str() << endl;
                    goToNext(currentRoad, items[road.XCoord] [road.YCoord]);
                    break;
                }
                if (road == currentRoad){
                    cout << ">>-------\t\t" << "My current point" << road._str() << endl;
                    next = true;
                }
            }

        };
        string _str(){
            auto s = "<Car " + name + ">";
            return s;
        };





};

#endif //MDILIVERY_CAR_H
