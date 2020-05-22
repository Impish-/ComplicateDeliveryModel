//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_ORDER_H
#define MDILIVERY_ORDER_H

#include "Store.h"
#include "Car.h"
#include <list>


class OrderPart{
    private:
        Store fromStore;
    //    MapPoint toPoint;
        Car car;
        bool delivered = false;
    public:
        void setDeliveredStatus(bool newStatus){delivered=newStatus;};

};

class Order {
public:
    Order():{};
    list<OrderPart> parts;
    Store getNearStore(int product_id){};
    void addPartToDelivery(){};
    void genParts(){};
    void status(){}; //Процент исполеннных частей.
};

#endif //MDILIVERY_ORDER_H
