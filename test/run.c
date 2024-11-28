#include "cases.h"
#include "testlogger.h"
#include "watchdog.h"

int main(void) {
    TestLogger *tl = create_testlogger();

    test1(tl);

    destroy_testlogger(tl);

    return 0;
}
