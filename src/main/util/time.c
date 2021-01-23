#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "util/time.h"

#define SEC_PER_DAY   86400
#define SEC_PER_HOUR  3600
#define SEC_PER_MIN   60

void util_time_sleep_ns(uint64_t time_ns)
{
    struct timespec t;
    t.tv_sec = (__time_t) (time_ns / (1000 * 1000 * 1000));
    t.tv_nsec = (__syscall_slong_t) time_ns % (1000 * 1000 * 1000);

    nanosleep(&t, NULL);
}

void util_time_sleep_us(uint64_t time_us)
{
    struct timespec t;
    t.tv_sec = (__time_t) (time_us / (1000 * 1000));
    t.tv_nsec = (__syscall_slong_t) (time_us % (1000 * 1000)) * 1000;

    nanosleep(&t, NULL);
}

void util_time_sleep_ms(uint64_t time_ms)
{
    struct timespec t;
    t.tv_sec = (__time_t) (time_ms / 1000);
    t.tv_nsec = (__syscall_slong_t) (time_ms % 1000) * 1000 * 1000;

    nanosleep(&t, NULL);
}

void util_time_sleep_sec(uint64_t time_sec)
{
    struct timespec t;
    t.tv_sec = (__time_t) time_sec;
    t.tv_nsec = 0;

    nanosleep(&t, NULL);
}

void util_time_get_current_time(struct util_time_timestamp* timestamp)
{
    if (!timestamp) {
        return;
    }

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    timestamp->millisec = tv.tv_usec / 1000;
    timestamp->sec = tm->tm_sec;
    timestamp->min = tm->tm_min;
    timestamp->hour = tm->tm_hour;
    timestamp->day = tm->tm_mday;
    // Months since January [0, 11]
    timestamp->month = tm->tm_mon + 1;
    // Years since 1900
    timestamp->year = tm->tm_year + 1900;
}

uint64_t util_time_now_ns()
{
    struct timespec ts;

    clock_gettime(0, &ts);

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

uint64_t util_time_delta_ns(uint64_t prev_ns, uint64_t now_ns)
{
    return now_ns - prev_ns;
}

double util_time_delta_us(uint64_t prev_ns, uint64_t now_ns)
{
    return util_time_delta_ns(prev_ns, now_ns) / 1000.0;
}

double util_time_delta_ms(uint64_t prev_ns, uint64_t now_ns)
{
    return util_time_delta_ns(prev_ns, now_ns) / 1000.0 / 1000.0;
}

double util_time_delta_sec(uint64_t prev_ns, uint64_t now_ns)
{
    return util_time_delta_ns(prev_ns, now_ns) / 1000.0 / 1000.0 / 1000.0;
}