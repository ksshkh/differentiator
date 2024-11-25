#include "tree.hpp"
#include "utils.hpp"
#include "treedump.hpp"

#define OP_CHECK(data)  data == ADD || data == SUB || data == MUL || data == DIV || data == DEG
#define VAR_CHECK(data) data == X

void TreeCtor(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    tree->depth = 0;
    tree->root = NULL;

    tree->data_base = ReadInBuff(INPUT_FILE, &(tree->size_data_base), code_error);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    GetTreeDepth(tree, code_error);
}

Node* NodeCtor(Type type, TreeElem data, Node* left, Node* right, Node* parent, int* code_error) {

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    MY_ASSERT(new_node != NULL, PTR_ERROR);

    new_node->type = type;
    new_node->data = data;
    new_node->left = left;
    new_node->right = right;
    new_node->parent = parent;

    if(left)  left->parent  = new_node;
    if(right) right->parent = new_node;

    return new_node;

}

void AddNewNode(Type type, Node* node, TreeElem data, Side side, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    switch(side) {
        case LEFT: {
            Node* new_node = NodeCtor(type, data, node->left, NULL, node, code_error);
            MY_ASSERT(new_node != NULL, PTR_ERROR);

            node->left = new_node;
            break;
        }
        case RIGHT: {
            Node* new_node = NodeCtor(type, data, NULL, node->right, node, code_error);
            MY_ASSERT(new_node != NULL, PTR_ERROR);

            node->right = new_node;
            break;
        }
        default:
            fprintf(stderr, "wrong side\n");
    }

}

void TreeDtor(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    TREE_ASSERT(tree);

    FreeNode(tree->root, code_error);
    tree->root = NULL;

    free(tree->data_base);
    tree->data_base = NULL;

    tree->depth = 0;
    tree->size_data_base = 0;

}

void FreeNode(Node* node, int* code_error) {

    if(!node) return;

    FreeNode(node->left, code_error);
    FreeNode(node->right, code_error);
    free(node);

}

void ReadTree(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    char* copy_data_base = tree->data_base;

    tree->root = ReadPreNode(tree, tree->root, NULL, code_error);

    tree->data_base = copy_data_base;

}

Node* ReadPreNode(Tree* tree, Node* node, Node* parent, int* code_error) {

    while(isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
        tree->data_base++;
    }

    if(*(tree->data_base) == ')') {
        return node;
    }

    while(isspace(*(tree->data_base)) || *(tree->data_base) == '(') {
        tree->data_base++;
    }

    char* data = tree->data_base;

    while(!isspace(*(tree->data_base))) {
        tree->data_base++;
    }

    *(tree->data_base) = '\0';

    Type type = DEFAULT;

    if(OP_CHECK(*data)) {
        type = OP;
    }
    else if(VAR_CHECK(*data)) {
        type = VAR;
    }
    else {
        type = NUM;
    }

    double num = strtod(data, NULL);
    if(!num) num = (double)(int)(*data);
    node = NodeCtor(type, num, NULL, NULL, parent, code_error);

    if(node->type == NUM || node->type == VAR) {
        while(*(tree->data_base) == ')' || isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
            tree->data_base++;
        }
        return node;
    }

    node->left = ReadPreNode(tree, node->left, node, code_error);

    while(*(tree->data_base) == ')' || isspace(*(tree->data_base))) {
        tree->data_base++;
    }

    node->right = ReadPreNode(tree, node->right, node, code_error);

    return node;
}

void GetTreeDepth(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    size_t curtain_depth = 0;

    for(long int i = 0; i < tree->size_data_base; i++) {

        if     (tree->data_base[i] == '(') curtain_depth++;
        else if(tree->data_base[i] == ')') curtain_depth--;

        tree->depth = (curtain_depth > tree->depth) ? curtain_depth : tree->depth;
    }
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