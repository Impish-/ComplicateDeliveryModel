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

       bool checkNeighboardPoints(T_RoadObjectType curPoint){
            bool left = !(curPoint.XCoord -1 > 0) & elementExist(curPoint.XCoord -1, curPoint.YCoord);
            bool right = !(curPoint.XCoord +1 <= N) & elementExist(curPoint.XCoord +1, curPoint.YCoord);
            bool top = !(curPoint.YCoord -1 > 0) & elementExist(curPoint.XCoord, curPoint.YCoord -1);
            bool bottom = !(curPoint.XCoord +1 <= M) & elementExist(curPoint.XCoord, curPoint.YCoord +1);
            bool result = left + right + top + bottom;
            return result;
        }

       void buildRoad(int x, int y, list<pair<int, int>>&road){
            bool ox = (M/2) > x;
            bool oy = (N/2) > y;

            float  rx = abs(M/2 - x);
            float  ry = abs(N/2 - y);

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


       void addStore(int x, int y, string name, list<int>productIds){
            T_RoadObjectType newStorePoint = getElement(x, y);
            if (newStorePoint.XCoord == -1 & newStorePoint.YCoord == -1){
//                throw NoRoadException();
                    newStorePoint.setCoords(x, y);

                    bool haveNeibprds = checkNeighboardPoints(newStorePoint);
                    if (!haveNeibprds){
                        list<pair<int, int>> road;
                        buildRoad(x,y, road);
                    };
            }
            newStorePoint.addStore(name, productIds);
            items[x][y] = newStorePoint;

         };

       list <pair<int, int>> convertRoadObjectToCoords(list<RoadObject> way){
            list <pair<int, int>> pathCoords;
            for (auto x: way){
                pathCoords.push_back(pair<int, int> (x.XCoord, x.YCoord));
            }
            return pathCoords;
        }

       bool getDeliveryCandidates(list<int> & productIds,
                RoadObject& orderTo,
                list<pair<Store*, int>> & candidates, int availTime){

           list<int> canDeliveryProducts;
           for ( auto X : items ){
                for ( auto Y : X.second ){
                    if (Y.second.store == NULL) { continue;};
                    if (Y.second.store->checkProduct(productIds).size() == 0){ continue;}
                    if(getPathList(Y.second, orderTo).size() > availTime) { continue;}
                    list<int> orderHere;
                    orderHere.clear();
                    for (auto productId: Y.second.store->checkProduct(productIds)){
                        list<int>::iterator it;
                        it = std::find(canDeliveryProducts.begin(), canDeliveryProducts.end(), productId);
                        if (it == canDeliveryProducts.end()){
                            orderHere.push_back(productId);
                            canDeliveryProducts.push_back(productId);
                        }
                    }
                    pair<Store*, int> path_(
                            Y.second.store, getPathList(Y.second, orderTo).size()
                    );
                    candidates.push_back(path_);
                };
           }

           candidates.sort([](const pair<Store*, int> &a,
                                    const pair<Store*, int> &b ){
                                return a.second < b.second;
                          });
           return (canDeliveryProducts.size() == productIds.size());
        }

        void processOrder(int x, int y, int deliveryTime, list<int>productIds){
            T_RoadObjectType orderPoint = getElement(x, y);
            list<pair<list<int>, list<RoadObject>>> deals;
            list<pair<Store*, int>> candidates;
            int deliveryAvailableInterval = deliveryTime - currentTick;
            if (deliveryAvailableInterval <= 0){
                throw PastIntervalException();
            };

            bool availDelivery = getDeliveryCandidates(productIds, orderPoint, candidates, deliveryAvailableInterval);
            if (!availDelivery){
                bool _subAvailDelivery = false;
                int deliveryAvailableInterval_ = deliveryAvailableInterval;
                while (!_subAvailDelivery){
                    candidates.clear();
                    bool availDelivery_ = getDeliveryCandidates(
                            productIds,
                            orderPoint,
                            candidates,
                            deliveryAvailableInterval_++
                            );
                    if (availDelivery_){
                        throw CantDeliveryException(deliveryAvailableInterval_);
                    }
                    if (deliveryAvailableInterval_ > max(M,N)){
                        throw CantDeliveryException();
                    }
                }
            }

            list<int> canDeliveryProducts;
            for (auto x : candidates){
               pair<list<int>, list<RoadObject>> storePair(
                            canDeliveryProducts,
                            getPathList(
                                    getElement(x.first->coords.first, x.first->coords.second),
                                    orderPoint
                            )
                     );
               deals.push_back(storePair);

            }
            orderPoint.addOrder(productIds, deliveryTime, deals, items);
            items[x][y] = orderPoint;
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
