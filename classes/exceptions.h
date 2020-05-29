//
// Created by Соколов Илья on 30.05.2020.
//

#ifndef MDILIVERY_EXCEPTIONS_H
#define MDILIVERY_EXCEPTIONS_H

#include <iostream>
#include <string>

class RoadPointIsFull : public std::exception
{
public:
    RoadPointIsFull(){};
    string virtual PrintMessage(){
        return "Road is occupied"; // **
    }
};

#endif //MDILIVERY_EXCEPTIONS_H
