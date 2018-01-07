//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#ifndef IMS_LUNCH_H
#define IMS_LUNCH_H

#include "../common/config.h"
#include "Toilet.h"

class LunchGenerator : public Event {
    enum State { H7 = 7, H8 = 8, H9 = 9 };
    int S = H7;
    int link;
    void Behavior() override;
public:
    LunchGenerator(int i) : link(i) {}
};

class Lunch : public Process {
    void Behavior() override;
    int link;
    Store * employee;
    Toilet * toilet;
    Queue * Q;
public:
    Lunch(int i);
};


#endif //IMS_LUNCH_H
