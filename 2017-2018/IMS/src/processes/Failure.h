//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#ifndef IMS_FAILURE_H
#define IMS_FAILURE_H

#include "../common/config.h"
#include "Post.h"

class Failure : public Process {
    void Behavior() override;
public:
    Failure() { Activate(); };
};

class Control : public Process {
    void Behavior() override;
    Facility *line;
    double wait;
    int link;
public:
    Control(int i, double t);
};


#endif //IMS_FAILURE_H
