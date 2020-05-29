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
        bool finish = false;
        string customer;
        list<OrderPart *> parts;
        list<int> waitingProducts;
        list<int> deliveredProducts;

        Order() = default;
        Order(list<int>productIds, int ideliveryTime, pair<int,int> orderToCoords, list<pair<list<int>,
                list<T>>> ideals,  map<int, map<int, T >>& items){
            this->customer = "Неведомый";
            this->deliveryTime = ideliveryTime;
            this->waitingProducts = productIds;

            for (pair<list<int>, list<T>> deal : ideals){
                if (deal.second.size() == 0) { continue;}
                list <pair<int, int>> pathCoords;
                Store * store = NULL;
                for (auto x: deal.second){
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
                store->orderToSchedule(ideliveryTime, part_);
                parts.push_back(part_);
//                items[store->coords.first][store->coords.second].store =
            }

        };
    //    Store getNearStore(int product_id){};

        void genParts(){};
        void status(){}; //Процент исполеннных частей.
        void orderPartDelivered(){

        };

        void nextTick(int tickCount, map<int, map<int, T >>& items){
            OrderPart * removePart;
            for (OrderPart * part : parts){
                if (!part->getDeliveredStatus()){
                    continue;
                }
                deliveredProducts.insert(
                        waitingProducts.end(), part->products.begin(), part->products.end()
                        );
                removePart = part;
            }
            parts.remove(removePart);
            finish = (parts.size() == 0);
        }

    json serialize(){
        json order;
//        if (this == NULL) {return order;}

//        order["customer"] = this->customer;
        order["deliveryTime"] = deliveryTime;
        order["status"] = finish;

        return order;
    }

    };

#endif //MDILIVERY_ORDER_H
