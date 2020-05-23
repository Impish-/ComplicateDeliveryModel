//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_ORDER_H
#define MDILIVERY_ORDER_H

#include "Car.h"
#include "MapPoint.h"
#include <list>

template <class T>
class Order {
    public:
        int deliveryTime;
        list<pair <Store, pair<list<int>, list<T>>>> deals;
        bool finish = false;

        Order() = default;
        Order(int ideliveryTime, list<pair <Store, pair<list<int>, list<T>>>> ideals){
            deliveryTime = ideliveryTime;
            deals = ideals;
            for ( pair <Store, pair<list<int>, list<T>>> deal : deals){
                list<T> path = deal.second.second;
                deal.first.orderToSchedule(ideliveryTime, deal.second.first);
            }
        };
    //    Store getNearStore(int product_id){};

        void genParts(){};
        void status(){}; //Процент исполеннных частей.
        void setStoresList(){};
    };

#endif //MDILIVERY_ORDER_H
