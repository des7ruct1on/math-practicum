#ifndef logger_h
#define logger_h

#include "struct.h"

typedef enum Severity {
    NEW_REQUEST,
    REQUEST_HANDLING_STARTED,
    REQUEST_HANDLING_FINISHED,
    DEPARTMENT_OVERLOADED,
    BAD_ALLOC,
    INVALID_PARAMETER,
    ERROR_OPPENING,
    STARTED,
    FINISHED,
    INFO
} Severity;

typedef struct Logger {
    my_time time;
    Severity sev;
    char* info;
    char* op;
    char* name_post;
    int time_process;
} Logger;

status_code create_log(Logger** new_log, char* _info, Severity _sev, char* _op, char* _name_post, int _time_process, my_time _time);
void free_log(Logger* a);
status_code write_log(Logger* new_log);
Severity get_sev_from_status(status_code st);
#endif