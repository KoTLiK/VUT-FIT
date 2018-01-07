//
// Created by milan on 2.12.2017.
//

#ifndef IMS_TOILET_H
#define IMS_TOILET_H

#include "../common/config.h"

class Toilet : public Process {
    void Behavior() override;
private:
    int link, state;
    Store * store;
public:
    enum State { START, READY, ACTIVE, LUNCH, RESET};
    Toilet(int i);
    void setState(State s) { this->state = s; }
    int getState() { return this->state; }
    void leaveStore() { Leave(*store, 1); }
};


#endif //IMS_TOILET_H
