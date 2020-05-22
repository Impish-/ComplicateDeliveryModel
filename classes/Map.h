#ifndef MDILIVERY_MAP_H
#define MDILIVERY_MAP_H

#include <iostream>
#include <map>

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

        void nextTick(int tickCount){
            cout << "MAP NextTick "<< tickCount << endl;
            for ( auto X : items )
            {
                for ( auto Y : X.second ){
                    Y.second.nextTick(tickCount); // Делегируем обработку события всем элементам
                };
            }
        };
        void serialize();
        void unSerialize();
};


#endif //MDILIVERY_MAP_H
