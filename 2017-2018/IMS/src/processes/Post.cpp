//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#include "Post.h"

void Post1::Behavior() {
    Post1Start:
    if (!Line1.Busy() && !Type_A.Full() && (Employees1.Free() == E1_MAX)) {
        LineLoading(Type_A, E1_MAX, FAST);
        TimeAway(Type_A, AWAY_A_FROM, AWAY_A_TO);
    } else if (!Line1.Busy() && !Type_A.Full() && (Employees1.Free() == E1_MIN)) {
        LineLoading(Type_A, E1_MIN, SLOW);
        TimeAway(Type_A, AWAY_A_FROM, AWAY_A_TO);
    } else {
        before = Time;
        Into(QStart1);
        Passivate();
        after = Time;
        sum1 += (after - before);
        goto Post1Start;
    }
}

void Post1::LineLoading(Store &type, unsigned employees, Speed speed) {
    /// ============== CALL
    this->state = CALL;
    Seize(Line1);
    Enter(type, 1);
    Enter(Employees1, employees);
    Wait(25);
    /// ============== LOAD
    this->state = LOAD;
    double time;
    if (speed == FAST)
        time = Uniform(MINUTE*5,MINUTE*25);
    else time = Uniform(MINUTE*9,MINUTE*35);
    Wait(time);
    /// ============== SEND
    this->state = SEND;
    Wait(MINUTE);
    Leave(Employees1, employees);
    auto* t = (Toilet*) toilet1;
    if (t->getState() == Toilet::READY)
        t->Activate();

    Release(Line1);

    /// Resume passivated process
    if (QStart1.Length() > 0)
        (QStart1.GetFirst())->Activate();
    else new Post1();

    if (QStart2.Length() > 0)
        (QStart2.GetFirst())->Activate();

    if (QStart3.Length() > 0)
        (QStart3.GetFirst())->Activate();
}


void Post1::TimeAway(Store &type, const double from, const double to) {
    this->state = AWAY;
    double time = Uniform(from * HOUR, to * HOUR);
    double awayTime = Time + time;
    Into(post1);
    Wait(time);
    time = awayTime - Time;
    while ((long)time > 0) {
        DEBUG_PRINT("    $$$  1   Out: "<< this->timeout/HOUR << " Left: " << time/HOUR << " Time: " << Time/HOUR);
        awayTime = Time + time + this->timeout;
        Wait(time + this->timeout);
        time = awayTime - Time;
    }
    this->state = OUT;
    for (Queue::iterator p = post1.begin(); p != post1.end(); ++p)
        if (((Post*)(*p)) == this) post1.Get(p);

    Leave(type, 1);

    if (QStart1.Length() > 0)
        (QStart1.GetFirst())->Activate();
    if (QStart2.Length() > 0)
        (QStart2.GetFirst())->Activate();
    if (QStart3.Length() > 0)
        (QStart3.GetFirst())->Activate();
}

void Post2::Behavior() {
    Post2Start:
    if (!Line2.Busy() && !Type_B.Full() && (Employees2.Free() == E2_MAX)) { // P = 3
        LineLoading(Type_B, E2_MAX, FAST);
        TimeAway(Type_B, AWAY_B_FROM, AWAY_B_TO);
    } else if (!Line2.Busy() && !Type_B.Full() && (Employees2.Free() == E2_MIN)) { // P = 2
        LineLoading(Type_B, E2_MIN, SLOW);
        TimeAway(Type_B, AWAY_B_FROM, AWAY_B_TO);
    } else if (!Line2.Busy() && !Type_A.Full() && (Employees2.Free() == E2_MAX)) { // P = 1
        LineLoading(Type_A, E2_MAX, FAST);
        TimeAway(Type_A, AWAY_A_FROM, AWAY_A_TO);
    } else if (!Line2.Busy() && !Type_A.Full() && (Employees2.Free() == E2_MIN)) { // P = 0
        LineLoading(Type_A, E2_MIN, SLOW);
        TimeAway(Type_A, AWAY_A_FROM, AWAY_A_TO);
    } else {
        before = Time;
        Into(QStart2);
        Passivate();
        after = Time;
        sum2 += (after - before);
        goto Post2Start;
    }
}

