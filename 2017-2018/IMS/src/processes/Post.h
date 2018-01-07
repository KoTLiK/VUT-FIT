//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#ifndef IMS_POST_H
#define IMS_POST_H

#include "../common/config.h"
#include "Toilet.h"

class Post : public Process {
protected:
    double before{}, after{}, timeout;
    int state{};
public:
    enum Speed { FAST, SLOW };
    enum State { CALL, LOAD, SEND, AWAY, OUT };
    Post() : timeout(0) {}
    int getState() { return state; }
    Post* setTimeout(double time) { this->timeout = time; return this; }
};

class Post1 : public Post {
    void Behavior() override;
public:
    Post1() { Activate(); }
private:
    void LineLoading(Store &, unsigned, Speed);
    void TimeAway(Store &, double, double);
};

class Post2 : public Post {
    void Behavior() override;
public:
    Post2() { Activate(); }
private:
    void LineLoading(Store &, unsigned , Speed);
    void TimeAway(Store &, double, double);
};

class Post3 : public Post {
    void Behavior() override;
public:
    Post3() { Activate(); }
private:
    void LineLoading(Store &, unsigned, Speed);
    void TimeAway(Store &, double, double);
};

#endif //IMS_POST_H
