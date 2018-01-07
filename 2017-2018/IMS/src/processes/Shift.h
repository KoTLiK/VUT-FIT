//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#ifndef IMS_SHIFT_H
#define IMS_SHIFT_H

#include "../common/config.h"
#include "Toilet.h"

class ShiftGenerator : public Event {
    void Behavior() override;
};

class Shift : public Process {
    void Behavior() override;
public:
    Shift() { Activate(); }
};


#endif //IMS_SHIFT_H
