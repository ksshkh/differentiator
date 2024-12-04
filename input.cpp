#include "input.hpp"

#define SNTX_ERR fprintf(stderr, "syntax error: %c (line: %d)\n", (char)tokens[*ip]->data, __LINE__); \
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
        else if(tree->data_base[data_base_ip] == 'x') {
            tree->tokens[tokens_ip] = _VAR(tree->data_base[data_base_ip]);
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

Node* GetTree(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if(tokens[*ip]->data != EOT) {SNTX_ERR}
    (*ip)++;

    Node* val = GetAddAndSub(num_of_nodes, tokens, ip, code_error);

    if(tokens[*ip]->data != EOT) {SNTX_ERR}
    (*ip)++;

    return val;
}

Node* GetNum(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    size_t old_ip = *ip;

    Node* ret_node = tokens[*ip];
    if(tokens[*ip]->type == NUM) (*ip)++;

    if(old_ip == *ip) {SNTX_ERR}

    return ret_node;
}

Node* GetVar(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    size_t old_ip = *ip;

    Node* ret_node = tokens[*ip];
    if(tokens[*ip]->type == VAR) (*ip)++;

    if(old_ip == *ip) {SNTX_ERR}

    return ret_node;
}

Node* GetDeg(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetBrackets(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip]->data == DEG) {
        Operations op = (Operations)tokens[*ip]->data;
        (*ip)++;

        Node* node_right = GetBrackets(num_of_nodes, tokens, ip, code_error);
        Node* node_left  = node;

        switch(op) {
            case DEG: {
                node = _DEG(node_left, node_right);
                break;
            }
            default: {
                break;
            }
        }
    }

    return node;

}

Node* GetAddAndSub(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetMulAndDiv(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip]->data == ADD || (Operations)tokens[*ip]->data == SUB) {
        Operations op = (Operations)tokens[*ip]->data;
        (*ip)++;

        Node* right_node = GetMulAndDiv(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        switch(op) {
            case ADD: {
                node = _ADD(left_node, right_node);
                break;
            }
            case SUB: {
                node = _SUB(left_node, right_node);
                break;
            }
            default: {
                break;
            }
        }
    }

    return node;
}

Node* GetMulAndDiv(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetDeg(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip]->data == MUL || (Operations)tokens[*ip]->data == DIV) {
        Operations op = (Operations)tokens[*ip]->data;
        (*ip)++;

        Node* right_node = GetDeg(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        switch(op) {
            case MUL: {
                node = _MUL(left_node, right_node);
                break;
            }
            case DIV: {
                node = _MUL(left_node, right_node);
                break;
            }
            default: {
                break;
            }
        }
    }

    return node;
}

Node* GetBrackets(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if((Operations)tokens[*ip]->data == L_BR) {
        (*ip)++;
        Node* node = GetAddAndSub(num_of_nodes, tokens, ip, code_error);

        if((Operations)tokens[*ip]->data != R_BR) {SNTX_ERR}

        (*ip)++;
        return node;
    }
    else if(tokens[*ip]->type == VAR) {
        return GetVar(num_of_nodes, tokens, ip, code_error);
    }
    else {
        return GetNum(num_of_nodes, tokens, ip, code_error);
    }
}

#undef SNTX_ERROR