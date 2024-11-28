#ifndef TEST1_H
#define TEST1_H

#include "testlogger.h"
#include "watchdog.h"
#include <unistd.h>

int callback_called = 0;

void callback_test1(Watchdog *watchdog, uint32_t mask) { callback_called = 1; }

void test1(TestLogger *tl) {
    system("echo 'test1' > test1.txt");

    Watchdog *watchdog =
        create_watchdog("test1.txt", IN_MODIFY, callback_test1);

    spawn_watchdog_thread(watchdog);

    sleep(1);
    system("echo 'test1aaaa' > test1.txt");

    destroy_watchdog(watchdog);

    remove("test1.txt");

    logtest(tl, "test1", callback_called);
}

#endif // TEST1_H
