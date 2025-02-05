#include "../inc/utils.hpp"

long int CountSizeFile(FILE* program, int* code_error) {

    MY_ASSERT(program != NULL, FILE_ERROR);

    struct stat file_info = {};
    fstat(fileno(program), &file_info);

    return file_info.st_size;
}

char* ReadInBuff(const char* filename, long int* stream_size, int* code_error) {

    FILE* stream = fopen(filename, "r");
    MY_ASSERT(stream != NULL, FOPEN_ERROR);

    *stream_size = CountSizeFile(stream, code_error);

    char *buffer = (char*)calloc(*stream_size, sizeof(char));
    MY_ASSERT(buffer != NULL, PTR_ERROR);

    MY_ASSERT(fread(buffer, sizeof(char), *stream_size, stream) == (size_t)*stream_size, READ_ERROR);

    MY_ASSERT(fclose(stream) == 0, FCLOSE_ERROR);

    return buffer;
}

void CleanBuffer(void) {
    int c = 0;

    while (((c = getchar()) != '\n') && c != EOF) {}
}

char* GetString(FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    size_t max_len = 10;
    size_t len = 0;

    char* string = (char*)calloc(max_len, sizeof(char));
    MY_ASSERT(string != NULL, PTR_ERROR);

    char ch = 0;

    while((ch = (char)fgetc(stream)) != '\n') {
        string[len++] = ch;

        if(len == max_len) {
            max_len *= 2;

            string = (char*)realloc(string, max_len);
            MY_ASSERT(string != NULL, PTR_ERROR);
        }
    }

    string[len++] = '\0';

    string = (char*)realloc(string, len);
    MY_ASSERT(string != NULL, PTR_ERROR);

    return string;
 }

bool CompareDoubles(double x, double y) {
    const double EPS = 1e-9;

    return fabs(x - y) < EPS;
}