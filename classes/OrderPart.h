//
// Created by Соколов Илья on 23.05.2020.
//

#ifndef MDILIVERY_ORDERPART_H
#define MDILIVERY_ORDERPART_H
#include <list>



using namespace std;

class OrderPart{
private:
//        MapPoint toPoint;
    bool delivered = false;
public:
    int ID;
    list <pair<int, int>> path;
    std::map<string, int> products;
    pair<int, int> deliveryTo;
    pair<int, int> deliveryFrom;

    OrderPart() = default;
    OrderPart(int fromOrderID, std::map<string, int> iproducts, list <pair<int, int>> ipath, pair<int, int> ideliveryFrom,
            pair<int, int> ideliveryTo){
        ID=fromOrderID;
        products = iproducts;
        path = ipath;
        deliveryFrom = pair<int, int> (ideliveryFrom.first, ideliveryFrom.second);
        deliveryTo = ideliveryTo;
    };

    void setDeliveredStatus(bool newStatus){delivered=newStatus;};
    bool getDeliveredStatus() const {return delivered;};

    bool operator == (const OrderPart other){
        return this->path == other.path;
    };
};

#endif //MDILIVERY_ORDERPART_H
