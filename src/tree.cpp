#include "../inc/tree.hpp"
#include "../inc/utils.hpp"
#include "../inc/treedump.hpp"
#include "../inc/input.hpp"

static const char* INPUT_FILE = "./input.txt";

#define OP_CHECK(data)  data == ADD || data == SUB || data == MUL || data == DIV || data == DEG || !strncmp(&data, "sin", 3) || !strncmp(&data, "cos", 3) || !strncmp(&data, "ln", 2)
#define VAR_CHECK(data) data == X

void TreeCtor(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    tree->num_of_nodes = 0;
    tree->root = NULL;

    tree->data_base = ReadInBuff(INPUT_FILE, &(tree->size_data_base), code_error);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    tree->tokens = (Token*)calloc(tree->size_data_base, sizeof(Token));
    MY_ASSERT(tree->tokens != NULL, PTR_ERROR);
}

Node* NodeCtor(size_t* num_of_nodes, Type type, TreeElem data, Node* left, Node* right, Node* parent, int* code_error) {

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    MY_ASSERT(new_node != NULL, PTR_ERROR);

    (*num_of_nodes)++;

    new_node->type = type;
    new_node->data = data;
    new_node->left = left;
    new_node->right = right;
    new_node->parent = parent;

    if(left)  left->parent  = new_node;
    if(right) right->parent = new_node;

    return new_node;

}

void AddNewNode(size_t* num_of_nodes, Type type, Node* node, TreeElem data, Side side, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    switch(side) {
        case LEFT: {
            Node* new_node = NodeCtor(num_of_nodes, type, data, node->left, NULL, node, code_error);
            MY_ASSERT(new_node != NULL, PTR_ERROR);

            node->left = new_node;
            break;
        }
        case RIGHT: {
            Node* new_node = NodeCtor(num_of_nodes, type, data, NULL, node->right, node, code_error);
            MY_ASSERT(new_node != NULL, PTR_ERROR);

            node->right = new_node;
            break;
        }
        default:
            fprintf(stderr, "wrong side\n");
    }
}

Node* ReplaceNode(size_t* num_of_nodes, Node* node, Node* new_node, int* code_error) {

    MY_ASSERT(node     != NULL, PTR_ERROR);
    MY_ASSERT(new_node != NULL, PTR_ERROR);

    if(node->parent) {
        if(node->parent->left == node) {
            node->parent->left = new_node;
        }
        else if(node->parent->right == node) {
            node->parent->right = new_node;
        }
    }

    new_node->parent = node->parent;

    if(new_node == node->left) {
        FreeNode(num_of_nodes, node->right, code_error);
        free(node);
        (*num_of_nodes)--;
    }
    else if(new_node == node->right) {
        FreeNode(num_of_nodes, node->left, code_error);
        free(node);
        (*num_of_nodes)--;
    }
    else {
        FreeNode(num_of_nodes, node, code_error);
    }

    return new_node;
}

void TreeDtor(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    TREE_ASSERT(tree);

    FreeNode(&(tree->num_of_nodes), tree->root, code_error);
    tree->root = NULL;

    free(tree->data_base);
    tree->data_base = NULL;

    free(tree->tokens);
    tree->tokens = NULL;

    tree->size_data_base = 0;
    tree->num_of_nodes = 0;

}

void FreeNode(size_t* num_of_nodes, Node* node, int* code_error) {

    if(!node) return;

    FreeNode(num_of_nodes, node->left, code_error);
    FreeNode(num_of_nodes, node->right, code_error);

    (*num_of_nodes)--;
}

void ReadTree(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    if(*(tree->data_base + tree->size_data_base - 1) == '$') {
        size_t ip = 0;
        TokensParcing(tree, code_error);
        tree->root = GetTree(&(tree->num_of_nodes), tree->tokens, &ip, code_error);
    }
    else {
        char* copy_data_base = tree->data_base;

        tree->root = ReadNode(tree, tree->root, NULL, code_error);

        tree->data_base = copy_data_base;
    }

}

Node* ReadNode(Tree* tree, Node* node, Node* parent, int* code_error) {

    node = NodeCtor(&(tree->num_of_nodes), DEFAULT, DEFAULT_VALUE, NULL, NULL, parent, code_error);

    while(isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
        tree->data_base++;
    }

    if(*(++tree->data_base) == '(') {
        node->left  = ReadNode(tree, node->left,  node, code_error);
    }

    while(isspace(*(tree->data_base))) {
        tree->data_base++;
    }

    char* data = tree->data_base;

    while(!isspace(*(tree->data_base))) {
        tree->data_base++;
    }

    *(tree->data_base) = '\0';

    Type type = DEFAULT;

    double arg = 0;

    if(OP_CHECK(*data)) {
        type = OP;
        arg = (double)(int)(*data);
    }
    else if(VAR_CHECK(*data)) {
        type = VAR;
        arg = (double)(int)(*data);
    }
    else {
        type = NUM;
        arg = strtod(data, NULL);
    }

    node->data = arg;
    node->type = type;

    if(node->type == NUM || node->type == VAR) {
        while(*(tree->data_base) == ')' || isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
            tree->data_base++;
        }
        return node;
    }

    while(*(tree->data_base) == ')' || isspace(*(tree->data_base))) {
        tree->data_base++;
    }

    if(*(++tree->data_base) == '(') {
        node->right  = ReadNode(tree, node->right,  node, code_error);
    }

    return node;
}

#ifdef DEBUG

    int TreeVerification(const Tree* tree, int* code_error) {

        if(!tree) {
            *code_error |= NO_TREE;
            return *code_error;
        }

        if(!tree->data_base) {
            *code_error |= BASE_ERROR;
        }

        if(!tree->root) {
            *code_error |= NO_ROOT;
            return *code_error;
        }

        *code_error = NodeVerificator(tree->root, code_error);

        return *code_error;
    }

    int NodeVerificator(const Node* node, int* code_error) {

        if(!node) {
            return NO_ERROR;
        }

        if(node->parent != NULL) {
            if(!(node->parent->left == node || node->parent->right == node)) {
                *code_error |= CONNECT_ERROR;
            }
            if(node->parent->left == node && node->parent->right == node) {
                *code_error |= CONNECT_ERROR;
            }
        }

        *code_error |= NodeVerificator(node->left, code_error);
        *code_error |= NodeVerificator(node->right, code_error);

        return *code_error;
    }

#endif

#undef OP_CHECK
#undef VAR_CHECK