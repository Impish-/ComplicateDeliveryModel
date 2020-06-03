#ifndef MDILIVERY_ROADOBJECT_H
#define MDILIVERY_ROADOBJECT_H

#include <deque>
#include "MapPoint.h"
#include "../json.hpp"

using nlohmann::json;

class RoadObject: MapPoint{
private:
    Barrier activeBarrier;
    int carMaxCount = 1;
    list<Car<RoadObject>> cars;
    list<Car<RoadObject>>::iterator carsIter = cars.begin();

public:
    using MapPoint::store;
    using MapPoint::XCoord;
    using MapPoint::YCoord;
    using MapPoint::addStore;
    using MapPoint::MapPoint;
    using MapPoint::setCoords;
    Order<RoadObject> * activeOrder = NULL;

    RoadObject() {XCoord=-1; YCoord=-1;};
    RoadObject(int x, int y){XCoord = x;YCoord = y; activeOrder = NULL;};


    std::deque<std::vector<unsigned char>> m_chunks;
    std::size_t m_available = 0;
    unsigned char* m_memory;
    int m_allocations = 0;

    void* allocate(std::size_t n) {
        if (n > m_available) {
            m_chunks.emplace_back(100500);
            m_available = m_chunks.back().size();
            m_memory = &m_chunks.back().front();
        }

        auto mem = m_memory;
        m_available -= n;
        m_memory += n;
        ++m_allocations;
        return mem;
    }
    void deallocate(void* p, std::size_t n) {
        --m_allocations;
        auto mem = (unsigned char*)p;
        if (mem + n == m_memory) {
            m_memory = mem;
            m_available += n;
        }
    }


    bool operator == (const RoadObject other){
        return this->XCoord == other.XCoord && this->YCoord == other.YCoord;
    };


    void addOrder(std::map<string, int> productsToOrder,  int deliveryTick,
                    list<pair<std::map<string, int>, list<RoadObject> *>> deals,
                    map<int, map<int, RoadObject >>& items){
        this -> activeOrder = new Order<RoadObject>(productsToOrder, deliveryTick, pair<int,int> (XCoord, YCoord), deals, items);
        items[XCoord][YCoord] = *this;
    };

    void closeOrder(){
//            activeOrder = NULL;
    };

    void nearRoads(){

    };

    void canIncome(){};
    void serBarrier(){};
    void deleteBarrier(){};

    bool carExist(Car<RoadObject> checkingCar){
        for ( auto car : cars ){
            if (checkingCar == car){
                return true;
            };
        };
        return false;
    };

//    Car<RoadObject> getCar(Car<RoadObject> checkingCar){
//        for ( auto car : cars ) {
//            if (checkingCar == car) {
//                return car;
//            };
//        };
//    };

    void updateCar(Car<RoadObject> checkingCar){
        list<Car<RoadObject>> newCars;
        for ( auto car : cars ){
            if (checkingCar == car){
                newCars.push_back(checkingCar);

            }else{newCars.push_back(car);}

            cars = newCars;
            return;
        };
    };

    void leftCar(Car<RoadObject> outCar){
        list<Car<RoadObject>> newCars;
        for ( auto car : cars ){
            if (outCar == car){
                continue;
            };
            newCars.push_back(car);
        };
        cars = newCars;
    }

    RoadObject outOffCar(Car<RoadObject> outCar){
        leftCar(outCar);
        return *this;
    }

    RoadObject incomingCar(Car<RoadObject>& incomeCar, bool startFromHere=false){
        if (cars.size() == carMaxCount){
            throw RoadPointIsFull();
        }
        if(!carExist(incomeCar)) cars.push_back(incomeCar); // if not exist
        incomeCar.setCurrent(*this);
//        cars.push_back(incomeCar);

//        cout << "\t\t~ RoadObject" << _str() << "INSERTED CAR size("<< cars.size()<<")"<<endl;
        return *this;
    }

    RoadObject startCar(list<RoadObject> carPath, OrderPart * orderPart, Store & store){
        Car<RoadObject> newCar = Car<RoadObject>(carPath, orderPart, store);
        newCar.name = rnd();
        return incomingCar(newCar, true);
    };

    bool haveActions(int tickCount){
        bool result = true;
        for ( auto car : cars ){
            result = result * (car.tickUpdated == tickCount);
        };
        return result;
    }

    string _str() {
        auto s = "<MapPoint X:" + to_string(XCoord) + ", Y:" + to_string(YCoord) + ">";
        return s;
    };


    void nextTick(int tickCount, map<int, map<int, RoadObject >>& items){
        if (activeOrder !=NULL){
            activeOrder->checkExpired(tickCount);
        }
//        if ((cars.size() < 1)) return;
//        if (haveActions(tickCount)) return;
//        cout << "\t POINT HAVE ACTIONS:" << _str() << ", CARS HERE:" << cars.size()<<endl;


        for ( auto car : cars ){
            if(car.tickUpdated >= tickCount){
                continue;
            };
            car.nextTick(tickCount, items, *this);
        };

        if (activeOrder !=NULL){
            activeOrder->checkParts(tickCount);
            if (activeOrder->finish){
                if(activeOrder->finishTime + 15 <= tickCount){
                    activeOrder = NULL;
                    items[this->XCoord][this->YCoord].activeOrder = NULL;
                    delete activeOrder;
                }
            }
        }
    };

    json serialize(){
        json point;
        point["x"] = this->XCoord;
        point["y"] = this->YCoord;
//        cout << "\nSERIALIZE X:" << XCoord << ", Y:" << YCoord <<endl;
//        cout << "ORDERS";
        if (this->activeOrder != NULL) (
                point["order"] = this->activeOrder->serialize()
        );

//        cout << "- done"<<endl;
//        cout << "STORE"  ;
        if (this->store != NULL){point["store"] = this->store->serialize();}
//        cout << "- done"<<endl;
//        cout << "CARS" ;
        if (this->cars.size() > 0){
            point["cars"] = json::array();
            for (auto car: this->cars){
                point["cars"].push_back(car.name);
            };
        }
//        cout << "- done"<<endl;
//        cout <<point <<endl;

        return point;
    }
};


#endif //MDILIVERY_ROADOBJECT_H
