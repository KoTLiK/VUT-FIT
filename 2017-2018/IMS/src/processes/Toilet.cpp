//
// Created by milan on 2.12.2017.
//

#include "Toilet.h"

Toilet::Toilet(int i) {
    this->Priority = 1;
    this->link = i;
    switch (i) {
        case 1: this->store = &Employees1; break;
        case 2: this->store = &Employees2; break;
        case 3: this->store = &Employees3; break;
    }
    Activate();
}

void Toilet::Behavior() {
    ToiletStart:
    this->state = START;
    Wait(Uniform((1.5*HOUR), (2.5*HOUR)));
//    Wait(3*HOUR);
    if (this->state == RESET) { // Reset at Start
        DEBUG_PRINT("RESET " << this->link);
        goto ToiletStart;
    } else if (this->state == LUNCH) { // Enter Lunch state
        goto ToiletLunch;
    }

    ToiletReady:
    this->state = READY;
    if (lunch != nullptr) { // Lunch
        ToiletLunch:
        this->state = LUNCH;
        Passivate(); // Start Lunch
        if (this->state != LUNCH) { // Shift interrupt check
            goto ToiletLunch;
        }
        goto ToiletStart;
    } else if (store->Free() > 0) { // Toilet Enter
        DEBUG_PRINT(" ^^^^^^^^^ WC   active "<< store->Free() <<"  Line: "<<this->link<<"  Time: "<< Time/HOUR);
        Enter(*store, 1);
    } else if (shift != nullptr) { // Reset at Ready
        goto ToiletStart;
    } else {
        Passivate();
        if (this->state == RESET) { // Reset at Ready
            DEBUG_PRINT("RESET " << this->link);
            goto ToiletStart;
        } else if (this->state == LUNCH) { // Enter Lunch state
            goto ToiletLunch;
        }
        goto ToiletReady;
    }

    this->state = ACTIVE;
    Wait(Uniform(20*MINUTE, 25*MINUTE));
    if (this->state == RESET) { // Reset at Active
        Leave(*store, 1); // Toilet Leave (other shift change)
        DEBUG_PRINT("RESET " << this->link);
        goto ToiletStart;
    } else if (this->state == LUNCH) { // Enter Lunch state
        goto ToiletLunch;
    }

    Leave(*store, 1); // Toilet Leave
    goto ToiletStart;
}
