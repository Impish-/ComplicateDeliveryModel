#ifndef MDILIVERY_MAP_H
#define MDILIVERY_MAP_H

#include <iostream>
#include <map>
#include <list>
#include "RoadObject.h"
#include "../json.hpp"

using namespace std;
using nlohmann::json;

class NoRoadException : public CException
{
public:
    void virtual PrintMessage()
    {
        cout << "Try to add point without road"; // **
    }
};


template <class RoadObjectType>
class Map {
    private:
        int N;
        int M;
        map<int, map<int, RoadObjectType >> items;
//        map<int, map<int, RoadObjectType >>::iterator = items.begin();
        allocator<RoadObjectType> typeAllocator;
    public:
        Map(int m, int n){
            M=m;
            N=n;
        };
        RoadObjectType insert(int x, int y){
//            Map self = *this;
            items[x][y] = RoadObjectType(x, y);
//            items[x][y].setMapObject(Map<RoadObject>, this);
            return getElement(x,y);
        };

        RoadObjectType update(int x, int y, RoadObjectType newItem){
            items[x][y] = newItem;
            return getElement(x,y);
        };

        bool elementExist(int XCoord, int YCoord){
            RoadObjectType * z = &items[XCoord][YCoord];
            return (z->XCoord > -1 & z->YCoord > -1);
        };

        RoadObjectType getElement(int XCoord, int YCoord){
            return items[XCoord][YCoord];
        };

        void remove(int XCoord, int YCoord){
            items[XCoord].erase(YCoord);
        };
        void tickH(pair<const int, pair<int, map<int, RoadObjectType >> >& pair){
            cout << "sdf" << endl;
        };

        list<RoadObjectType> checkBranch(RoadObjectType roadBranc, RoadObjectType to, list<RoadObjectType> pathList, RoadObjectType fromRoad){
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


        list<RoadObjectType> getPathList(RoadObjectType from, RoadObjectType to, list<RoadObjectType> pathList){
            list<RoadObjectType> rightRoad = checkBranch(items[from.XCoord + 1][from.YCoord], to, pathList, from);
            if ((std::find(rightRoad.begin(), rightRoad.end(), to) != rightRoad.end())){
                return rightRoad;
            }
            list<RoadObjectType> toLeft = checkBranch(items[from.XCoord - 1][from.YCoord], to, pathList, from);
            if ((std::find(toLeft.begin(), toLeft.end(), to) != toLeft.end())){
                return toLeft;
            }
            list<RoadObjectType> toBottom = checkBranch(items[from.XCoord][from.YCoord + 1], to, pathList, from);
            if ((std::find(toBottom.begin(), toBottom.end(), to) != toBottom.end())){
                return toBottom;
            }
            list<RoadObjectType> toTop = checkBranch(items[from.XCoord][from.YCoord - 1], to, pathList, from);
            if ((std::find(toTop.begin(), toTop.end(), to) != toTop.end())){
                return toTop;
            }
            return pathList;
        };

        list<RoadObjectType> getPathList(RoadObjectType from, RoadObjectType to) {
           list<RoadObject> a;
           return getPathList(from, to, a);
       }

       void checkNeighboardPoints(RoadObjectType curPoint){
            bool left = elementExist(curPoint.XCoord -1, curPoint.YCoord);
            bool right = elementExist(curPoint.XCoord -1, curPoint.YCoord);
            bool top = elementExist(curPoint.XCoord, curPoint.YCoord -1);
            bool bottom = elementExist(curPoint.XCoord, curPoint.YCoord +1);
            bool result = left * right * top * bottom;

        }

        void addStore(int x, int y, string name, list<int>productIds){
            RoadObjectType targetPoint = getElement(x, y);
            if (targetPoint.XCoord==-1 & targetPoint.YCoord==-1){
                throw NoRoadException();
//                if (!elementExist(x, y)){
//                    checkNeighboardPoints(targetPoint);
//                }
                targetPoint.setCoords(x,y);
            }
            targetPoint.addStore(name, productIds);
            items[x][y] = targetPoint;
         };

        list <pair<int, int>> convertRoadObjectToCoords(list<RoadObject> way){
            list <pair<int, int>> pathCoords;
            for (auto x: way){
                pathCoords.push_back(pair<int, int> (x.XCoord, x.YCoord));
            }
            return pathCoords;
        }

        void processOrder(int x, int y, int deliveryTime, list<int>productIds){
            RoadObjectType obj = getElement(x, y);
            list<pair<list<int>, list<RoadObject>>> deals;
            for ( auto X : items )
            {
                for ( auto Y : X.second ){
                    if (Y.second.store == NULL) { continue;};

                    list<int> haveProducts = Y.second.store->checkProduct(productIds);
                    if (haveProducts.size() > 0){
                        list <RoadObject> pathToOrder = getPathList(Y.second, obj);
                        pair<list<int>, list<RoadObject>>
                                storePair( haveProducts, pathToOrder);
                        deals.push_back(storePair);
                    }
                };
            }

            obj.addOrder(deliveryTime, deals, items);
            items[x][y] = obj;
        }

        void nextTick(int tickCount){
            for ( auto X : items ){
                for ( auto Y : X.second ){
                    if (Y.second.store == NULL){ continue;}
                    list<OrderPart> orders = Y.second.store->nextTick(tickCount);
                    for (OrderPart orderPartToDelivery : orders){
                        list<RoadObject> convertedPath;
                        for (auto pathPair: orderPartToDelivery.path){
                            convertedPath.push_back(items[pathPair.first][pathPair.second]);
                        };
                        pair<int, int> fromChords = orderPartToDelivery.path.front();
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
