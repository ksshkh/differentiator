#include "input.hpp"

#define SNTX_ERR fprintf(stderr, "syntax error: %c (line: %d)\n", buffer[*ip], __LINE__); \
                 exit(0);

void TokensParcing(Tree* tree, size_t* num_of_nodes, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    size_t data_base_ip = 0;
    size_t tokens_ip   = 0;

    for(data_base_ip = 0; (long int)data_base_ip < tree->size_data_base; tokens_ip++) {

        while(isspace(tree->data_base[data_base_ip])) {
            data_base_ip++;
        }

        if(tree->data_base[data_base_ip] == '$') {
            tree->tokens[tokens_ip] = _EOT;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '(') {
            tree->tokens[tokens_ip] = _L_BR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == ')') {
            tree->tokens[tokens_ip] = _R_BR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '+') {
            tree->tokens[tokens_ip] = _ADD(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '-') {
            tree->tokens[tokens_ip] = _SUB(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '*') {
            tree->tokens[tokens_ip] = _MUL(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '/') {
            tree->tokens[tokens_ip] = _DIV(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '^') {
            tree->tokens[tokens_ip] = _DEG(NULL, NULL);
            data_base_ip++;
        }
        else if('0' <= tree->data_base[data_base_ip] && tree->data_base[data_base_ip] <= '9') {

            double num = 0;
            char* begin = tree->data_base + data_base_ip;

            while(('0' <= tree->data_base[data_base_ip] && tree->data_base[data_base_ip] <= '9') || tree->data_base[data_base_ip] == '.') {
                data_base_ip++;
            }

            char old_sym = tree->data_base[data_base_ip];

            tree->data_base[data_base_ip] = '\0';

            num = strtod(begin, NULL);

            tree->data_base[data_base_ip] = old_sym;

            tree->tokens[tokens_ip] = _NUM(num);
        }
        else if(!strncmp(tree->data_base + data_base_ip, "sin", 3)) {
            tree->tokens[tokens_ip] = _SIN(NULL);
            data_base_ip += 3;
        }
        else if(!strncmp(tree->data_base + data_base_ip, "cos", 3)) {
            tree->tokens[tokens_ip] = _COS(NULL);
            data_base_ip += 3;
        }
        else if(!strncmp(tree->data_base + data_base_ip, "ln", 2)) {
            tree->tokens[tokens_ip] = _LN(NULL);
            data_base_ip += 3;
        }
    }

    for(tokens_ip = 0; (long int)tokens_ip < tree->size_data_base; tokens_ip++) {
        if(tree->tokens[tokens_ip]) printf("%lf\n", tree->tokens[tokens_ip]->data);
    }
}

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

    char old_sym = buffer[*ip];

    buffer[(*ip)] = '\0';

    val = strtod(begin, NULL);

    buffer[*ip] = old_sym;

    if(old_ip == *ip) {SNTX_ERR}

    return _NUM(val);
}

Node* GetVar(size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(buffer       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    char var = 0;
    size_t old_ip = *ip;

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
        char op = buffer[*ip];
        (*ip)++;

        Node* val2 = GetBrackets(num_of_nodes, buffer, ip, code_error);

        switch(op) {
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