#ifndef LOG_H_SENTRY
#define LOG_H_SENTRY

#include <cstdio>

class Log {
public:
    FILE *log;
    Log();
    ~Log();
    operator FILE*();
};

#endif
