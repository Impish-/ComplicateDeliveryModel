#ifndef MDILIVERY_MAP_H
#define MDILIVERY_MAP_H

#include <iostream>
#include <map>
#include <list>
#include "RoadObject.h"
#include "../json.hpp"

using namespace std;
using nlohmann::json;

class NoRoadException : public exception{
public:
    void virtual PrintMessage()
    {
        cout << "Try to add point without road"; // **
    }
};

class PastIntervalException : public exception{
public:
    void virtual PrintMessage()
    {
        cout << "delivery time in past"; // **
    }
};


class CantDeliveryException : public exception{
public:
    int aTick = -1;
    CantDeliveryException() = default;
    CantDeliveryException(int avalableTick){aTick = avalableTick;};
    const int get_avail() const { return aTick; }
    const string PrintMessage() const {
        if (aTick == -1){
            return "cant delivery current list of products (one or more products does not exist now)"; // **
        }
        return "cant delivery current list of products in available time, available interval:"
        + to_string( aTick); // **
    }
};

template <class T_RoadObjectType>
class Map {
    private:
        int N;
        int M;
        map<int, map<int, T_RoadObjectType >> items;
//        map<int, map<int, T_RoadObjectType >>::iterator = items.begin();
        allocator<T_RoadObjectType> typeAllocator;
        int currentTick =0;
    public:
        Map(int m, int n){
            M=m;
            N=n;
        };
        T_RoadObjectType insert(int x, int y){
//            Map self = *this;
//            if (!elementExist(x,y)){}
            items[x][y] = T_RoadObjectType(x, y);

//            items[x][y].setMapObject(Map<RoadObject>, this);
            return getElement(x,y);
        };

        T_RoadObjectType update(int x, int y, T_RoadObjectType newItem){
            items[x][y] = newItem;
            return getElement(x,y);
        };

        bool elementExist(int XCoord, int YCoord){
            T_RoadObjectType  z = items[XCoord][YCoord];
            return (z.XCoord > -1 & z.YCoord > -1);
        };

        T_RoadObjectType getElement(int XCoord, int YCoord){
            return items[XCoord][YCoord];
        };

        void remove(int XCoord, int YCoord){
            items[XCoord].erase(YCoord);
        };

        list<T_RoadObjectType> checkBranch(T_RoadObjectType roadBranc, T_RoadObjectType to, list<T_RoadObjectType> pathList, T_RoadObjectType fromRoad){
            pathList.push_back(fromRoad);
            bool found = (std::find(pathList.begin(), pathList.end(), roadBranc) != pathList.end());
            if (found){return pathList ;}
            if (roadBranc.XCoord == -1 & roadBranc.YCoord == -1){return pathList ;}

            if (roadBranc == to){
                pathList.push_back(roadBranc);
                return pathList;
            }
            return getPathList(roadBranc, to, pathList);
            }


        list<T_RoadObjectType> getPathList(T_RoadObjectType from, T_RoadObjectType to, list<T_RoadObjectType> pathList){
            list<T_RoadObjectType> rightRoad = checkBranch(items[from.XCoord + 1][from.YCoord], to, pathList, from);
            if ((std::find(rightRoad.begin(), rightRoad.end(), to) != rightRoad.end())){
                return rightRoad;
            }
            list<T_RoadObjectType> toLeft = checkBranch(items[from.XCoord - 1][from.YCoord], to, pathList, from);
            if ((std::find(toLeft.begin(), toLeft.end(), to) != toLeft.end())){
                return toLeft;
            }
            list<T_RoadObjectType> toBottom = checkBranch(items[from.XCoord][from.YCoord + 1], to, pathList, from);
            if ((std::find(toBottom.begin(), toBottom.end(), to) != toBottom.end())){
                return toBottom;
            }
            list<T_RoadObjectType> toTop = checkBranch(items[from.XCoord][from.YCoord - 1], to, pathList, from);
            if ((std::find(toTop.begin(), toTop.end(), to) != toTop.end())){
                return toTop;
            }
            return pathList;
        };

        list<T_RoadObjectType> getPathList(T_RoadObjectType from, T_RoadObjectType to) {
           list<RoadObject> a;
           return getPathList(from, to, a);
       }

       bool checkNeighboardPoints(int x, int y){
            bool left = !(x -1 > 0) & elementExist(x -1, y);
            bool right = !(x +1 <= N) & elementExist(x +1, y);
            bool top = !(y -1 > 0) & elementExist(x, y -1);
            bool bottom = !(y +1 <= M) & elementExist(x, y +1);
            bool result = left + right + top + bottom;
            return result;
        }

