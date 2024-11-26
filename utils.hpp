#ifndef UTILS_HPP
#define UTILS_HPP

#include <math.h>

#include "errors.hpp"
#include "tree.hpp"

long int CountSizeFile(FILE* program, int* code_error);

char* ReadInBuff(const char* filename,  long int* stream_size, int* code_error);

void CleanBuffer(void);

char* GetString(FILE* stream, int* code_error);

bool CompareDoubles(double x, double y);

#endif // UTILS_HPP