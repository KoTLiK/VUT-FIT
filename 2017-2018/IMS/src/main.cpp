//
// Created by xaugus09 & xkozel18 on 26.11.2017.
//

#include "common/config.h"
#include "processes/Failure.h"
#include "processes/Lunch.h"
#include "processes/Post.h"
#include "processes/Shift.h"
#include "processes/Toilet.h"

Store Type_A("Typ A", TYP_A);
Store Type_B("Typ B", TYP_B);

Facility Line1("#1 Linka");
Queue QStart1;
Store Employees1("#1 Zamestnanci",E1);

Facility Line2("#2 Linka");
Queue QStart2;
Store Employees2("#2 Zamestnanci",E2);

Facility Line3("#3 Linka");
Queue QStart3;
Store Employees3("#3 Zamestnanci",E3);

Queue post1;
Queue post2;
Queue post3;
Process* toilet1 = nullptr;
Process* toilet2 = nullptr;
Process* toilet3 = nullptr;
Process* lunch = nullptr;
Process* shift = nullptr;
double sum1 = 0, sum2 = 0, sum3 = 0,
        sumF = 0, sumF1 = 0, sumF2 = 0, sumF3 = 0;

// 63*YEAR
#define END_TIME (1*YEAR)

int main() {
    /* ============================ INITIALIZATION ============================ */
    RandomSeed(time(nullptr));
    SetOutput("runtime.out");
    /* ============================   SIMULATION   ============================ */
    Init(0,END_TIME);
    (new LunchGenerator(1))->Activate((4*HOUR));
    (new LunchGenerator(2))->Activate((4*HOUR+30*MINUTE));
    (new LunchGenerator(3))->Activate((5*HOUR));
    (new ShiftGenerator())->Activate();
    toilet1 = new Toilet(1);
    toilet2 = new Toilet(2);
    toilet3 = new Toilet(3);
    new Failure();
    new Post1();
    new Post2();
    new Post3();

    (new StatPrint())->Activate(30*MINUTE);

    Run();

    /* ============================     OUTPUT     ============================ */
    SetOutput("usage.out");
    Print("IMS Project in SIMLIB/C++ (2017/2018)\n");
    double l1 = sum1-sumF1, l2 = sum2-sumF2, l3 = sum3-sumF3;
    Print("Usage linky #1 %g %%\nUsage linky #2 %g %%\nUsage linky #3 %g %%\n\n",
          -l1/(END_TIME-sumF1)*100+106.25,
          -l2/(END_TIME-sumF2)*100+106.25,
          -l3/(END_TIME-sumF3)*100+106.25);
    Type_A.Output();
    Type_B.Output();
    return 0;
}