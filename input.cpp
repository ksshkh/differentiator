#include "input.hpp"

#define SNTX_ERR fprintf(stderr, "syntax error: %c\n", buffer[*ip]); \
                 exit(0);

Node* GetTree(size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(buffer       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if(buffer[*ip] != '$') {SNTX_ERR}
    (*ip)++;

    Node* val = GetAddAndSub(num_of_nodes, buffer, ip, code_error);

    if(buffer[*ip] != '$') {SNTX_ERR}
    (*ip)++;

    return val;
}

Node* GetNum(size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(buffer       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    double val = 0;
    size_t old_ip = *ip;

    char* begin = buffer + *ip;

    while(('0' <= buffer[*ip] && buffer[*ip] <= '9') || buffer[*ip] == '.') {
        (*ip)++;
    }

    char old_s = buffer[*ip];

    buffer[(*ip)] = '\0';

    val = strtod(begin, NULL);

    buffer[*ip] = old_s;

    if(old_ip == *ip) {SNTX_ERR}

    return _NUM(val);
}

Node* GetVar(size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(buffer       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    char var = 0;
    size_t old_ip = *ip;

    char* begin = buffer + *ip;

    if(buffer[*ip] == X) {
        var = X;
        (*ip)++;
    }

    if(old_ip == *ip) {SNTX_ERR}

    return _VAR(var);
}

Node* GetDeg(size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(buffer       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* val = GetBrackets(num_of_nodes, buffer, ip, code_error);

    while(buffer[*ip] == '^') {
        double op = buffer[*ip];
        (*ip)++;

        Node* val2 = GetBrackets(num_of_nodes, buffer, ip, code_error);

        switch((int)op) {
            case '^': {
                val = _DEG(val, val2);
                break;
            }
            default: {
                break;
            }
        }
    }

    return val;

}

Node* GetAddAndSub(size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(buffer       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* val = GetMulAndDiv(num_of_nodes, buffer, ip, code_error);

    while(buffer[*ip] == '+' || buffer[*ip] == '-') {
        char op = buffer[*ip];
        (*ip)++;

        Node* val2 = GetMulAndDiv(num_of_nodes, buffer, ip, code_error);

        switch(op) {
            case '+': {
                val = _ADD(val, val2);
                break;
            }
            case '-': {
                val = _SUB(val, val2);
                break;
            }
            default: {
                break;
            }
        }
    }

    return val;
}

Node* GetMulAndDiv(size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(buffer       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* val = GetDeg(num_of_nodes, buffer, ip, code_error);

    while(buffer[*ip] == '*' || buffer[*ip] == '/') {
        char op = buffer[*ip];
        (*ip)++;

        Node* val2 = GetDeg(num_of_nodes, buffer, ip, code_error);

        switch(op) {
            case '*': {
                val = _MUL(val, val2);
                break;
            }
            case '/': {
                val = _DIV(val, val2);
                break;
            }
            default: {
                break;
            }
        }
    }

    return val;
}

Node* GetBrackets(size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(buffer       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if(buffer[*ip] == '(') {
        (*ip)++;
        Node* val = GetAddAndSub(num_of_nodes, buffer, ip, code_error);

        if(buffer[*ip] != ')') {SNTX_ERR}

        (*ip)++;
        return val;
    }
    else if(buffer[*ip] == X) {
        return GetVar(num_of_nodes, buffer, ip, code_error);
    }
    else {
        return GetNum(num_of_nodes, buffer, ip, code_error);
    }
}

#undef SNTX_ERROR