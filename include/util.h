#pragma once

#ifndef NDEBUG
        #define ERROR(str, label) do {perror(str); goto label;} while(0)
#else
        #define ERROR(str, label) do {} while(0)
#endif

#define CMD_SIZE 4096
