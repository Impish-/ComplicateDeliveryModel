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
//        map<int, map<int, Type >>::iterator roadMapIter;
        allocator<Type> typeAllocator;
    public:
        Map(int m, int n){
            M=m;
            N=n;
        };
        Type insert(int x, int y){
            Type item = Type(x, y);
            items[item.XCoord][item.YCoord] = item;
//            cout << "insert (Map Object): " << item.XCoord << ", " << item.YCoord << endl;
            return item;
        };
        Type getElement(int XCoord, int YCoord){
            return items[XCoord][YCoord];
        };
        void remove(int XCoord, int YCoord){
            typeAllocator.destroy(&items[XCoord][YCoord]);
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
