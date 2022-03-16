#include "log.h"

Log gLog;

Log::Log()
{
    log = fopen("log.txt", "w");
}

Log::~Log()
{
    fclose(log);
}

Log::operator FILE*() {
    return log;
}

