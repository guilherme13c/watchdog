#if __linux__

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>

#define WATCHDOG_CALLBACK_T(name)                                              \
    void (*name)(struct Watchdog_t * watchdog, uint32_t mask)

#define WATCHDOG_FILENAME_MAX 256

typedef struct Watchdog_t {
    int fd;
    int wd;
    uint32_t mask;
    char path[WATCHDOG_FILENAME_MAX];
    WATCHDOG_CALLBACK_T(callback);
    pthread_t *thread;
} Watchdog;

Watchdog *create_watchdog(const char *path, uint32_t mask,
                          WATCHDOG_CALLBACK_T(callback)) {
    Watchdog *watchdog = (Watchdog *)malloc(sizeof(Watchdog));
    if (watchdog == NULL) {
        return NULL;
    }

    watchdog->fd = inotify_init();
    if (watchdog->fd < 0) {
        free(watchdog);
        return NULL;
    }

    watchdog->wd = inotify_add_watch(watchdog->fd, path, mask);
    if (watchdog->wd < 0) {
        close(watchdog->fd);
        free(watchdog);
        return NULL;
    }

    watchdog->mask = IN_MODIFY | IN_CREATE | IN_DELETE;
    strncpy(watchdog->path, path, WATCHDOG_FILENAME_MAX);
    watchdog->callback = callback;

    return watchdog;
}

void destroy_watchdog(Watchdog *watchdog) {
    if (watchdog == NULL) {
        return;
    }

    if (watchdog->thread != NULL) {
        pthread_cancel(*watchdog->thread);
        free(watchdog->thread);
    }

    close(watchdog->fd);
    free(watchdog);
}

void *check_watchdog(void *arg) {
    Watchdog *watchdog = (Watchdog *)arg;
    if (watchdog == NULL) {
        perror("check_watchdog: watchdog is NULL");
        return NULL;
    }

    char buffer[WATCHDOG_FILENAME_MAX];
    ssize_t length;
    length = read(watchdog->fd, buffer, WATCHDOG_FILENAME_MAX);
    if (length < 0) {
        return NULL;
    }

    struct inotify_event *event = (struct inotify_event *)&buffer;

    WATCHDOG_CALLBACK_T(callback) = watchdog->callback;
    if (callback != NULL) {
        callback(watchdog, event->mask);
    }

    return NULL;
}

void spawn_watchdog_thread(Watchdog *watchdog) {
    pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t));
    if (thread == NULL) {
        return;
    }

    int status = pthread_create(thread, NULL, check_watchdog, (void *)watchdog);
    if (status != 0) {
        free(thread);
    }

    watchdog->thread = thread;
}

#endif // !WATCHDOG_H

#endif // __linux__
