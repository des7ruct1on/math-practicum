#include "headers/logger.h"

status_code create_log(Logger** new_log, char* _info, Severity _sev, char* _op, char* _name_post, int _time_process, my_time _time) {
    if (!strcmp("", _info) || !_info) return code_invalid_parameter;
    (*new_log) = (Logger*)malloc(sizeof(Logger));
    if (!(*new_log)) return code_error_alloc;

    (*new_log)->info = strdup(_info);
    (*new_log)->sev = _sev;
    (*new_log)->name_post = strdup(_name_post);
    (*new_log)->op = strdup(_op);
    (*new_log)->time_process = _time_process;
    (*new_log)->time = _time;

    return code_success;
}

void free_log(Logger* a) {
    if (!a) return;
    free(a->info);
    if (a->name_post) free(a->name_post);
    if (a->op) free(a->op);
    free(a);
    a = NULL;
}

status_code write_log(Logger* new_log) {
    status_code st_act;
    if (!log) return code_invalid_parameter;

    FILE* file = fopen("logs/log.txt", "a+");
    if (!file) return code_invalid_parameter;

    switch (new_log->sev) {
        case NEW_REQUEST:
            fprintf(file, "[%d:%d:%d %d:%d:%d] [NEW_REQUEST] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
        case REQUEST_HANDLING_STARTED:
            fprintf(file, "[%d:%d:%d %d:%d:%d] [REQUEST_HANDLING_STARTED] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
        case REQUEST_HANDLING_FINISHED:
            fprintf(file, "[%d:%d:%d %d:%d:%d] [REQUEST_HANDLING_FINISHED] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
        case DEPARTMENT_OVERLOADED:
            fprintf(file, "[%d:%d:%d %d:%d:%d] [DEPARTMENT_OVERLOADED] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
        case BAD_ALLOC:
            fprintf(file, "[%d:%d:%d %d:%d:%d] [BAD_ALLOC] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
        case INVALID_PARAMETER:
            fprintf(file, "[%d:%d:%d %d:%d:%d] [INVALID_PARAMETER] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
        case ERROR_OPPENING:
            fprintf(file, "[%d:%d:%d %d:%d:%d] [ERROR_OPPENING] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
        case STARTED:
            fprintf(file, "[%d:%d:%d %d:%d:%d] [START] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
        case FINISHED:
            fprintf(file, "[%d:%d:%d %d:%d:%d] [FINISH] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
        case INFO:
            fprintf(file, "     [%d:%d:%d %d:%d:%d] [INFO] %s\n", new_log->time.year, new_log->time.month, new_log->time.day, new_log->time.hour, new_log->time.minute, new_log->time.second, new_log->info);
            break;
    }
    if (st_act != code_success) return st_act;
    free_log(new_log);
    fclose(file);
    return code_success;
}

Severity get_sev_from_status(status_code st) {
    switch(st) {
        case code_error_alloc:
            return BAD_ALLOC;
        case code_invalid_parameter:
            return INVALID_PARAMETER;
        case code_error_oppening:
            return ERROR_OPPENING;
        default:
            return INFO;
    }
}