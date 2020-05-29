#ifndef MDILIVERY_STORE_H
#define MDILIVERY_STORE_H

#include <string>
#include <list>
#include <set>
#include <iostream>
#include "OrderPart.h"
#include "../json.hpp"

using nlohmann::json;


using namespace std;

class Store{
    public:
        string name;
        list<pair<int, OrderPart>> schedule;
        list<OrderPart> deliveryNotify;
        list<int> productIds;
        pair<int, int> coords;

        Store() = default;
        Store(string iName,list<int> iproductIds, pair<int, int> icoords){
            name = iName;
            productIds = iproductIds;
            coords = icoords;
        };

        Store orderToSchedule(int ideliveryTime, OrderPart orderPart){
            schedule.push_back(pair<int, OrderPart> (ideliveryTime, orderPart));
            return *this;
        };

        void delivered(OrderPart & part){
            list<pair<int, OrderPart>>::iterator it = schedule.begin();
            while (it != schedule.end()) {
                if ((*it).second == part) {
                    it = schedule.erase(it);
                } else
                    it++;
            }
            part.setDeliveredStatus(true);
        };

        bool checkSingleProductAvailable(int productID){
            list<int>::iterator it;
            it = std::find(productIds.begin(), productIds.end(), productID);
            return (it != productIds.end());
        }

        list<int> checkProduct(list<int> orderProductIds){
            list<int> result;
            for (auto product : orderProductIds){
                if(checkSingleProductAvailable(product))
                    result.push_back(product);
            }
            return result;
        };

    list<OrderPart> nextTick(int tickCount){
        list<OrderPart> ordersToDelivery;
        for ( auto part : schedule ){
            if (part.first == tickCount){
                part.second.deliveryFrom = coords;
                ordersToDelivery.push_back(part.second);
                cout << "TIME TO DELIVERY IT" << endl;
            }
        };
        return ordersToDelivery;
    };

    json serialize(){
        json store;
        store["productIds"] = json::array();
        store["schedule"] = json::array();
        store["name"] = this->name;
        for (auto x : this->productIds){
            store["schedule"].push_back(x);
        };
        for (auto x : this->schedule){
            json schedule;
            schedule["time"] = x.first;
            schedule["coords"] = json ::array({x.second.deliveryTo.first, x.second.deliveryTo.second});
            store["schedule"] = schedule;
        };
        return store;
    }
};

#endif //MDILIVERY_STORE_H
