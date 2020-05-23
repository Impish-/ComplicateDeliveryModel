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
        Order(int ideliveryTime, list<pair <Store, pair<list<int>, list<T>>>> ideals,  map<int, map<int, T >>& items){
            deliveryTime = ideliveryTime;
            deals = ideals;
            for ( pair <Store, pair<list<int>, list<T>>> deal : deals){
                list<T> path = deal.second.second;
                Store store = deal.first.orderToSchedule(ideliveryTime, deal.second.first);
                items[store.coords.first][store.coords.second].store = store;
            }
        };
    //    Store getNearStore(int product_id){};

        void genParts(){};
        void status(){}; //Процент исполеннных частей.
        void setStoresList(){};
    };

#endif //MDILIVERY_ORDER_H
