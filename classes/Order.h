//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_ORDER_H
#define MDILIVERY_ORDER_H

#include "Car.h"
#include "MapPoint.h"
#include <list>


class OrderPart{
    private:
    //    MapPoint toPoint;
        bool delivered = false;
    public:
        int startDeliveryTick = -1;
        void to_car(){};
        void setDeliveredStatus(bool newStatus){delivered=newStatus;};
};

class Order {
    // Поступает от юзера,
    // генерит куски доставки и раскидывает по сторам
public:
    Order() = default;
    Order(int x, int y, int deliveryTime, int *productIds){

    };
    list<OrderPart> parts;
//    Store getNearStore(int product_id){};
    void addPartToDelivery(){};
    void genParts(){};
    void status(){}; //Процент исполеннных частей.
    void setStoresList(){};
};

#endif //MDILIVERY_ORDER_H
