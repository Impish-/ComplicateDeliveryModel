#ifndef MDILIVERY_STORE_H
#define MDILIVERY_STORE_H

#include <string>
#include <list>
#include <set>
#include <iostream>
#include "OrderPart.h"
#include "MapPoint.h"

using namespace std;

class Store{
    public:
        string name;
        int carsCount = 0;
        list<pair<int, OrderPart>> schedule;
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

        void doDelivery(){};
        void runOrderPart(){};

        list<int> checkProduct(list<int>orderProductIds){
            list<int> result;
            list<int>::iterator it;
            for (auto product : orderProductIds){
                it = std::find(productIds.begin(), productIds.end(), product);
                if(it != productIds.end())
                    result.push_back(product);
            }
            return result;
        };

        void nextTick(int tickCount){
                for ( auto part : schedule ){
                    if (part.first == tickCount){
                        cout << "TIME TO DELIVERY IT" << endl;
                    }
                };

            };
};

#endif //MDILIVERY_STORE_H
