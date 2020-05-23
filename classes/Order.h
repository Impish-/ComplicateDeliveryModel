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
                Store store = deal.first;
                list<T> path = deal.second.second;
                pair<int, int> from;

                bool getfirst = true;
                for (auto x: path){
                    if(getfirst){
                        from = pair<int, int> (x.XCoord, x.YCoord); getfirst = false;
                    };
                    pathCoords.push_back(pair<int, int> (x.XCoord, x.YCoord));
                }

                OrderPart orderPart = OrderPart(deal.second.first, pathCoords, from, orderToCoords);
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
