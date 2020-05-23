#ifndef MDILIVERY_MAP_H
#define MDILIVERY_MAP_H

#include <iostream>
#include <map>
#include <list>
#include "RoadObject.h"


using namespace std;

template <class Type>
class Map {
    private:
        int N;
        int M;
        map<int, map<int, Type >> items;
//        map<int, map<int, Type >>::iterator = items.begin();
        allocator<Type> typeAllocator;
    public:
        Map(int m, int n){
            M=m;
            N=n;
        };
        Type insert(int x, int y){
//            Map self = *this;
            items[x][y] = Type(x, y);
//            items[x][y].setMapObject(Map<RoadObject>, this);
            return getElement(x,y);
        };

        Type update(int x, int y, Type newItem){
            items[x][y] = newItem;
            return getElement(x,y);
        };

        Type getElement(int XCoord, int YCoord){
            return items[XCoord][YCoord];
        };

        void remove(int XCoord, int YCoord){
            items[XCoord].erase(YCoord);
        };
        void tickH(pair<const int, pair<int, map<int, Type >> >& pair){
            cout << "sdf" << endl;
        };

        list<Type> checkBranch(Type roadBranc, Type to,list<Type> pathList, Type fromRoad){
            pathList.push_back(fromRoad);
            bool found = (std::find(pathList.begin(), pathList.end(), roadBranc) != pathList.end());
            if (found){return pathList ;}
            if (roadBranc.XCoord == -1 & roadBranc.YCoord == -1){return pathList ;}

            if (roadBranc == to){
                cout << "WAY BUILDED";
                pathList.push_back(roadBranc);
                return pathList;
            }
            return getPathList(roadBranc, to, pathList);
            }


        list<Type> getPathList(Type from, Type to, list<Type> pathList){
            list<Type> rightRoad = checkBranch(items[from.XCoord + 1][from.YCoord], to, pathList, from);
            if ((std::find(rightRoad.begin(), rightRoad.end(), to) != rightRoad.end())){
                return rightRoad;
            }
            list<Type> toLeft = checkBranch(items[from.XCoord - 1][from.YCoord], to, pathList, from);
            if ((std::find(toLeft.begin(), toLeft.end(), to) != toLeft.end())){
                return toLeft;
            }
            list<Type> toBottom = checkBranch(items[from.XCoord][from.YCoord + 1], to, pathList, from);
            if ((std::find(toBottom.begin(), toBottom.end(), to) != toBottom.end())){
                return toBottom;
            }
            list<Type> toTop = checkBranch(items[from.XCoord][from.YCoord-1], to, pathList, from);
            if ((std::find(toTop.begin(), toTop.end(), to) != toTop.end())){
                return toTop;
            }
            return pathList;
        };

        list<Type> getPathList(Type from, Type to) {
           list<RoadObject> a;
           return getPathList(from, to, a);
       }

        void addStore(int x, int y, string name, list<int>productIds){
            Type targetPoint = getElement(x, y);
            targetPoint.addStore(name, productIds);
            items[x][y] = targetPoint;
         };

        void processOrder(int x, int y, int deliveryTime, list<int>productIds){
            Type obj = getElement(x, y);
            list<pair <Store, pair<list<int>, list<RoadObject>>>> deals;
            for ( auto X : items )
            {
                for ( auto Y : X.second ){
                    list<int> haveProducts = Y.second.store.checkProduct(productIds);
                    if (haveProducts.size() > 0){
                        list<RoadObject> pathToOrder = getPathList(Y.second, obj);
                        pair<Store, pair<list<int>, list<RoadObject>>>
                                storePair (Y.second.store, pair<list<int>,
                                        list<RoadObject>>( haveProducts, pathToOrder));
                        deals.push_back(storePair);
                    }
                };
            }

            obj.addOrder(deliveryTime, deals, items);
            items[x][y] = obj;
        }

        void nextTick(int tickCount){
            for ( auto X : items )
            {
                for ( auto Y : X.second ){
                    Y.second.nextTick(tickCount, items);
                    list<OrderPart> orders = Y.second.store.nextTick(tickCount);
                    for (auto order : orders){
                        RoadObject from = items[order.deliveryFrom.first][order.deliveryFrom.second];
                        list<RoadObject> convertedPath;
                        for (auto pathPair: order.path){
                            convertedPath.push_back(items[pathPair.first][pathPair.second]);
                        };
                        from.startCar(convertedPath);
                    };
                };
            }
        };
        void serialize();
        void unSerialize();
};


#endif //MDILIVERY_MAP_H
