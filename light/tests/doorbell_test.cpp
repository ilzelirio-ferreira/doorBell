#include "../main/app_doorbell.h"
#include <cassert>
int main() {
    AppDoorbell d;
    assert(!d.sample(false, 0));
    assert(!d.sample(true, 1000));
    assert(!d.sample(false, 10000)); // contact bounce
    assert(!d.sample(true, 20000));
    assert(!d.sample(true, 49999));
    assert(d.sample(true, 50000) && d.active && d.phase);
    d.tick(549999); assert(d.phase);
    d.tick(550000); assert(!d.phase && d.active);
    d.tick(1050000); assert(d.phase);
    d.tick(10049999); assert(d.active);
    d.tick(10050000); assert(!d.active && !d.phase);
    assert(!d.sample(true, 11000000)); // held button never retriggers
    assert(!d.sample(false, 12000000));
    assert(!d.sample(false, 12030000)); // release never rings
    assert(!d.sample(true, 13000000));
    assert(d.sample(true, 13030000));
    assert(!d.sample(false, 14000000)); d.sample(false, 14030000);
    d.sample(true, 15000000); assert(d.sample(true, 15030000));
    d.tick(23030000); assert(d.active); // new press restarts timer
    d.tick(25030000); assert(!d.active && !d.phase);
    AppDoorbell boot;
    boot.sample(true, 0); assert(boot.sample(true, 30000));
}
