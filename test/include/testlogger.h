#ifndef TESTLOGGER_H
#define TESTLOGGER_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int totalTestsLogged;
    int totalTestsPassed;
} TestLogger;

TestLogger *create_testlogger(void) {
    TestLogger *tl = (TestLogger *)malloc(sizeof(TestLogger));
    if (tl == NULL) {
        return NULL;
    }

    tl->totalTestsLogged = 0;
    tl->totalTestsPassed = 0;

    return tl;
}

void destroy_testlogger(TestLogger *tl) {
    if (tl == NULL) {
        return;
    }

    free(tl);
}

void logtest(TestLogger *tl, const char *testname, int passed) {
    const int passedBool = passed > 0;

    tl->totalTestsLogged++;
    tl->totalTestsPassed += passedBool;

    printf("%-20s [%c]\n", testname, passedBool ? 'V' : ' ');
}

#endif // !TESTLOGGER_H
