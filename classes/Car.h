//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_CAR_H
#define MDILIVERY_CAR_H

#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include "Store.h"
#include "exceptions.h"
#include <string>


using namespace std;

template <class T>
class Car {
    private:
        list<T> path;
        Store store;
        OrderPart * orderPart;
    public:
        string name;
        bool arrived = false;
        int tickUpdated = -1;

        T current;
        T wayTo;

        int getPartId(){
            return orderPart->ID;
        }
        bool operator == (const Car other){
            return this->name == other.name;
        };

        Car(){};
        Car(list<T> & carPath, OrderPart * orderPart, Store & store){
            this->path = carPath;
            this->current = path.front();
            this->wayTo = path.back();
            this->tickUpdated = -1;
            this->orderPart = orderPart;
            this->store = store;
        };

        void setCurrent(T& icurrent){
            current = icurrent;
        };

        string _str(){
            string returnstring = "";
            for (auto i: orderPart->products){
                returnstring += i.first+ ", ";
            };
            auto s = "<Car " + name + " products:" + returnstring +">";
            return s;
        };

        void nextTick(int tickCount, map<int, map<int, T >>& items, T& placedRoad){
            this->current = placedRoad;
            T oldRoad = items[current.XCoord][current.YCoord];

            if (current == wayTo){
//                cout << "\t\t"<< _str() <<"    -==ORDER DELIVERED==- " <<endl;
                store.delivered(orderPart);
                items[oldRoad.XCoord][oldRoad.YCoord] = oldRoad.outOffCar(*this);
                return;
            };

            bool next = false;
            for ( auto road : path ){
                if (next){
                    try{
                        current = items[road.XCoord] [road.YCoord].incomingCar(*this);
                        items[oldRoad.XCoord][oldRoad.YCoord] = oldRoad.outOffCar(*this);
                    } catch (RoadPointIsFull& ex){
//                        cout << "\t\t"<< _str() <<" CANT MOVIE, WAITING" <<endl;
                        return;
                    }
                    break;
                }
                if (road == placedRoad){
                    next = true;
                    continue;
                }
            }
//            cout << "\t\t>>------- " << _str() << " CAR MOVIE: from  " << placedRoad._str() << " to ---->>  " << current._str() << "\n" << endl;
            tickUpdated = tickCount;
            current.updateCar(*this);
            items[current.XCoord][current.YCoord] = current;
        };
    };

#endif //MDILIVERY_CAR_H
