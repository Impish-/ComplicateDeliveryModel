//
// Created by Соколов Илья on 23.05.2020.
//

#ifndef MDILIVERY_ORDERPART_H
#define MDILIVERY_ORDERPART_H
#include <list>
#include "MapPoint.h"

using namespace std;


class OrderPart{
private:
    //    MapPoint toPoint;
    bool delivered = false;
public:
    list<RoadObject> path;
    list<int> products;
    pair<int, int> deliveryTo;

    OrderPart() = default;
    OrderPart(list<int> iproducts, list<RoadObject> ipath){
        products = iproducts;
        path = ipath;
    };

    void setDeliveredStatus(bool newStatus){delivered=newStatus;};
};

#endif //MDILIVERY_ORDERPART_H