void Post2::LineLoading(Store &type, const unsigned employees, Speed speed) {
    /// ============== CALL
    this->state = CALL;
    Seize(Line2);
    Enter(type, 1);
    Enter(Employees2, employees);
    Wait(40);
    /// ============== LOAD
    this->state = LOAD;
    double time;
    if (speed == FAST)
        time = Uniform(MINUTE*5,MINUTE*25);
    else time = Uniform(MINUTE*9,MINUTE*35);
    Wait(time);
    /// ============== SEND
    this->state = SEND;
    Wait(MINUTE);
    Leave(Employees2, employees);
    auto* t = (Toilet*) toilet2;
    if (t->getState() == Toilet::READY)
        t->Activate();

    Release(Line2);

    /// Resume passivated process
    if (QStart1.Length() > 0)
        (QStart1.GetFirst())->Activate();

    if (QStart2.Length() > 0)
        (QStart2.GetFirst())->Activate();
    else new Post2();

    if (QStart3.Length() > 0)
        (QStart3.GetFirst())->Activate();
}


void Post2::TimeAway(Store &type, const double from, const double to) {
    this->state = AWAY;
    double time = Uniform(from * HOUR, to * HOUR);
    double awayTime = Time + time;
    Into(post2);
    Wait(time);
    time = awayTime - Time;
    while ((long)time > 0) {
        DEBUG_PRINT("    $$$  2   Out: "<< this->timeout/HOUR << " Left: " << time/HOUR << " Time: " << Time/HOUR);
        awayTime = Time + time + this->timeout;
        Wait(time + this->timeout);
        time = awayTime - Time;
    }
    this->state = OUT;
    for (Queue::iterator p = post2.begin(); p != post2.end(); ++p)
        if (((Post*)(*p)) == this) post2.Get(p);

    Leave(type, 1);

    if (QStart1.Length() > 0)
        (QStart1.GetFirst())->Activate();
    if (QStart2.Length() > 0)
        (QStart2.GetFirst())->Activate();
    if (QStart3.Length() > 0)
        (QStart3.GetFirst())->Activate();
}

void Post3::Behavior() {
    Post3Start:
    if (!Line3.Busy() && !Type_A.Full() && (Employees3.Free() == E3_MAX)) {
        LineLoading(Type_A, E3_MAX, FAST);
        TimeAway(Type_A, AWAY_A_FROM, AWAY_A_TO);
    } else if (!Line3.Busy() && !Type_A.Full() && (Employees3.Free() == E3_MIN)) {
        LineLoading(Type_A, E3_MIN, SLOW);
        TimeAway(Type_A, AWAY_A_FROM, AWAY_A_TO);
    } else {
        before = Time;
        Into(QStart3);
        Passivate();
        after = Time;
        sum3 += (after - before);
        goto Post3Start;
    }
}

void Post3::LineLoading(Store &type, unsigned employees, Speed speed) {
    /// ============== CALL
    this->state = CALL;
    Seize(Line3);
    Enter(type, 1);
    Enter(Employees3, employees);
    Wait(MINUTE);
    /// ============== LOAD
    this->state = LOAD;
    double time;
    if (speed == FAST)
        time = Uniform(MINUTE*5,MINUTE*25);
    else time = Uniform(MINUTE*9,MINUTE*35);
    Wait(time);
    /// ============== SEND
    this->state = SEND;
    Wait(MINUTE);
    Leave(Employees3, employees);
    auto* t = (Toilet*) toilet3;
    if (t->getState() == Toilet::READY)
        t->Activate();

    Release(Line3);

    /// Resume passivated process
    if (QStart1.Length() > 0)
        (QStart1.GetFirst())->Activate();

    if (QStart2.Length() > 0)
        (QStart2.GetFirst())->Activate();

    if (QStart3.Length() > 0)
        (QStart3.GetFirst())->Activate();
    else new Post3();
}

void Post3::TimeAway(Store &type, const double from, const double to) {
    this->state = AWAY;
    double time = Uniform(from * HOUR, to * HOUR);
    double awayTime = Time + time;
    Into(post3);
    Wait(time);
    time = awayTime - Time;
    while ((long)time > 0) {
        DEBUG_PRINT("    $$$  3   Out: "<< this->timeout/HOUR << " Left: " << time/HOUR << " Time: " << Time/HOUR);
        awayTime = Time + time + this->timeout;
        Wait(time + this->timeout);
        time = awayTime - Time;
    }
    this->state = OUT;
    for (Queue::iterator p = post3.begin(); p != post3.end(); ++p)
        if (((Post*)(*p)) == this) post3.Get(p);

    Leave(type, 1);

    if (QStart1.Length() > 0)
        (QStart1.GetFirst())->Activate();
    if (QStart2.Length() > 0)
        (QStart2.GetFirst())->Activate();
    if (QStart3.Length() > 0)
        (QStart3.GetFirst())->Activate();
}
