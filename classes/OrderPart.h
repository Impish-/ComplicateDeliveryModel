//
// Created by Соколов Илья on 23.05.2020.
//

#ifndef MDILIVERY_ORDERPART_H
#define MDILIVERY_ORDERPART_H
#include <list>

using namespace std;


class OrderPart{
private:
    //    MapPoint toPoint;
    bool delivered = false;
public:
    list<int> products;
    OrderPart() = default;
    OrderPart(list<int> iproducts){
        products = iproducts;
    };
    pair<int, int> deliveryTo;
    void to_car(){};
    void setDeliveredStatus(bool newStatus){delivered=newStatus;};
};

#endif //MDILIVERY_ORDERPART_H
