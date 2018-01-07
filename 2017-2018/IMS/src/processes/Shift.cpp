//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#include "Shift.h"

void ShiftGenerator::Behavior() {
    new Shift();
    Activate(Time + (8 * HOUR));
}

void Shift::Behavior() {
    shift = this;
    auto* t1 = (Toilet*) toilet1;
    auto* t2 = (Toilet*) toilet2;
    auto* t3 = (Toilet*) toilet3;

    t1->setState(Toilet::RESET);
    t1->Activate();
    t2->setState(Toilet::RESET);
    t2->Activate();
    t3->setState(Toilet::RESET);
    t3->Activate();

    shift = nullptr;
}
