//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_ORDER_H
#define MDILIVERY_ORDER_H

#include "Car.h"
#include "MapPoint.h"
#include <list>


class Order {
    public:
        int deliveryTime;
        list<pair <Store, list<int>>> deals;
        bool finish = false;

        Order() = default;
        Order(int ideliveryTime, list<pair <Store, list<int>>> ideals){
            deliveryTime = ideliveryTime;
            deals = ideals;
            for ( pair <Store, list<int>> deal : deals){
                deal.first.orderToSchedule(ideliveryTime, deal.second);
            }
        };
    //    Store getNearStore(int product_id){};

        void genParts(){};
        void status(){}; //Процент исполеннных частей.
        void setStoresList(){};
    };

#endif //MDILIVERY_ORDER_H
