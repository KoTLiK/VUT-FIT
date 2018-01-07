//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#ifndef IMS_CONFIG_H
#define IMS_CONFIG_H

#include <iostream>
#include <simlib.h>


#define IMS_DEBUG_
#ifdef IMS_DEBUG
    #define DEBUG_PRINT(x) std::cout << x << std::endl
#else
    #define DEBUG_PRINT(x) //
#endif

#define MINUTE 60
#define HOUR 3600
#define DAY 86400
#define MONTH (30*DAY)
#define YEAR (365*DAY)

/// Sklad A
#define TYP_A 130
#define AWAY_A_FROM 16
#define AWAY_A_TO 22
extern Store Type_A;

/// Sklad B
#define TYP_B 60
#define AWAY_B_FROM 14
#define AWAY_B_TO 20
extern Store Type_B;

/// Stanoviste #1
#define E1 3
#define E1_MAX 3
#define E1_MIN 2
extern Facility Line1;
extern Queue QStart1;
extern Store Employees1;

/// Stanoviste #2
#define E2 3
#define E2_MAX 3
#define E2_MIN 2
extern Facility Line2;
extern Queue QStart2;
extern Store Employees2;

/// Stanoviste #3
#define E3 3
#define E3_MAX 3
#define E3_MIN 2
extern Facility Line3;
extern Queue QStart3;
extern Store Employees3;

extern Queue post1;
extern Queue post2;
extern Queue post3;
extern Process* toilet1;
extern Process* toilet2;
extern Process* toilet3;
extern Process* lunch;
extern Process* shift;

extern double sum1, sum2, sum3, sumF, sumF1, sumF2, sumF3;

class StatPrint : public Event {
    void Behavior() override {
        Print("%d,%d,", (int)Type_A.Used(),(int)Type_B.Used());
        Activate(Time + HOUR);
    }
};

#endif //IMS_CONFIG_H
