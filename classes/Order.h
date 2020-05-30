//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_ORDER_H
#define MDILIVERY_ORDER_H

#include "Car.h"
#include "MapPoint.h"
#include "Store.h"
#include <list>
#include "../json.hpp"

using nlohmann::json;


template <class T>
class Order {
    public:
        int deliveryTime;
        int finishTime;

        bool finish = false;

        bool expired = false;
        string customer;
        list<OrderPart *> parts;
        std::map<string, int> waitingProducts;
        std::map<string, int> deliveredProducts;

        Order() = default;
        Order(std::map<string, int>productsToOrder, int ideliveryTime, pair<int,int> orderToCoords,
                list<pair<std::map<string, int>, list<T> *>> ideals,  map<int, map<int, T >>& items){
            this->customer = "Неведомый";
            this->deliveryTime = ideliveryTime;
            this->waitingProducts = productsToOrder;

            for (pair<std::map<string, int>, list<T> * > deal : ideals){
                if (deal.second->size() == 0) { continue;}
                list <pair<int, int>> pathCoords;
                Store * store = NULL;
                for (T x: *deal.second){
                    if (store == NULL){
                        store = items[x.XCoord][x.YCoord].store;
                    }
                    pathCoords.push_back(pair<int, int> (x.XCoord, x.YCoord));
                }
                OrderPart * part_ = NULL;
                part_ =  new OrderPart(
                        parts.size(),
                        deal.first, pathCoords,
                        store->coords,
                        orderToCoords
                );
                store->orderToSchedule(ideliveryTime - pathCoords.size(), part_);
                parts.push_back(part_);
//                items[store->coords.first][store->coords.second].store =
            }

        };
    //    Store getNearStore(int product_id){};

        void checkExpired(int tickCount){
            expired = (finish)? finishTime <= deliveryTime: deliveryTime < tickCount;
        };

        void checkParts(int tickCount){
            list<OrderPart * > removePart;
            for (OrderPart * part : parts){
                if (!part->getDeliveredStatus()){
                    continue;
                }
                for (auto product: part->products){
                    deliveredProducts[product.first] = product.second;
                }
                removePart.push_back(part);
            }
            for (OrderPart * toRemove: removePart){
                parts.remove(toRemove);
                delete toRemove;
            }
            finish = (parts.size() == 0);
            if (finish){
                finishTime = tickCount;
                checkExpired(tickCount);
            }
        }

    json serialize(){
        json order;
//        if (this == NULL) {return order;}

//        order["customer"] = this->customer;
        order["deliveryTime"] = deliveryTime;
        order["finished"] = finish;
        order["expired"] = expired;

        return order;
    }

    };

#endif //MDILIVERY_ORDER_H
