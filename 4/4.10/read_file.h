#ifndef _READ_FILE_H
#define _READ_FILE_H

#define _GNU_SOURCE
#include "interpreter.h"
#include <string.h>

status read_file_with_settings(FILE *file, Current_settings_ptr settings);

#endif