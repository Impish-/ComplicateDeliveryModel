//
// Created by Соколов Илья on 22.05.2020.
//

#ifndef MDILIVERY_CAR_H
#define MDILIVERY_CAR_H

using namespace std;

template <class T>
class Car {
    public:
        Car(){};
        Car(T start, T end){
            currentPoint = start;
            finishPoint = end;
        };
        string name;
        void nextTick(int tickCount){
            cout   << "\t\t" <<  _str() <<": car.tick handler" <<endl;
    //        carsIter.
        };

    string _str(){
        auto s = "<Car " + name + ">";
        return s;
    };

    private:
        T currentPoint;
        T finishPoint;



};

#endif //MDILIVERY_CAR_H