       void buildRoad(int x, int y, list<pair<int, int>>&road){

            bool ox = (M/2) > x;
            bool oy = (N/2) > y;

            float  rx = abs(M/2 - x);
            float  ry = abs(N/2 - y);
           if (x==8 & y == 25){

           }
            bool nb = checkNeighboardPoints(x,y);

            if (nb){
               // пришли крч
                rx = 0; ry = 0;
            }

            if(rx == 0 & ry == 0){
                insert(x, y);
                for (auto r : road){
                    try{
                        insert(r.first, r.second);
                    } catch (...) {
                            cout << "except insert:  " << x<<", " <<y  << endl;
                        }
                    }
                return;
            }

            auto value = [x, y, ox, oy](bool expr){
                return (expr) ?pair<int, int>(((ox)? x+1: x-1), y):
                    pair<int, int>(x, ((oy)? y+1: y-1));

            };
            auto args = value(rx>ry);

            road.push_back(args);
            return buildRoad(args.first, args.second, road);
        }


       void addStore(int x, int y, string name, map<string, int> products){
            T_RoadObjectType newStorePoint = getElement(x, y);
            if (newStorePoint.XCoord == -1 & newStorePoint.YCoord == -1){
//                throw NoRoadException();


                    bool haveNeibprds = checkNeighboardPoints(x,y);
                    if (!haveNeibprds){
                        list<pair<int, int>> road;
                        buildRoad(x,y, road);
                    };
            }
           newStorePoint.setCoords(x, y);
            newStorePoint.addStore(name, products);
            items[x][y] = newStorePoint;

         };

       list <pair<int, int>> convertRoadObjectToCoords(list<RoadObject> way){
            list <pair<int, int>> pathCoords;
            for (auto x: way){
                pathCoords.push_back(pair<int, int> (x.XCoord, x.YCoord));
            }
            return pathCoords;
        }


    int pathCalcBarriers(list<T_RoadObjectType> path){
        list<int> crossroads; // int - кол-во джорог

        pair<int, int> from; // направление
        for (auto path_: path){
            bool _x = elementExist(path_.XCoord -1, path_.YCoord);
            bool x_ = elementExist(path_.XCoord +1, path_.YCoord);
            bool _y = elementExist(path_.XCoord, path_.YCoord -1);
            bool y_ = elementExist(path_.XCoord, path_.YCoord +1);

            from = pair<int, int>(path_.XCoord,path_.YCoord);
            int result = int(_x) + int(x_) + int(_y) + int(y_);
            if (result == 2) { continue;}
            crossroads.push_back((result > 3)?2:1);
        }

        return std::accumulate(crossroads.begin(), crossroads.end(), 0);
    }

    int pathCalcDeliveryTime(list<T_RoadObjectType> path){
        return path.size() + pathCalcBarriers(path);
    }

       bool getDeliveryCandidates(std::map<string, int> & productsToOrder,
                RoadObject& orderTo,
                list<pair<pair<Store*, map<string, int>>, int>> & candidates, int availTime){
           /// вернет список всеъ <Сторов, кол-во интересующего оборудования>
           /// все Store подходят для доставки в заданный интервал

           map<string, int> canDeliveryProducts;
           canDeliveryProducts.clear();
           for ( auto X : items ){
                for ( auto Y : X.second ){
                    list<RoadObject> path = getPathList(Y.second, orderTo);
                    int canditateDeliveryTime = pathCalcDeliveryTime(path);
                    if(canditateDeliveryTime > availTime) { continue;}
                    if (Y.second.store == NULL) { continue;};
                    if (Y.second.store->checkProduct(productsToOrder).size() == 0){ continue;}
                    if(canditateDeliveryTime > availTime) { continue;}
                    map<string, int> orderHere;
                    orderHere.clear();

                    for (auto productFromStore: Y.second.store->checkProduct(productsToOrder)){
//                        map<string, int>::iterator it;
                        auto search = canDeliveryProducts.find(productFromStore.first);
//                        it = canDeliveryProducts.find(productFromStore);
                        if (search == canDeliveryProducts.end()){
                            orderHere[productFromStore.first] = productFromStore.second;
                            canDeliveryProducts[productFromStore.first] = productFromStore.second;
                        }
                    }
                    auto store_order  = pair<Store*, map<string, int>> (Y.second.store, orderHere);
                    pair<pair<Store*, map<string, int>>, int> path_ (
                            store_order,
                            canditateDeliveryTime
                    );
                    candidates.push_back(path_);
                };
           }

           candidates.sort([](const pair<pair<Store*, map<string, int>>, int> &a,
                                    const pair<pair<Store*, map<string, int>>, int> &b ){
                                return a.second < b.second;
                          });
           return (canDeliveryProducts.size() == productsToOrder.size());
        }

