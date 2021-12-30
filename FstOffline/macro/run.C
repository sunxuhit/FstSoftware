#include "fstHits.C"

void run() {
    
    fstHits *t11=new fstHits(0,"st_physics_22351023_raw_1500001.fstQa.root");
    t11->Loop();
    fstHits *t13=new fstHits(0,"st_physics_22351023_raw_1500003.fstQa.root");
    t13->Loop();
    fstHits *t14=new fstHits(0,"st_physics_22351023_raw_1500004.fstQa.root");
    t14->Loop();
    fstHits *t15=new fstHits(0,"st_physics_22351023_raw_1500005.fstQa.root");
    t15->Loop();
    fstHits *t16=new fstHits(0,"st_physics_22351023_raw_1500006.fstQa.root");
    t16->Loop();
    fstHits *t17=new fstHits(0,"st_physics_22351023_raw_1500007.fstQa.root");
    t17->Loop();

/*
    fstHits *t0=new fstHits(0,"st_physics_22351023_raw.fstQa.root");
    t0->Loop();

    fstHits *t1=new fstHits(0,"st_physics_22351015_raw_1500001.fstQa.root");
    t1->Loop();

    fstHits *t2=new fstHits(0,"st_physics_22351021_raw_1500001.fstQa.root");
    t2->Loop();

    fstHits *t3=new fstHits(0,"st_physics_22351023_raw_1500001.fstQa.root");
    t3->Loop();

    fstHits *t4=new fstHits(0,"st_physics_22351026_raw_1500001.fstQa.root");
    t4->Loop();

    fstHits *t5=new fstHits(0,"st_physics_22351028_raw_1500001.fstQa.root");
    t5->Loop();
 */
}
