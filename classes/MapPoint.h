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

class CException
{
public:
    void virtual PrintMessage() {}
};

class RoadPointIsFull : public CException
{
public:
    void virtual PrintMessage()
    {
        cout << "Hallo from exception"; // **
    }
};

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
        Store store;


        void addStore(string name, list<int> iproductIds){
            store = Store(name, iproductIds, pair<int, int> (XCoord, YCoord));
        };

        void saveTo(){};
        void load(){};
};







#endif //MDILIVERY_MAPPOINT_H
