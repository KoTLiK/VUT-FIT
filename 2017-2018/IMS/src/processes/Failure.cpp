//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#include "Failure.h"

void Failure::Behavior() {
    while (true) {
        Wait(Exponential(DAY));

        double time = Exponential(2*HOUR);
        DEBUG_PRINT(" ----- FAILURE START ----- "<< Time/HOUR << "  Timeout: " << time/HOUR);
        sumF += time;
        for (Queue::iterator p = post1.begin(); p != post1.end(); ++p) {
            Post* position = (Post*)(*p);
            if (position->getState() == Post::AWAY)
                position->setTimeout(time)->Activate();
        }
        for (Queue::iterator p = post2.begin(); p != post2.end(); ++p) {
            Post* position = (Post*)(*p);
            if (position->getState() == Post::AWAY)
                position->setTimeout(time)->Activate();
        }
        for (Queue::iterator p = post3.begin(); p != post3.end(); ++p) {
            Post* position = (Post*)(*p);
            if (position->getState() == Post::AWAY)
                position->setTimeout(time)->Activate();
        }

        new Control(1, time);
        new Control(2, time);
        new Control(3, time);

        Wait(time);
    }
}

Control::Control(int i, double t) {
    this->Priority = 1;
    this->link = i;
    this->wait = t;
    switch (i) {
        case 1: this->line = &Line1; break;
        case 2: this->line = &Line2; break;
        case 3: this->line = &Line3; break;
    }
    Activate();
}

void Control::Behavior() {
    double now = Time;
    Seize(*(this->line));
    double diff = Time - now;
    DEBUG_PRINT(" ----- Control  ----- "<<this->link<<" now: "<<now/HOUR<<" time: "<<Time/HOUR<<" diff: "<<diff/HOUR);
    if (this->wait-diff > 0) {
        switch (this->link) {
            case 1: sumF1 += this->wait-diff; break;
            case 2: sumF2 += this->wait-diff; break;
            case 3: sumF3 += this->wait-diff; break;
        }
        Wait(this->wait-diff);
    }
    Release(*(this->line));
    DEBUG_PRINT(" ----- FAILURE END ----- "<<this->link<<"  time: "<<Time/HOUR);
}
