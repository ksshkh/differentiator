#include "../inc/input.hpp"

#define SNTX_ERR fprintf(stderr, "syntax error: %c (line: %d)\n", (char)tokens[*ip].value, __LINE__); \
                 exit(0);

void TokensParcing(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    size_t data_base_ip = 0;
    size_t tokens_ip   = 0;

    for(data_base_ip = 0; (long int)data_base_ip < tree->size_data_base; tokens_ip++) {

        while(isspace(tree->data_base[data_base_ip])) {
            data_base_ip++;
        }

        if(tree->data_base[data_base_ip] == 'x') {
            tree->tokens[tokens_ip].type  = VAR;
            tree->tokens[tokens_ip].value = tree->data_base[data_base_ip];
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '(') {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = L_BR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == ')') {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = R_BR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '+') {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = ADD;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '-') {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = SUB;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '*') {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = MUL;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '/') {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = DIV;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '^') {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = DEG;
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

            tree->tokens[tokens_ip].type  = NUM;
            tree->tokens[tokens_ip].value = num;
        }
        else if(!strncmp(tree->data_base + data_base_ip, "sin", 3)) {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = SIN;
            data_base_ip += 3;
        }
        else if(!strncmp(tree->data_base + data_base_ip, "cos", 3)) {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = COS;
            data_base_ip += 3;
        }
        else if(!strncmp(tree->data_base + data_base_ip, "ln", 2)) {
            tree->tokens[tokens_ip].type  = OP;
            tree->tokens[tokens_ip].value = LN;
            data_base_ip += 3;
        }
    }

    tree->tokens[tokens_ip].type  = OP;
    tree->tokens[tokens_ip].value = EOT;
}

Node* GetTree(size_t* num_of_nodes, Token* tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* val = GetAddAndSub(num_of_nodes, tokens, ip, code_error);

    if((Operations)tokens[*ip].value != EOT) {SNTX_ERR}
    (*ip)++;

    return val;
}

Node* GetAddAndSub(size_t* num_of_nodes, Token* tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetMulAndDiv(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip].value == ADD || (Operations)tokens[*ip].value == SUB) {
        Operations op = (Operations)tokens[*ip].value;
        (*ip)++;

        Node* right_node = GetMulAndDiv(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wswitch-enum"

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

        #pragma GCC diagnostic pop
    }

    return node;
}

Node* GetMulAndDiv(size_t* num_of_nodes, Token* tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetDeg(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip].value == MUL || (Operations)tokens[*ip].value == DIV) {
        Operations op = (Operations)tokens[*ip].value;
        (*ip)++;

        Node* right_node = GetDeg(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wswitch-enum"

        switch(op) {
            case MUL: {
                node = _MUL(left_node, right_node);
                break;
            }
            case DIV: {
                node = _DIV(left_node, right_node);
                break;
            }
            default: {
                break;
            }
        }

        #pragma GCC diagnostic pop
    }

    return node;
}

Node* GetDeg(size_t* num_of_nodes, Token* tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetUnaryOp(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip].value == DEG) {
        Operations op = (Operations)tokens[*ip].value;
        (*ip)++;

        Node* node_right = GetUnaryOp(num_of_nodes, tokens, ip, code_error);
        Node* node_left  = node;

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wswitch-enum"

        switch(op) {
            case DEG: {
                node = _DEG(node_left, node_right);
                break;
            }
            default: {
                break;
            }
        }

        #pragma GCC diagnostic pop
    }

    return node;
}

Node* GetUnaryOp(size_t* num_of_nodes, Token* tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    while(tokens[*ip].type == OP && (Operations)tokens[*ip].value != L_BR && (Operations)tokens[*ip].value != R_BR) {

        Operations op = (Operations)tokens[*ip].value;
        (*ip)++;

        Node* left_node = GetBrackets(num_of_nodes, tokens, ip, code_error);

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wswitch-enum"

        switch(op) {
            case SIN: {
                return _SIN(left_node);
            }
            case COS: {
                return _COS(left_node);
            }
            case LN: {
                return _LN(left_node);
            }
            default: {
                break;
            }
        }

        #pragma GCC diagnostic pop
    }
    Node* node = GetBrackets(num_of_nodes, tokens, ip, code_error);

    return node;
}

Node* GetBrackets(size_t* num_of_nodes, Token* tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if((Operations)tokens[*ip].value == L_BR) {
        (*ip)++;
        Node* node = GetAddAndSub(num_of_nodes, tokens, ip, code_error);

        if((Operations)tokens[*ip].value != R_BR) {SNTX_ERR}

        (*ip)++;
        return node;
    }
    else if(tokens[*ip].type == VAR) {
        return GetVar(num_of_nodes, tokens, ip, code_error);
    }
    else {
        return GetNum(num_of_nodes, tokens, ip, code_error);
    }
}

Node* GetNum(size_t* num_of_nodes, Token* tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    size_t old_ip = *ip;

    Node* node = _NUM(tokens[*ip].value);
    if(tokens[*ip].type == NUM) (*ip)++;

    if(old_ip == *ip) {SNTX_ERR}

    return node;
}

Node* GetVar(size_t* num_of_nodes, Token* tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    size_t old_ip = *ip;

    Node* node = _VAR(tokens[*ip].value);
    if(tokens[*ip].type == VAR) (*ip)++;

    if(old_ip == *ip) {SNTX_ERR}

    return node;
}

#undef SNTX_ERROR