//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#include "Lunch.h"

void LunchGenerator::Behavior() {
    new Lunch(this->link);
    Activate(Time + this->S*HOUR);
    DEBUG_PRINT("REST            Next in "<< this->S << " H     T: "<< Time/HOUR);

    switch (this->S) {
        case H7: this->S = H8; break;
        case H8: this->S = H9; break;
        case H9: this->S = H7; break;
    }
}

Lunch::Lunch(int i) {
    this->Priority = 2;
    this->link = i;
    switch (i) {
        case 1:
            this->employee = &Employees1;
            this->toilet = (Toilet*) toilet1;
            this->Q = &QStart1;
            break;
        case 2:
            this->employee = &Employees2;
            this->toilet = (Toilet*) toilet2;
            this->Q = &QStart2;
            break;
        case 3:
            this->employee = &Employees3;
            this->toilet = (Toilet*) toilet3;
            this->Q = &QStart3;
            break;
    }
    Activate();
}

void Lunch::Behavior() {
    lunch = this;

    if (this->toilet->getState() == Toilet::ACTIVE)
        this->toilet->leaveStore();
    this->toilet->setState(Toilet::LUNCH);
    this->toilet->Activate();
    Enter(*(this->employee), 3);
    DEBUG_PRINT("   #    Lunch next 30min > " << this->link << "  Time: "<<Time/HOUR);
    Wait(30 * MINUTE);
    DEBUG_PRINT("   #    Lunch end        > " << this->link);
    Leave(*(this->employee), 3);
    this->toilet->Activate();
    if (this->Q->Length() > 0)
        (this->Q->GetFirst())->Activate();

    lunch = nullptr;
}