        void processOrder(int x, int y, int deliveryTime, std::map<string, int> productsToOrder){
            T_RoadObjectType orderPoint = getElement(x, y);
            list<pair<pair<Store*, map<string, int>>, int>> candidates;

            int deliveryAvailableInterval = deliveryTime - currentTick;
            if (deliveryAvailableInterval <= 0){
                throw PastIntervalException();
            };
            bool availDelivery = getDeliveryCandidates(productsToOrder, orderPoint, candidates, deliveryAvailableInterval);
            if (!availDelivery){
                bool _subAvailDelivery = false;
                int deliveryAvailableInterval_ = deliveryAvailableInterval;
                while (!_subAvailDelivery){
                    candidates.clear();
                    bool availDelivery_ = getDeliveryCandidates(
                            productsToOrder,
                            orderPoint,
                            candidates,
                            deliveryAvailableInterval_++
                            );
                    if (availDelivery_){
                        throw CantDeliveryException(deliveryAvailableInterval_);
                    }
                    if (deliveryAvailableInterval_ > M*N){
                        throw CantDeliveryException();
                    }
                }
            }

            map<string, int> canDeliveryProducts;
            auto * dealsP1 = new list<
                    pair<std::map<string, int>, T_RoadObjectType >
            >;

            for (auto deliveryCandidate : candidates){
                std::map<string, int> orderHere;
                orderHere.clear();
                RoadObject storePoint =
                        getElement(
                                deliveryCandidate.first.first->coords.first,
                                deliveryCandidate.first.first->coords.second
                                );

                // можно зарефакторить (сщбираем заказ у конкретного Store)
                for (pair<string, int> product: deliveryCandidate.first.second){
//                    list<pair<string, int>>::iterator it;
                    auto search = canDeliveryProducts.find(product.first);
                    if (search == canDeliveryProducts.end()){
                        orderHere[product.first] = product.second;
                        canDeliveryProducts[product.first] = product.second;
                    }else{

                    }

                }
                // /можно зарефакторить
                if (orderHere.size() == 0){
                    continue;
                }


               dealsP1->push_back( pair<std::map<string, int>, T_RoadObjectType> (
                        orderHere,
                        storePoint
                ));
            }

            list<
                    pair<
                            std::map<string, int>,
                            list<T_RoadObjectType>
                    >
            > dealsP2;



            for (auto d: *dealsP1){
                list<RoadObject> path__ = getPathList(d.second, orderPoint);

            }

//            orderPoint.addOrder(productsToOrder, deliveryTime, dealsP2, items);
//            items[x][y] = orderPoint;
        }

        void nextTick(int tickCount){
           currentTick ++;
            for ( auto X : items ){
                for ( auto Y : X.second ){
                    if (Y.second.store == NULL){ Y.second.nextTick(tickCount, items); continue;}
                    list<OrderPart*> orders = Y.second.store->nextTick(tickCount);
                    for (OrderPart * orderPartToDelivery : orders){
                        list<RoadObject> convertedPath;
                        for (pair<int, int> pathPair: orderPartToDelivery->path){
                            convertedPath.push_back(items[pathPair.first][pathPair.second]);
                        };
                        pair<int, int> fromChords = orderPartToDelivery->path.front();
                        RoadObject from = items[fromChords.first][fromChords.second];
                        items[fromChords.first][fromChords.second] = from.startCar(convertedPath,
                                orderPartToDelivery, *Y.second.store);
                    };
                    Y.second.nextTick(tickCount, items);
                };
            }
        };
        json serialize(){
            json mapJSON;
            mapJSON["points"] = json::array();
            for ( auto X : items ) {
                for (auto Y : X.second) {
                    if (Y.second.XCoord>0 & Y.second.YCoord>0)
                        mapJSON["points"].push_back(Y.second.serialize());
                };
            };
            return mapJSON;
        };
};


#endif //MDILIVERY_MAP_H
