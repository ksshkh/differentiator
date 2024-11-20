#include "errors.hpp"

static const char* errors_names[] = {"NO_ERROR",
                                     "PTR_ERROR",
                                     "SIZE_ERROR",
                                     "STACK_UNDERFLOW",
                                     "NO_STACK",
                                     "FILE_ERROR",
                                     "FOPEN_ERROR",
                                     "FCLOSE_ERROR",
                                     "READ_ERROR",
                                     "BAD_CAPACITY",
                                     "NO_DATA",
                                     "BAD_DATA_RIGHT_CANARY",
                                     "BAD_DATA_LEFT_CANARY",
                                     "BAD_STACK_RIGHT_CANARY",
                                     "BAD_STACK_LEFT_CANARY",
                                     "BAD_DATA_HASH",
                                     "BAD_STACK_HASH",
                                     "BAD_HASH",
                                     "BAD_DATA_CANARIES",
                                     "BAD_STACK_CANARIES",
                                     "NO_TREE",
                                     "NO_ROOT",
                                     "BASE_ERROR",
                                     "CONNECT_ERROR"};

void ErrorsPrint(FILE* stream, int* code_error) {
    for (int i = 0; i < N_ERROR; i++) {
        if (*code_error & (1 << i)) {
            fprintf(stream, ERR("%s"), errors_names[i]);
        }
    }
}