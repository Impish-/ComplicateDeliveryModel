#ifndef MDILIVERY_STORE_H
#define MDILIVERY_STORE_H

#include <string>
#include <list>
#include "Order.h"

using namespace std;

class Store{
    public:
        string name;
        int carsCount = 0;
        list<OrderPart> schedule;

        Store() = default;
        Store(string iName, int *productIds){
            name = iName;
        };

        void orderToSchedule(){};
        void doDelivery(){};
        void runOrderPart(){};
        void checkProducts(){};

        void nextTick(int tickCount){
                for ( auto part : schedule ){
                    if (part.startDeliveryTick == tickCount){
                        cout << "TIME TO DELIVERY IT" << endl;
                    }
                };

            };
};

#endif //MDILIVERY_STORE_H
