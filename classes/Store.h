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
        list<pair<int, OrderPart *>> schedule;
        list<OrderPart> deliveryNotify;
        map<string, int> productsAvailable;
        pair<int, int> coords;

        Store() = default;
        Store(string iName,map<string, int>products, pair<int, int> icoords){
            name = iName;
            productsAvailable = products;
            coords = icoords;
        };

        Store orderToSchedule(int ideliveryTime, OrderPart * orderPart){
            schedule.push_back(pair<int, OrderPart*> (ideliveryTime, orderPart));
            return *this;
        };

        void delivered(OrderPart * part){
            list<pair<int, OrderPart *>>::iterator it = schedule.begin();
            while (it != schedule.end()) {
                if ((*it).second == part) {
                    it = schedule.erase(it);
                } else
                    it++;
            }
            part->setDeliveredStatus(true);
        };

        bool checkSingleProductAvailable(string productName){
            map<string, int>::iterator it;
            it =productsAvailable.find(productName);
            return (it !=  productsAvailable.end());
        }

        list<pair<string, int>> checkProduct(map<string, int> orderProductIds){
            list<pair<string, int>> result;
            for (auto product : orderProductIds){
                if(checkSingleProductAvailable(product.first))
                    result.push_back(product);
            }
            return result;
        };

    list<OrderPart *> nextTick(int tickCount){
        list<OrderPart *> ordersToDelivery;
        for ( auto part : schedule ){
            if (part.first == tickCount){
                part.second->deliveryFrom = coords;
                ordersToDelivery.push_back(part.second);
                cout << "TIME TO DELIVERY IT" << endl;
            }
        };
        return ordersToDelivery;
    };

    json serialize(){
        json store;
        store["products"] = json::array();
        store["schedule"] = json::array();
        store["name"] = this->name;
        for (auto x : this->productsAvailable){
            json product;
            product['name'] = x.first;
            product['countNow'] = x.second;
            store["products"].push_back(product);
        };
        for (auto x : this->schedule){
            json schedule;
            schedule["time"] = x.first;
            schedule["coords"] = json ::array({
                x.second->deliveryTo.first,
                x.second->deliveryTo.second
            });
            store["schedule"] = schedule;
        };
        return store;
    }
};

#endif //MDILIVERY_STORE_H
