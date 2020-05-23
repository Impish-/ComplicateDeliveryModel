//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_ORDER_H
#define MDILIVERY_ORDER_H

#include "Car.h"
#include "MapPoint.h"
#include "Store.h"
#include <list>

template <class T>
class Order {
    public:
        int deliveryTime;
        list<pair <Store, pair<list<int>, list<T>>>> deals;
        bool finish = false;

        Order() = default;
        Order(int ideliveryTime, pair<int,int> orderToCoords, list<pair <Store, pair<list<int>, list<T>>>> ideals,  map<int, map<int, T >>& items){
            deliveryTime = ideliveryTime;
            deals = ideals;
            list <pair<int, int>> pathCoords;
            for (pair <Store, pair<list<int>, list<T>>> deal : deals){
                list<T> path = deal.second.second;
                for (auto x: path){
                    pathCoords.push_back(pair<int, int> (x.XCoord, x.YCoord));
                }
                Store store = deal.first;
                OrderPart orderPart = OrderPart(deal.second.first, pathCoords, store.coords, orderToCoords );
                store.orderToSchedule(ideliveryTime, orderPart);
                items[store.coords.first][store.coords.second].store = store;
            }
        };
    //    Store getNearStore(int product_id){};

        void genParts(){};
        void status(){}; //Процент исполеннных частей.
        void setStoresList(){};
    };

#endif //MDILIVERY_ORDER_H
