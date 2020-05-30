//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_MAPPOINT_H
#define MDILIVERY_MAPPOINT_H

#include <string>
#include <list>

#include <iostream>
#include <map>
#include "Order.h"

using namespace std;

static const char alphanum[] =
        "0123456789"
        "!@#$%^&*"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

char genRandom()
{

    return alphanum[rand() % stringLength];
}

string rnd(){
    string Str;
    for(unsigned int i = 0; i < 20; ++i)
    {
        Str += genRandom();

    }
    return Str;
}


class Barrier{
public:
    string name;
    int blockInterval;
    void changeTransmit(){};
    void setChangeTimeout(int timeout){};
};


class MapPoint{
    public:
        int XCoord;
        int YCoord;
        Store* store = NULL;

        void addStore(string name, map<string, int> iproductd){
             store = new Store(name, iproductd, pair<int, int> (XCoord, YCoord));
        };
        void setCoords(int x, int y){
            XCoord = x;
            YCoord = y;
        }

        void saveTo(){};
        void load(){};
};







#endif //MDILIVERY_MAPPOINT_H
